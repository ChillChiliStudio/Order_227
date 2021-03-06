#include "Geometry.h"
#include "Textures.h"
#include "Entity_Manager.h"
#include "Unit.h"
#include "Projectile.h"
#include "Log.h"
#include "Render.h"
Projectile::Projectile() {
	
}

Projectile::~Projectile() {

}

void Projectile::Fly() {
	LOG("I BELIEVE I CAN FLAY");
	CheckDirection(ProjectileVecSpeed);
	SetupVecSpeed();
	CheckExplosion();
}

bool Projectile::Update(float dt) {
	

	position.x += (ProjectileVecSpeed.x * dt);
	position.y += (ProjectileVecSpeed.y * dt);

	entityRect.x = position.x;
	entityRect.y = position.y;
	entityRect.w = 10;
	entityRect.h = 10;

	centerPos.x = position.x + (entityRect.w / 2);
	centerPos.y = position.y + (entityRect.h / 2);


	spriteRect = (myApp->entities->ParticleAnimArray[(int)ProjectileDirection]);
	

	myApp->render->Blit(texture, (int)position.x, (int)position.y, &spriteRect);


	return true;
}

projectile_directions Projectile::CheckDirection(fVec2 direction)
{
	ProjectileVecAngle = direction.GetAngle({ 0.0f, -1.0f });
	ProjectileVecAngle = RadsToDeg(ProjectileVecAngle);

	if (ProjectileVecAngle > 337.5f || ProjectileVecAngle <= 22.5f) {
		ProjectileDirection = projectile_directions::NORTH;
	}
	else if (ProjectileVecAngle > 292.5f) {
		ProjectileDirection = projectile_directions::NORTH_EAST;
	}
	else if (ProjectileVecAngle > 247.5f) {
		ProjectileDirection = projectile_directions::EAST;
	}
	else if (ProjectileVecAngle > 202.5f) {
		ProjectileDirection = projectile_directions::SOUTH_EAST;
	}
	else if (ProjectileVecAngle > 157.5f) {
		ProjectileDirection = projectile_directions::SOUTH;
	}
	else if (ProjectileVecAngle > 112.5f) {
		ProjectileDirection = projectile_directions::SOUTH_WEST;
	}
	else if (ProjectileVecAngle > 67.5f) {
		ProjectileDirection = projectile_directions::WEST;
	}
	else if (ProjectileVecAngle > 22.5f) {
		ProjectileDirection = projectile_directions::NORTH_WEST;
	}

	return ProjectileDirection;
}

fVec2 Projectile::SetupVecSpeed()
{
	fPoint nodePos = { (Destination.x), (Destination.y) };

	ProjectileVecSpeed = GetVector2(position, nodePos);
	ProjectileVecSpeed = ProjectileVecSpeed.GetUnitVector();
	ProjectileVecSpeed *= 200;
	return ProjectileVecSpeed;
}

void Projectile::CheckExplosion() {
	if (this->position.x - Destination.x < 10 && this->position.x - Destination.x > -10 && this->position.y - Destination.y < 10 && this->position.y - Destination.y > -10) {
		Explode();
	}
}

void Projectile::Explode() {
	LOG("EXPLOSION");
	
	for (std::vector<Unit>::iterator item = myApp->entities->unitPool.begin(); item != myApp->entities->unitPool.end(); item = next(item)) {
		if ((*item).active == true && (*item).IsDead() == false /*&& (*item).faction != faction*/) {

			if (InsideSquareRadius((this->centerPos), RANGE, (*item).position) && InsideRadius((this->centerPos), RANGE, (*item).position))
			{
				(*item).Hurt(this->damage);
			}

		}
	}
	for (std::vector<Launcher>::iterator item = myApp->entities->launcherPool.begin(); item != myApp->entities->launcherPool.end(); item = next(item)) {
		if ((*item).active == true && (*item).IsDead() == false /*&& (*item).faction != faction*/) {

			if (InsideSquareRadius((this->centerPos), RANGE, (*item).position) && InsideRadius((this->centerPos), RANGE, (*item).position))
			{
				(*item).Hurt(this->damage);
			}

		}
	}	
	for (int i = 0; myApp->entities->buildingsArray[i].active; i++) {
		if (myApp->entities->buildingsArray[i].active) {
			if (InsideSquareRadius((this->centerPos), RANGE, myApp->entities->buildingsArray[i].position) && InsideRadius((this->centerPos), RANGE, myApp->entities->buildingsArray[i].position))
				myApp->entities->buildingsArray[i].Hurt(this->damage);
		}
	}

	this->active = false;
	
}