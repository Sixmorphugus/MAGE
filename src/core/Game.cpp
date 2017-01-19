#include "Game.h"

// SUBSYSTEMS
#include "view.h"
#include "gameState.h" // includes group, which includes basic
#include "inputKeyboard.h" // IO stuff
#include "inputMouse.h" // IO stuff
#include "resources.h" // resource stuff
#include "prefabs.h" // prefab stuff
#include "jukebox.h"
#include "mixer.h"
#include "scriptingEngine.h"
#include "schedules.h"
#include "renderer.h"

// MISC
#include "helpers.h"
#include "constants.h"
#include "resourceFont.h"
#include "resourceScriptRoot.h"
#include "resourceSoundBuffer.h"
#include "resourceTexture.h"
#include "resourceFont.h"
#include "resourceShader.h"
#include "view.h"
#include "stringHelpers.h"

using namespace mage;

// LE GAME ENGINE
// SIMPLES

Game* globalGame = nullptr;

// CONSTRUCTOR, DESTRUCTOR
// -------------------------------------------------------------------------------------

Game::Game(int argc, char* argv[], sf::RenderWindow* wind):
	window(wind)
{
	if (!globalGame)
		globalGame = this;
	else
		p::fatal("Two instances of the Game core class cannot exist in the same process. Game is a singleton.");

	if (!platform::init()) {
		p::fatal("Platform init failed!\nPlease check that you're running the latest version of the game.");
	}

	init();

	p::log("\nREADY TO START", true);
	p::log("------------------------------------------", true);
}

Game::~Game()
{
	onGameDone.notify();
	platform::deInit();
}

void Game::init()
{
	// reboot safety
	onGameDone.clearObservers();
	onGameDraw.clearObservers();
	onGameInitDone.clearObservers();
	onGamePreUpdate.clearObservers();
	onGameUpdate.clearObservers();

	// actual boot stuff
	debug = false;

	p::log("\nMAGE GAME ENGINE r" + MAGE_BUILDSTRING + " for " + MAGE_PLATFORM_STRING + "\nCopyright Deadhand 2016.\n", true);

	p::log("SFML v" + std::to_string(SFML_VERSION_MAJOR) + "." + std::to_string(SFML_VERSION_MINOR) + "." + std::to_string(SFML_VERSION_PATCH), true);
	p::log("CHAISCRIPT v" + std::to_string(chaiscript::version_major) + "." + std::to_string(chaiscript::version_minor) + "." + std::to_string(chaiscript::version_patch) + "\n", true);

	p::log("INITIALIZING", true);
	p::log("------------------------------------------", true);

	// CREATE SUBSYSTEMS
	p::info("STARTING SUBSYSTEMS");

	p::log("\tInput...");
	keyboard = std::make_shared<inputKeyboard>();
	mouse = std::make_shared<inputMouse>();

	p::log("\tAudio...");
	sound = std::make_shared<mixer>();
	music = std::make_shared<jukebox>();

	p::log("\tResources and Prefabs...");
	resources = std::make_shared<resourceMngr>();
	prefabs = std::make_shared<prefabMngr>();

	p::log("\tRenderer...");
	mainRenderer = std::make_shared<renderer>();

	p::log("\tCamera...");
	camera = std::make_shared<view>();

	p::log("\tScripting...");
	scripting = std::make_shared<scriptingEngine>();

	p::log("\tScheduler...");
	scheduler = std::make_shared<scheduleMngr>();

	// state system is technically a subsystem but SHOWMANSHIP!
	p::info("Creating game world.\n");
	states = std::make_shared<gameStateMngr>();

	// INIT COMPONENTS
	scripting->initChai();

	fps = 0;
	tps = 0;
	missedTime = sf::seconds(0.f);
	isInFocus = true;
	worldObjectsFrozen = false;
	conservativeMouse = true;
	inMainUpdateLoop = false;
	doRestart = false;
	colBoxes = MAGE_SHOWCOLLISIONBOXES;
	showFps = MAGE_SHOWFPS;
	tickScale = 1.f;
	state = states->current; // this never changes at the moment so only needs to be set once

	mouseMode = 0; // 0 is no mouse. Other numbers represent various sprite settings for the cursor.

	p::info("Loading settings");

	loadAudioSettings();

	if (window) {
		windowInit(window);
	}

	p::info("Loading keybinds");
	setKeybinds();

	//p::info("Creating prefabs");
	//createPrefabs();

	p::info("Loading resources");
	loadResources();

	onGameInitDone.notify();

	tickClock.restart();
	updateTime.restart();

	p::log("\nDONE!", true);
}

