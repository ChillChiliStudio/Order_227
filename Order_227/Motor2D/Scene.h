#ifndef SCENE_H
#define SCENE_H

#include "Module.h"
#include "Spawning_Point.h"
#include <vector>
#include <time.h>
#include "PerfTimer.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;

class Scene : public Module
{

public:

	Scene();
	
	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

<<<<<<< HEAD
=======
public:
>>>>>>> GroupsMovement_Jaume

	void ChooseSpawningPoints();
	SDL_Texture* TestTexture = nullptr;

<<<<<<< HEAD
public:

std::vector<Spawning_Point*> SpawningPoints_Array;

=======
>>>>>>> GroupsMovement_Jaume
private:

	//Spawning & SP
	int roundThreat = 0;
	int threatIncremental = 0;
	int roundNumber = 0;
	//List of Spawning points


	pugi::xml_document SP_Doc;

<<<<<<< HEAD
	//Pathfinding Debug Stuff
	SDL_Texture* debug_tex;
	PerfTimer PathfindingTimer;
	float Ptime = 0.0f;

=======
	
>>>>>>> GroupsMovement_Jaume
};

#endif // SCENE_H
