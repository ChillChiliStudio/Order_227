#ifndef ACTION_BOX_H
#define ACTION_BOX_H

#include "Button.h"

template <class Ret, class... Args>
UI_Element* NewActionBox(Ret(*action)(Args...), fPoint center, SDL_Rect spriteList[4], SDL_Texture* tex = NULL, UI_Element* parent = NULL)
{
	UI_Element* ret = nullptr;

	if (tex == NULL) {
		tex = myApp->gui->GetAtlas();
	}

	ret = new Action_Box<Ret, Args...>(action, center, spriteList, tex, parent);
	myApp->gui->AddElement(ret);

	return ret;
}

template <class Ret, class... Args>
class Action_Box : public Button
{
public:
	typedef Ret(*eventFunction)(Args...);

	//Constructor
	Action_Box(eventFunction action, fPoint center, SDL_Rect spriteList[4], SDL_Texture* tex, UI_Element* parent = NULL)
		: Button(ui_type::BUTTON_ACTION, center, spriteList[(int)button_state::IDLE], tex, false, parent, NULL), action(action)
	{
		stateSprites = new SDL_Rect[(int)button_state::MAX_TYPES];

		for (int i = 0; i < (int)button_state::MAX_TYPES; i++) {
			stateSprites[i] = spriteList[i];
		}
	};

	virtual ~Action_Box()
	{
		RELEASE(sprite);
		RELEASE_ARRAY(stateSprites);
	}

	//Button action calling
	virtual Ret operator() (Args&... args) const{
		return (action)(args...);
	}

	virtual Ret DoAction(Args&... args) const {
		return (action)(args...);
	}

public:
	//Enable/Disable
	virtual void Enable()
	{
		status = button_state::IDLE;
		*sprite = stateSprites[(int)button_state::IDLE];
	}

	virtual void Disable()
	{
		status = button_state::DISABLED;
		*sprite = stateSprites[(int)button_state::DISABLED];
	}

protected:
	virtual void OnIdle()
	{
		*sprite = stateSprites[(int)button_state::IDLE];
	}

	virtual void OnHover()
	{
		*sprite = stateSprites[(int)button_state::HOVERING];
	}

	virtual void OnPress()
	{
		//myApp->audio->PlayFx(myApp->audio->buttonSfx.id, 0);
		*sprite = stateSprites[(int)button_state::PRESSING];
		DoAction(Args...);
	}

	virtual void WhileIdle() {}

	virtual void WhileHover() {}

	virtual void WhilePress() {}

protected:
	eventFunction action;
	SDL_Rect* stateSprites = nullptr;	//Disabled, Idle, Hover, Pressed
};

#endif //ACTION_BOX_H