bool Game::renderWindowExists()
{
	return window;
}

void Game::windowInit(sf::RenderWindow * wind)
{
	window = wind;

	loadWindowSettings();
}

// STARTUP FUNCTIONS (modular design and all that)
// -------------------------------------------------------------------------------------

void Game::setKeybinds() {
	// ADD KEYBINDS HERE
	keyboard->setBind("Screenshot", { sf::Keyboard::LControl, sf::Keyboard::P }, true);
	keyboard->setBind("Grid", { sf::Keyboard::LControl, sf::Keyboard::G }, true);
	keyboard->setBind("Save", { sf::Keyboard::LControl, sf::Keyboard::S }, true);

	keyboard->setBind("Up", sf::Keyboard::W, true);
	keyboard->setBind("Down", sf::Keyboard::S, true);
	keyboard->setBind("Left", sf::Keyboard::A, true);
	keyboard->setBind("Right", sf::Keyboard::D, true);

	keyboard->setBind("Quick Cast", sf::Keyboard::Q, true);
	keyboard->setBind("Interact", sf::Keyboard::E, true);
	keyboard->setBind("Select", sf::Keyboard::Space, true);

	keyboard->setBind("Devmode", sf::Keyboard::F1, true);

	keyboard->setDefaultBindTextures();
	mouse->setDefaultButtonTextures();
}

void Game::loadResources() {
	// registry
	p::info("Creating variable registry...");

	// sound effects used by engine objects
	resources->add("sfx_begin", std::make_shared<resourceSoundBuffer>("base/sfx/enterBattle.wav"), true);
	resources->add("sfx_explosion", std::make_shared<resourceSoundBuffer>("base/sfx/Explosion2.wav"), true);
	resources->add("sfx_dlgDefault", std::make_shared<resourceSoundBuffer>("base/sfx/talk_info.wav"), true);
	resources->add("sfx_damage", std::make_shared<resourceSoundBuffer>("base/sfx/damage.wav"), true);
	resources->add("sfx_menuopen", std::make_shared<resourceSoundBuffer>("base/sfx/menuopen.wav"), true);
	resources->add("sfx_menuclose", std::make_shared<resourceSoundBuffer>("base/sfx/menuclose.wav"), true);
	resources->add("sfx_menuselect", std::make_shared<resourceSoundBuffer>("base/sfx/menuselect.wav"), true);
	resources->add("sfx_menuchoose", std::make_shared<resourceSoundBuffer>("base/sfx/menuchoose.wav"), true);
	resources->add("sfx_menuback", std::make_shared<resourceSoundBuffer>("base/sfx/menuback.wav"), true);

	// undeferred
	resources->add("__fontDef", std::make_shared<resourceFont>("base/ui/text/PIXEARG_.TTF"), true);
	resources->add("__splash", std::make_shared<resourceTexture>("base/ui/splash.png"), true);

	// generic engine stuff
	resources->add("mouse", std::make_shared<resourceTexture>("base/ui/mouse.png"));

	// ui required by legacy classes
	resources->add("ui_title", std::make_shared<resourceTexture>("base/ui/title2.png"));
	resources->add("ui_title_small", std::make_shared<resourceTexture>("base/ui/title.png"));
	resources->add("ui_info_image", std::make_shared<resourceTexture>("base/ui/infoimage.png"));

	resources->add("ui_block_base", std::make_shared<resourceTexture>("base/ui/block/base.png"));

	resources->add("ui_notfound", std::make_shared<resourceTexture>("base/ui/nosprite.png"));

	// lazyload ui
	resources->addFolder<resourceTexture>("ui_input_", "base/ui/bindimages");

	// shaders
	resources->add("interactiveObject", std::make_shared<resourceShader>("base/shaders/outline.glsl", sf::Shader::Fragment));
	resources->add("desaturation", std::make_shared<resourceShader>("base/shaders/grayscale.glsl", sf::Shader::Fragment));

	// MAIN SCRIPT
	resources->add("main", std::make_shared<resourceScriptRoot>("resources/main.chai"), true);

	resources->update();
}

