#pragma once

/*
 ▄▀▀▄ ▄▀▄  ▄▀▀█▄   ▄▀▀▀▀▄   ▄▀▀█▄▄▄▄ 
█  █ ▀  █ ▐ ▄▀ ▀▄ █        ▐  ▄▀   ▐ 
▐  █    █   █▄▄▄█ █    ▀▄▄   █▄▄▄▄▄  
  █    █   ▄▀   █ █     █ █  █    ▌  
▄▀   ▄▀   █   ▄▀  ▐▀▄▄▄▄▀ ▐ ▄▀▄▄▄▄   
█    █    ▐   ▐   ▐         █    ▐   
▐    ▐                      ▐        
 
 Multiplatform Arcade Game Engine
        See licence.txt.
*/

#include "platform.h" // OS stuff
#include "registry.h"
#include "hook.h"
#include "gmo.h"
#include "timer.h"

namespace mage {

class jukebox;
class mixer;
class inputMouse;
class inputKeyboard;
class gameStateMngr;
class gameState;
class resourceMngr;
class scheduleMngr;
class prefabMngr;
class scriptingEngine;
class batchRenderer;
class loadingScreen;
class resourceTexture;
class view;

// The game class is the core of our game.
// It holds all of the managers and the window.
// It has our main loop.
// It has all of the top-down functions that examine global listings.

// It is not THE game; if the game only consisted of the Game class it would be a broken game,
//  missing many important subsystems.

// You can and should derive this for your own applications.
// The macro DefineGame(classname) will allow you to have your game instance run instead of main().
// Alternatively, you can just use the base game class with DefineGame(Game). Writing your own classes is still trivial with the scripting engine functions.

class MAGEDLL Game
{
public:
	Game(int argc, char* argv[], sf::RenderWindow* wind);
	~Game();

	bool renderWindowExists();
	void windowInit(sf::RenderWindow* window);

	virtual int run(bool tickByTick = false);

	virtual void draw();
	virtual void update(interval elapsed);

	void setWindowSettings(point2U resolution, std::string title, bool fullscreen, bool vsync, int maxFps);
	void setAudioSettings(float masterVolume, float sfxVolume, float musicVolume);

	virtual void fixViews();

	virtual float getDelta(); // delta - progress through a frame from 0.0 to 1.0

	std::string version();

	// window settings
	bool isFullscreen();
	bool isVsyncEnabled();
	int getFramerateLimit();
	point2U getScreenResolution();
	std::string getWindowTitle();

	void setScreenResolution(point2U res);
	void setFullscreen(bool tf);
	void setVsyncEnabled(bool tf);
	void setFramerateLimit(int fpsl);
	void setWindowTitle(std::string t);

	interval getSimTime();

	sf::RenderWindow& getRenderWindow();

public:
	// SUBSYSTEMS
	std::shared_ptr<inputMouse> mouse;
	std::shared_ptr<inputKeyboard> keyboard;
	std::shared_ptr<jukebox> music;
	std::shared_ptr<mixer> sound;
	std::shared_ptr<resourceMngr> resources;
	std::shared_ptr<prefabMngr> prefabs;
	std::shared_ptr<batchRenderer> renderer;
	std::shared_ptr<scriptingEngine> scripting;
	std::shared_ptr<scheduleMngr> scheduler;

	std::shared_ptr<gameStateMngr> states;
	std::shared_ptr<gameState> state;

	std::shared_ptr<view> camera;

	hook<> onGameInitDone;
	hook<> onGamePreUpdate;
	hook<> onGameUpdate;
	hook<> onGameDraw;
	hook<> onGameDone;

	unsigned int mouseMode;
	std::shared_ptr<resourceTexture> mouseTexture;

	bool showFps;
	bool debug;
	bool isInFocus;

	float fps, tps;

	bool worldObjectsFrozen;
	bool conservativeMouse;

	bool colBoxes;
	bool inMainUpdateLoop;

	float tickScale;

	bool doRestart;

protected:
	void loadWindowSettings(point2U resolution = point2U(1280, 720), std::string title = "MAGE Game Engine", bool fullscreen = false, bool vsync = false, int maxFps = 60);
	void loadAudioSettings(float masterVolume = 100.f, float sfxVolume = 100.f, float musicVolume = 100.f);

	virtual void setKeybinds();
	virtual void loadResources();

protected:
	registry windowSettings;
	registry audioSettings;

	timer tickClock;
	interval missedTime;

	timer magicClock;

	timer updateTime;

	bool consoleMode;

	std::string windowTitle;

private:
	void init();
	sf::RenderWindow* window;
};

MAGEDLL Game* theGame();

} // namespace mage

#define MAGE_DeclareGame(typ) int main(int argc, char *argv[]) {\
sf::RenderWindow wind;\
\
typ gm(argc, argv, &wind);\
return gm.run();\
}
