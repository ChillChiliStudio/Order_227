#include "Geometry.h"
#include "Render.h"
#include "Textures.h"
#include "Scene.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Unit.h"

Unit::Unit(unit_type unitType, fPoint pos, faction_enum faction) : Entity(entity_type::UNIT, pos), unitFaction(faction), unitType(unitType)
{
	life = 1;

	if (faction == faction_enum::CAPITALIST) {

		switch (unitType) {

		case (unit_type::INFANTRY):
			texture = myApp->scene->TestTexture;
			break;
		case(unit_type::UNKNOWN):
			break;
		default:
			break;
		}

		//hostileUnits = &myApp->entities->enemiesList;
	}
	else if (faction == faction_enum::COMMUNIST) {

		switch (unitType) {

		case (unit_type::INFANTRY):
			texture = myApp->scene->TestTexture;
			break;
		case(unit_type::UNKNOWN):
			break;
		default:
			break;
		}

		//hostileUnits = &myApp->entities->alliesList;
	}

	

}

Unit::~Unit()
{}

bool Unit::Update(float dt)
{
	UnitWorkflow(dt);

	unitRect = {12, 0, 55,47};
	CheckInCamera = {(int)position.x, (int)position.y, unitRect.w, unitRect.h };
//	Draw();

	if (life <= 0)	//TODO: This should be included inside the workflow AND must work with entity pools
		myApp->entities->DestroyEntity(this);


	if (myApp->render->InsideCamera(CheckInCamera) == true) {

		UpdateBlitOrder();

		myApp->render->Push(order, texture, position.x, position.y, &unitRect);

	}

	return true;
}

bool Unit::Draw() {

	myApp->render->Blit(texture, (int)position.x, (int)position.y, &unitRect);
	return true;
}

void Unit::UpdateBlitOrder() {

	std::list<Entity*>::iterator item = myApp->entities->entities_list.begin();
	while (item != myApp->entities->entities_list.end()) {

		if ((*item) != this) {

			if (this->position.y > (*item)->position.y)
				order += 1;
			else
				order -= 1;


		}
		item = next(item);
	}

}

// Main workflow
void Unit::UnitWorkflow(float dt)
{
	unit_state prevState = unitState;

	switch (unitOrders) {
	case unit_orders::HOLD:
		DoHold(dt);
		break;
	case unit_orders::MOVE:
		DoMove(dt);
		break;
	case unit_orders::ATTACK:
		DoAttack(dt);
		break;
	case unit_orders::MOVE_AND_ATTACK:
		DoMoveAndAttack(dt);
		break;
	case unit_orders::PATROL:
		DoPatrol(dt);
		break;
	}

	if (prevState != unitState) {
		ApplyState();
	}
}

void Unit::ApplyState()
{
	switch (unitState) {
	case unit_state::IDLE:
		//currentAnim = idleAnim;
		break;
	case unit_state::MOVING:
		//currentAnim = movingAnim;
		break;
	case unit_state::FIRING:
		//currentAnim = firingAnim;
		break;
	case unit_state::DEAD:
		//currentAnim = deadAnim;
		break;
	}
}

// Order processing
void Unit::DoHold(float dt)
{
	switch (unitState) {
	case unit_state::IDLE:
		target = EnemyInRange();
		if (target != nullptr) {
			AttackTarget();
		}
		break;
	case unit_state::FIRING:
		if (TargetInRange() && target->IsDead() == false) {
			AttackTarget();
		}
		else {
			target = nullptr;
			unitState = unit_state::IDLE;
		}
		break;
	}
}

void Unit::DoMove(float dt)
{
	if (DestinationReached() == false) {	//NOTE: Pathfinding should define destination as the tile where a specific unit should be even if it's in a group
		Move(dt);
	}
	else {
		StartHold();
		unitState = unit_state::IDLE;
	}
}

void Unit::DoAttack(float dt)
{
	if (target->IsVisible() == true) {
		if (TargetDisplaced() == true) {
			origin = { (int)position.x, (int)position.y };
			destination = { (int)target->position.x, (int)target->position.y };
			//TODO (LuchoAlert): Recalculate unit pathfinding using origin and destination
		}
	}

	if (target->IsDead() == false) {
		if (TargetInRange() == true) {
			AttackTarget();
		}
		else {
			Move(dt);
		}
	}
	else {
		StartHold();
		unitState = unit_state::IDLE;
	}
}

void Unit::DoMoveAndAttack(float dt)
{
	if (DestinationReached() == false) {	//NOTE: Pathfinding should define destination as the tile where a specific unit should be even if it's in a group
		if (unitState == unit_state::IDLE || unitState == unit_state::MOVING) {
			target = EnemyInRange();
			if (target != nullptr) {
				AttackTarget();
			}
			else {
				Move(dt);
			}
		}
		else if (unitState == unit_state::FIRING) {
			if (TargetInRange() && target->IsDead() == false) {
				AttackTarget();
			}
			else {
				target = nullptr;
				Move(dt);
			}
		}
	}
	else {
		StartHold();
		unitState = unit_state::IDLE;
	}
}