// PROCESS FUNCTIONS
// -------------------------------------------------------------------------------------

void Game::draw() {
	// GAME VIEW
	camera->render(getRenderWindow(), state->bgCol); // temp
	onGameDraw.notify();

	getRenderWindow().display();
}

void Game::update(interval elapsed) {
	p::update();

	music->update();
	sound->update();
	scripting->update();

	// check for console commands
	std::string in;
	if (p::getConsoleInput(in)) {
		if (in != "") {
			try {
				scripting->eval(in);
			}
			catch (chaiscript::exception::eval_error &e) {
				handleEvalError(e);
			}
			catch (std::runtime_error &e) {
				p::warn(e.what());
			}
			catch (...) {
				p::warn("Unknown error. Please never do that again.");
			}
		}
	}

	if (keyboard->getBind("Screenshot")->getPressed()) {
		sf::Texture windowTex;
		windowTex.update(*window);

		sf::Image img = windowTex.copyToImage();

		p::info("Saving as \"screenshots/shot" + std::to_string(time(0)) + ".png\"");
		if (!img.saveToFile("screenshots/shot" + std::to_string(time(0)) + ".png")) {
			p::info("Saving failed!");
		}
		else {
			p::info("Success!");
		}
	}

	// process game inputs too
	//if (keyboard.getBind("ESCAPE")->getPressed())
	//	window.close();

	inMainUpdateLoop = true;

	// everything else is passed down to objects
	state->preUpdateObjects(elapsed);
	onGamePreUpdate.notify();

	state->updateObjects(elapsed);
	onGameUpdate.notify();

	inMainUpdateLoop = false;

	// io
	mouse->update();
	keyboard->update();

	// finish
	debug = true;

	mouseTexture = resources->getAs<resourceTexture>("mouse");
}

int Game::run(bool tickByTick) {
	bool stopLooping = false;
	sf::Clock frameTimer;

	try {
		while (getRenderWindow().isOpen() && !stopLooping)
		{
			if (doRestart) {
				p::warn("RESTARTING!");
				init();
			}

			resources->update();
			frameTimer.restart();

			draw();

			missedTime += tickClock.restart();

			if (tickScale <= 0) {
				p::warn("NO.");
				tickScale = 1.f;
			}

			interval magicMs = sf::milliseconds(MAGE_TICKMS * tickScale); // legacy support means you can initialize interval with sf::Time

			while (missedTime >= magicMs) {
				if (missedTime >= magicMs * 10.f) {
					interval mt = missedTime / magicMs;

					//p::warn("Can't keep up! " + std::to_string(missedTime.asMilliseconds()) + "ms behind, skipping " + std::to_string(mt) + " ticks");
					missedTime = magicMs * 10.f;
				}

				tps = floor(1.f / updateTime.getElapsedTime().get());

				missedTime -= magicMs;
				update(updateTime.restart());

				sf::Event evt;
				while (getRenderWindow().pollEvent(evt)) {
					if (isInFocus) {
						mouse->processEvent(evt);
						keyboard->processEvent(evt);
					}

					if (evt.type == sf::Event::LostFocus) {
						isInFocus = false;
						getRenderWindow().setMouseCursorVisible(true);
					}

					if (evt.type == sf::Event::GainedFocus) {
						isInFocus = true;
						getRenderWindow().setMouseCursorVisible(false);
					}

					if (evt.type == sf::Event::Closed) {
						getRenderWindow().close();
					}
				}
			}

			fps = floor(1.f / frameTimer.getElapsedTime().asSeconds());

			if (tickByTick) {
				stopLooping = true;
			}
		}
	}
	catch (std::runtime_error &e) {
		p::fatal("Fatal runtime error: " + std::string(e.what()));
	}

	return 0;
}

