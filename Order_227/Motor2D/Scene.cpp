#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Fonts.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Audio.h"
#include "PathFinding.h"
#include "Entity_Manager.h"
#include "UserInterface.h"
#include "ButtonActions.h"
#include "ParamBox.h"
#include "Scene.h"
#include "GroupManager.h"


Scene::Scene() : Module()
{
	name.assign("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	return true;
}

// Called before the first frame
bool Scene::Start()
{
	srand(time(NULL));
	if(myApp->map->Load("Map1_0.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(myApp->map->CreateWalkabilityMap(w, h, &data))
			myApp->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	ingame_song3_loop = "audio/music/game/ingame_song3_loop.ogg";
	ingame_song3_intro = "audio/music/game/ingame_song3_intro.ogg";
	ingame_song3_outro = "audio/music/game/ingame_song3_outro.ogg";

	ingame_song4_loop = "audio/music/game/ingame_song4_loop.ogg";
	ingame_song4_intro = "audio/music/game/ingame_song4_intro.ogg";

	myApp->audio->ControlSFXVolume(100);
	myApp->audio->ControlMUSVolume(100);
	myApp->audio->ControlVolume(100);

	myApp->audio->PlayMusic(ingame_song3_intro, 0, 0.0f);
	current_song = ingame_song3_intro;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{

	if (myApp->audio->MusicPlaying() == false)
		ManageMusic();

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

	myApp->map->Draw();

	if (myApp->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		ChooseSpawningPoints();

	//Spawn Point Draw
	if (myApp->map->mapDebugDraw)	// TODO: This shouldn't be hereeeee
		for (int i = 0; i < SpawningPoints_Array.size(); i++) 
			myApp->render->Blit(myApp->map->debug_tex, SpawningPoints_Array[i]->position.x, SpawningPoints_Array[i]->position.y);
		

	//	myApp->render->DrawQuad(SpawningPoints_Array[i]->SP_Rect, 255, 100, 100);
	for (int i = 0; i < SpawningPoints_Array.size(); i++) {

		if (SpawningPoints_Array[i]->Enemies_to_Spawn.size() > 0 && SpawningPoints_Array[i]->SpawnTime.Read() > 500) {

					fPoint SP_Pos = fPoint(SpawningPoints_Array[i]->position.x, SpawningPoints_Array[i]->position.y);
					SpawningPoints_Array[i]->SpawnTime.Start();

					myApp->entities->ActivateInfantry(SP_Pos, infantry_type::BASIC, entity_faction::CAPITALIST);
					SpawningPoints_Array[i]->Enemies_to_Spawn.pop_back();

		}
	}

	myApp->gui->Draw();
	return true;
}


// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(myApp->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	for (int i = SpawningPoints_Array.size() - 1; i >= 0; i--)
		RELEASE(SpawningPoints_Array[i]);

	SpawningPoints_Array.clear();

	return true;
}

void Scene::ChooseSpawningPoints()
{

	//Restarting round if reached 20 - for MVP this should be 5
	if (roundNumber == 20) {

		roundNumber = 0;
		roundThreat = 0;
	}

	//This is VERY HARDCODED - I'm not proud of this, but will work for now (fck this shit) - Is just the manager of round threat TODO
	if (roundNumber == 0 || roundNumber == 1)
		threatIncremental = 10;
	else if (roundNumber % 3 == 0)
		threatIncremental += 10;
	else
		threatIncremental = 0;

	LOG("Round num: %i   Threat: %i   ThreatInc: %i", roundNumber, roundThreat, threatIncremental);

	roundThreat += threatIncremental;
	roundNumber++;

	//Reseting spawning points (put them at false to choose between them)
	for (int i = 0; i < SpawningPoints_Array.size(); i++)
		SpawningPoints_Array[i]->active = false;


	//If we want more spawning points just put more of these
	int r1 = rand() % SpawningPoints_Array.size(); //For ranges not starting at 0: rand()%X + Y --> Range of rands between Y and X
	int r2 = rand() % SpawningPoints_Array.size(); //Rand num between 0 and array's size (if 4 SP, then 0-4)

	if (SpawningPoints_Array.size() > 1) {

		while (r2 == r1)
			r2 = rand() % SpawningPoints_Array.size(); //This can be pretty unpredictable & uncontrolling shit
	}

	SpawningPoints_Array[r1]->active = true;
	SpawningPoints_Array[r2]->active = true;

	for (int i = 0; i < SpawningPoints_Array.size(); i++)
		if (SpawningPoints_Array[i]->active == true)
			SpawningPoints_Array[i]->FillEnemies(roundThreat);

}


void Scene::ManageMusic() {

	next_song = ChooseNextSong(current_song);
	myApp->audio->PlayMusic(next_song, 0, 0.0f);
	current_song = next_song;

}


char* Scene::ChooseNextSong(char* currSong) {

	char *ret = nullptr;

	if (currSong == ingame_song3_intro)
		ret = ingame_song3_loop;
	else if (currSong == ingame_song3_outro)
		ret = ingame_song4_intro;
	else if (currSong == ingame_song4_intro)
		ret = ingame_song4_loop;
	else if (currSong == ingame_song3_loop) {

		if (songRepetitions >= 0) {

			ret = ingame_song3_outro;
			songRepetitions = 0;
		}
		else {

			ret = ingame_song3_loop;
			songRepetitions++;
		}

	}
	else if (currSong == ingame_song4_loop) {

		if (songRepetitions >= 0) {

			ret = ingame_song3_intro;
			songRepetitions = 0;
		}
		else {

			ret = ingame_song4_loop;
			songRepetitions++;
		}
	}

	return ret;
}