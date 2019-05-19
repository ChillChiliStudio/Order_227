#ifndef AUDIO_H
#define AUDIO_H

#include "Module.h"
#include "Point.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define FACTION_NUM 2
#define VARIATION_PER_SOUND 4
#define SOV 0
#define CAP 1
#define MAX_INFANTRY_NUMBER 8
#define MAX_BUILDING_NUMBER 7
struct _Mix_Music;
struct Mix_Chunk;


enum class TroopType_Sounds
{
	SPAWN,
	MOVING,
	COMFIRMATION,
	HURT,
	SHOT,
	ATTACK,
	MAX

};

enum class BuildingsType_Sounds {

	SPAWN,
	CRITICAL,
    DESTROYED,
	MAX


};

enum class song_type {

	NONE = -1,
	MAIN_MENU,
	IN_GAME,
	WIN,
	LOSE
};

struct Music_Track {

	std::string trackName = "NULL";
	char* path = nullptr;
	int num_loops = 0;

	char* nextTrack_id = "NULL";
	Music_Track* nextTrack = nullptr;
};

struct Music_Song {

	std::string songName = "NULL";
	song_type songType = song_type::NONE;
	std::list<Music_Track*> songtracks_list;

	char* nextSong_id = "NULL";
	Music_Song* nextSong = nullptr;
};


class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&) override;

	bool Start() override;

	// Called before quitting
	bool CleanUp() override;

	// Play a music file
	bool PlayMusic(const char* path, int loops = -1, float fade_time = 0.0f);

	//Check if something is being played
	bool MusicPlaying();

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0, fPoint pos = { 0.0f, 0.0f }, bool spatial = false, int i = -1);

	//Volume Control (Paremeter input and variables use percentage 0 - 100 values, Mixer uses 0 - 128 values)
	//Master Volume
	void SetMasterVolume() const;
	void ChangeMasterVolume(uint vol);
  
	//Music Volume
	uint SetMusicVolume() const;
	uint ChangeMusicVolume(uint vol);
  
	//Sfx Volume
	uint SetChannelVolume(int channel = -1);
	uint ChangeChannelVolume(uint vol, int channel = -1);
  
	//Chunck Volume
	uint SetSfxChunkVolume(uint vol, int id = -1);
  
	//Get functions
	uint* GetMasterVolume() {
		return &masterVolume;
	}
	uint* GetSfxVolume() {
		return &sfxVolume;
	}
	uint* GetMusicVolume() {
		return &musicVolume;
	}

	//Legacy Lucho Methods
	/*void ControlVolume(int vol);
	void ControlMUSVolume(int vol);
	void ControlSFXVolume(int vol);*/

	void FillArrayFX();
	void LoadIntoArray();

public:
	// 0% - 100% Range
	uint masterVolume;	//General Volume
	uint musicVolume;	  //Music Volume
	uint sfxVolume;		  //Sfx Volume

	//Spatial Audio
	float sfxAudioRadius;
	uPoint leftEar;
	uPoint rightEar;
	int earOffset;

	uint SoundTroops_Array[MAX_INFANTRY_NUMBER][(int)TroopType_Sounds::MAX][VARIATION_PER_SOUND];
	uint SoundBuilding_Array[MAX_BUILDING_NUMBER][(int)BuildingsType_Sounds::MAX][VARIATION_PER_SOUND];
	
	int VarsXsound[MAX_INFANTRY_NUMBER][(int)TroopType_Sounds::MAX];
	int VarsXsound_Buildings[MAX_BUILDING_NUMBER][(int)BuildingsType_Sounds::MAX];

	pugi::xml_document SFX_XML;

private:

	_Mix_Music *music = NULL;
	std::list<Mix_Chunk*>	fx;
};

#endif // AUDIO_H