void Game::loadWindowSettings(point2U resolution, std::string title, bool fullscreen, bool vsync, int maxFps) {
	windowSettings.init("config/window.cfg");
	windowSettings.header = "Generally self-explanatory window settings file.";

	windowSettings.add(registry::prop("resolution", std::to_string(resolution.x) + "x" + std::to_string(resolution.y)));
	windowSettings.add(registry::prop("fullscreen", fullscreen));
	windowSettings.add(registry::prop("vsync", vsync));
	windowSettings.add(registry::prop("maxFps", maxFps));

	windowSettings.load();

	auto vm = splitString(windowSettings.get("resolution")->as<std::string>(), 'x');

	setWindowSettings(point2U(atof(vm[0].c_str()), atof(vm[1].c_str())),
		title,
		windowSettings.get("fullscreen")->as<bool>(),
		windowSettings.get("vsync")->as<bool>(),
		windowSettings.get("maxFps")->as<float>());
}

void Game::setWindowSettings(point2U resolution, std::string title, bool fullscreen, bool vsync, int maxFps) {
	if(fullscreen != isFullscreen() || resolution.toSf2() != getRenderWindow().getSize())
	getRenderWindow().create(sf::VideoMode(resolution.x, resolution.y), title, fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar);

	if (vsync != isVsyncEnabled())
		getRenderWindow().setVerticalSyncEnabled(vsync);

	if (maxFps != getFramerateLimit())
		getRenderWindow().setFramerateLimit(maxFps);

	if (vsync)
		getRenderWindow().setFramerateLimit(0);

	getRenderWindow().setTitle(title);
	windowTitle = title;

	if (!getRenderWindow().isOpen()) {
		p::fatal("Failed to apply settings to the window.\n\nIf you now get this every time you start the game, delete window.cfg in the config folder.\nIf this is your first attempt starting the game, refer to online help.");
	}

	p::info("Window settings:");
	p::log("\tresolution: " + std::to_string(resolution.x) + "x" + std::to_string(resolution.y));
	p::log("\ttitle: " + title);
	p::log("\tfullscreen: " + std::string(fullscreen ? "YES" : "NO"));
	p::log("\tvsync: " + std::string(vsync ? "YES" : "NO"));
	p::log("\tfpslimit: " + std::to_string(maxFps) + "fps" + (maxFps <= 30 && maxFps != 0 ? " cinematic experience" : ""));

	getRenderWindow().setMouseCursorVisible(false);

	// fix views
	fixViews();

	// save settings
	p::info("Saving window config.");

	windowSettings.get("resolution")->update(std::to_string(getRenderWindow().getSize().x) + "x" + std::to_string(getRenderWindow().getSize().y));
	windowSettings.get("fullscreen")->update(fullscreen);
	windowSettings.get("vsync")->update(vsync);
	windowSettings.get("maxFps")->update(maxFps);
}

