#ifndef BUTTON_H
#define BUTTON_H

#include "Image.h"

struct SDL_Texture;
struct SDL_Rect;

enum class button_state
{
	DISABLED = 0,
	IDLE,
	HOVERING,
	PRESSING,

	MAX_TYPES
};

class Button : public Image
{
public:
	//Constructor
	Button(ui_type type, fPoint center, SDL_Rect spriteRect, SDL_Texture* tex, bool dynamic = false, UI_Element* parent = NULL, std::list<UI_Element*>* children = NULL);

	virtual ~Button();

	// Called each frame (framerate dependant)
	virtual bool Update(float dt);

public:
	virtual void Enable();
	virtual void Disable();

protected:
	virtual button_state CheckCurrentState();
	virtual button_state ButtonStateEffects();

	virtual void OnIdle() {};
	virtual void OnHover() {};
	virtual void OnPress() {};

	virtual void WhileIdle() {};
	virtual void WhileHover() {};
	virtual void WhilePress() {};

protected:
	button_state status;
};

#endif //__BUTTON_H__