void Unit::DoPatrol(float dt)
{
	if (DestinationReached() == false) {	//NOTE: Pathfinding should define destination as the tile where a specific unit should be even if it's in a group
		if (unitState == unit_state::IDLE || unitState == unit_state::MOVING) {
			target = EnemyInRange();
			if (target != nullptr) {
				AttackTarget();
			}
			else {
				Move(dt);
			}
		}
		else if (unitState == unit_state::FIRING) {
			if (TargetInRange() && target->IsDead() == false) {
				AttackTarget();
			}
			else {
				target = nullptr;
				Move(dt);
			}
		}
	}
	else {
		StartPatrol(origin);
		unitState = unit_state::IDLE;
	}
}

// Actions
bool Unit::Move(float dt)	//TODO: Make it so unit goes straight to the destination (divide speed into x and y, and use hipotenuse angle to decide how much it applies to each)
{
	if (destination.x > (int)position.x) {
		position.x += (angledSpeed.x * dt);
	}
	else if (destination.x < (int)position.x) {
		position.x -= (angledSpeed.x * dt);
	}

	if (destination.y > (int)position.y) {
		position.y += (angledSpeed.y * dt);
	}
	else if (destination.y < (int)position.y) {
		position.y -= (angledSpeed.y * dt);
	}

	unitState = unit_state::MOVING;
	return true;
}

void Unit::AttackTarget()
{
	unitState = unit_state::FIRING;
}

// Unit Data
bool Unit::IsDead()
{
	if (life <= 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Unit::IsVisible()
{
	return true;	//TODO: Make function
}

bool Unit::DestinationReached()
{
	if (position.x != (float)destination.x || position.y != (float)destination.y) {
		return false;
	}
	else {
		return true;
	}
}

bool Unit::TargetDisplaced()
{
	if (target->position.x != destination.x || target->position.y != destination.y) {
		return true;
	}
	else {
		return false;
	}
}

// Unit Calculations
Unit* Unit::EnemyInRange()
{
	Unit* ret = nullptr;

	/*for (std::list<Unit*>::iterator iter = hostileUnits->begin(); iter != hostileUnits->end(); ++iter) {
		if ((*iter)->position.x > position.x + attackRange || (*iter)->position.x < position.x - attackRange
			|| (*iter)->position.y > position.y + attackRange || (*iter)->position.y < position.y + attackRange) {
			continue;
		}
		else {
			if (InsideRadius(position, attackRange, (*iter)->position) == true) {
				ret = (*iter);
				break;
			 }
		}
	}*/

	return ret;
}

bool Unit::TargetInRange()
{
	return InsideRadius(position, attackRange, target->position);
}

fPoint Unit::SetupAngledSpeed()
{
	return { 1.0f, 1.0f };	//TODO Carles
}

// Order calling
void Unit::StartHold()
{
	//TODO  (LuchoAlert): pls setup pathfinding stop
	//- Stop/Cancel Pathfinding
	origin = destination = { (int)position.x, (int)position.y };
	target = nullptr;
	unitOrders = unit_orders::HOLD;
	unitState = unit_state::IDLE;
}

void Unit::StartMove(iPoint destination)
{
	//TODO (LuchoAlert) pls setup unit pathfinding
	origin = { (int)position.x, (int)position.y };
	this->destination = destination;
	//Set pathfinding using origin and destination
	target = nullptr;
	unitOrders = unit_orders::MOVE;
	unitState = unit_state::IDLE;
}

void Unit::StartAttack(Unit* target)
{
	//TODO (LuchoAlert): Lucho pls setup unit pathfinding
	origin = { (int)position.x, (int)position.y };
	destination = { (int)target->position.x, (int)target->position.y };
	//Set pathfinding using origin and destination
	this->target = target;
	unitOrders = unit_orders::ATTACK;
	unitState = unit_state::IDLE;
}

void Unit::StartMoveAndAttack(iPoint destination)
{
	//TODO (LuchoAlert): Lucho pls setup unit pathfinding
	origin = { (int)position.x, (int)position.y };
	this->destination = destination;
	//Set pathfinding using origin and destination
	target = nullptr;
	unitOrders = unit_orders::MOVE_AND_ATTACK;
	unitState = unit_state::IDLE;
}

void Unit::StartPatrol(iPoint destination)
{
	//TODO (LuchoAlert): Lucho pls setup unit pathfinding
	origin = { (int)position.x, (int)position.y };
	this->destination = destination;
	//Set pathfinding using origin and destination
	target = nullptr;
	unitOrders = unit_orders::PATROL;
	unitState = unit_state::IDLE;
}