void Game::loadAudioSettings(float masterVolume, float sfxVolume, float musicVolume)
{
	audioSettings = registry();
	audioSettings.init("config/audio.cfg");
	audioSettings.header = "Self-explanitory audio settings file.";

	audioSettings.add(registry::prop("masterVolume", masterVolume));
	audioSettings.add(registry::prop("sfxVolume", sfxVolume));
	audioSettings.add(registry::prop("musicVolume", musicVolume));

	audioSettings.load(); // loads existing settings over the ones we set above if there are any

	setAudioSettings(audioSettings.get("masterVolume")->as<float>(),
		audioSettings.get("sfxVolume")->as<float>(),
		audioSettings.get("musicVolume")->as<float>()
	);
}

void Game::setAudioSettings(float masterVolume, float sfxVolume, float musicVolume)
{
	music->volume = musicVolume;
	sound->volume = sfxVolume;
	sf::Listener::setGlobalVolume(masterVolume);

	p::info("Audio settings:");
	p::log("\tmaster volume: " + std::to_string(masterVolume));
	p::log("\tsfx volume: " + std::to_string(sfxVolume));
	p::log("\tmusic volume: " + std::to_string(musicVolume));

	// save settings
	audioSettings.get("masterVolume")->update(masterVolume);
	audioSettings.get("sfxVolume")->update(sfxVolume);
	audioSettings.get("musicVolume")->update(musicVolume);
}

void Game::fixViews()
{
	auto wSize = sf::Vector2f(getRenderWindow().getSize().x, getRenderWindow().getSize().y);

	//p::info(std::to_string(wSize.x));

	camera->setBaseSize(pointF(wSize));
	camera->setPosition(sf::Vector2f(0, 0));
	camera->viewScene = state;
}

float Game::getDelta()
{
	return tickClock.getElapsedTime().get(interval::MILLISECONDS) / MAGE_TICKMS;
}

std::string Game::version()
{
	return MAGE_BUILDSTRING;
}

// HELPER FUNCTIONS
// -------------------------------------------------------------------------------------

point2U Game::getScreenResolution()
{
	// resolution (the hard one)
	// it's stored as a string consisting of 2 numbers with an x between
	auto resString = windowSettings.get("resolution")->read();
	auto resStringSplit = splitString(resString, 'x', ' ', 2);

	point2U wSize(atoi(resStringSplit[0].c_str()), atoi(resStringSplit[1].c_str()));

	return wSize;
}

std::string Game::getWindowTitle()
{
	// for some reason sfml can't get this.
	// luckily we have it stored.
	return windowTitle;
}

bool Game::isFullscreen()
{
	return windowSettings.get("fullscreen")->as<bool>();
}

bool Game::isVsyncEnabled()
{
	return windowSettings.get("vsync")->as<bool>();
}

int Game::getFramerateLimit() {
	return windowSettings.get("maxFps")->as<int>();
}

void Game::setScreenResolution(point2U res)
{
	setWindowSettings(res, getWindowTitle(), isFullscreen(), isVsyncEnabled(), getFramerateLimit());
}

void Game::setFullscreen(bool tf)
{
	setWindowSettings(getScreenResolution(), getWindowTitle(), tf, isVsyncEnabled(), getFramerateLimit());
}

void Game::setVsyncEnabled(bool tf)
{
	setWindowSettings(getScreenResolution(), getWindowTitle(), isFullscreen(), tf, getFramerateLimit());
}

void Game::setFramerateLimit(int fpsl) 
{
	setWindowSettings(getScreenResolution(), getWindowTitle(), isFullscreen(), isVsyncEnabled(), fpsl);
}

void Game::setWindowTitle(std::string t)
{
	setWindowSettings(getScreenResolution(), t, isFullscreen(), isVsyncEnabled(), getFramerateLimit());
}

interval Game::getSimTime()
{
	return magicClock.getElapsedTime();
}

sf::RenderWindow& Game::getRenderWindow()
{
	if (window == nullptr) {
		p::fatal("A rendering window is required to continue execution.");
	}

	return *window;
}

Game * mage::theGame()
{
	return globalGame;
}
