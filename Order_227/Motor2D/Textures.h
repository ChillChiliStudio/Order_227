#ifndef TEXTURES_H
#define TEXTURES_H

#include "Module.h"
#include <list>

struct SDL_Texture;
struct SDL_Surface;
struct SDL_Rect;

class Textures : public Module
{
public:

	Textures();

	// Destructor
	virtual ~Textures();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const	Load(const char* path);
	bool				UnLoad(SDL_Texture* texture);
	SDL_Texture* const	LoadSurface(SDL_Surface* surface);
	void				GetSize(const SDL_Texture* texture, uint& width, uint& height) const;
	SDL_Rect			GetSize(const SDL_Texture* texture) const;

public:

	std::list<SDL_Texture*>	textures;
};


#endif // TEXTURES_H