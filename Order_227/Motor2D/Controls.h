#ifndef CONTROLS_H
#define CONTROLS_H	//@Carles

#include <vector>
#include "SDL/include/SDL.h"
#include "Module.h"

//struct input_button	//IMPROVEMENT: The right way to do it
//{
//	int id;
//	enum class channel(keyboard / mouse / controller);
//};

class Key_Config_Box;

struct debug_controls
{
	int debugMap;
	int debugUI;
	int debugEntities;
	int nextRound ;
	//int clearEnemies;

	int spawnBlueDog;
	int spawnBlueChrono;
	int spawnBlueSniper;
	
	int spawnRedChrono;
	int spawnRedDesolator;
	int spawnRedBazooka;
	int spawnRedConscript;
};

struct mouse_controls
{
	int select;
	int issueOrders;
};

struct camera_controls
{
	int up;
	int down;
	int left;
	int right;
};

struct order_controls
{
	int hold;
	int hunt;
	int patrol;

	int defensive;
	int aggressive;
};

class Controls : public Module
{
public:
	Controls();
	~Controls();

	bool Awake(pugi::xml_node& node) override;
	bool Start() override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	// Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

public:
	void AllocateLists();

	void PrepareInputChange(int* selectedInput);
	std::string TranslateKeycode(int i);
	void CreateKeyboardStrings();

	bool MouseButtonInUse(int button);
	bool KeyInUse(int key);

	void UpdateMouseButton(int newVal);
	void UpdateKey(int newVal);

public:
	int pause = SDL_SCANCODE_ESCAPE;

	debug_controls debug;
	mouse_controls mouse;
	camera_controls camera;
	order_controls orders;

	int spawnHotKeys[6];

	std::string keyNames[SDL_NUM_SCANCODES];
	std::vector<int> mouseButtonsInUse;
	std::vector<int> keysInUse;

public:
	Key_Config_Box* buttonUsed = nullptr;
	bool awaitingInput = false;
	int* inputToChange = nullptr;
};

#endif	// CONTROLS_H