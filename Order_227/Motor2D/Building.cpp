#include "Building.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "Entity_Manager.h"
#include "UserInterface.h"
#include "Text.h"

Building::Building()
{}

Building::Building(fPoint position, building_type building_type, entity_faction faction) : Entity(position, entity_type::BUILDING, faction)
{}

bool Building::Start() {

	myApp->gui->CreateLifeBar(fPoint(position.x, position.y), NULL, myApp->entities->lifeBar_tex, &health);
	return true;
}

bool Building::Update(float dt)
{

	this;
	if(faction == entity_faction::COMMUNIST) {

		if (rewardGiven == false)
			GiveReward();

		if (health <= 0) {

			faction = entity_faction::NEUTRAL;
			health = 0;
			TakeReward();
		}
		else
			repairable = true;
	}
	else if (faction == entity_faction::NEUTRAL && health > 0)
			faction = entity_faction::COMMUNIST;


	Draw();

	if (myApp->map->mapDebugDraw)
		DebugDraw();
	

	return true;
}


void Building::GiveReward() {

	rewardGiven = true;
	myApp->player->playerIncome += income;

	//if (buildingType == building_type::TANK_FACTORY)
	//	player->heavyUnitsUnlocked = true; //TODO: Tocar UI con esto

	if (buildingType == building_type::HTPC)
		unitBuff = true; //Tocar en ActivateUnits() que si es true, le suba velocidad y vida de los buffs

	else if (buildingType == building_type::EPC) {

		healthRecovery *= 1.5;
		myApp->entities->mainBase->health += MainBaseLifeBuff;
		int i = 0;

		//while (i < myApp->entities->buildingsArray.size()) {
		//	if (myApp->entities->buildingsArray[i].buildingType != building_type::MAIN_BASE)
		//		myApp->entities->buildingsArray[i].maxHealth += StrategicPointsLifeBuff;

		//	myApp->entities->buildingsArray[i].health = maxHealth;
		//	i++;
		//}

		for(int i = 0; i < myApp->entities->buildingsArray.size(); i++) {

			if (myApp->entities->buildingsArray[i].buildingType != building_type::MAIN_BASE)
				myApp->entities->buildingsArray[i].maxHealth += StrategicPointsLifeBuff;

			myApp->entities->buildingsArray[i].health = maxHealth;
		}
	}

}


void Building::TakeReward() {

	rewardGiven = false;
	myApp->player->playerIncome -= income;

	//if (buildingType == building_type::TANK_FACTORY)
	//	player->heavyUnitsUnlocked = false; //TODO: Tocar UI con esto

	if (buildingType == building_type::HTPC)
		unitBuff = false;

	else if (buildingType == building_type::EPC) {

		healthRecovery /= 1.5;
		myApp->entities->mainBase->health -= MainBaseLifeBuff;

		for (int i = 0; i < myApp->entities->buildingsArray.size(); i++) {

			if (myApp->entities->buildingsArray[i].buildingType != building_type::MAIN_BASE)
				myApp->entities->buildingsArray[i].maxHealth -= StrategicPointsLifeBuff;

			myApp->entities->buildingsArray[i].health = maxHealth;
		}
	}
}


bool Building::CleanUp()
{
	return true;
}

bool Building::Draw()
{
	myApp->render->Push(order, texture, (int)position.x, (int)position.y, &spriteRect);
	return true;
}

bool Building::DebugDraw()
{
	return myApp->render->DrawQuad(entityRect, 255, 0, 0, 255, false);
}

float Building::Hurt(float damage)
{
	health -= damage;

	if (health <= 0.0f) {
		//Die();
	}

	return health;
}

float Building::Repair()
{
	health += healthRecovery;

	if (health >= maxHealth)
		health = maxHealth;

	return health;
}

bool Building::IsDead()
{
	bool ret = false;

	if (health <= 0.0f) {
		ret = true;
	}

	return ret;
}

bool Building::IsVisible()
{
	return true;	//TODO: Make function with Fog of War
}