#include "jukebox.h"
#include "Game.h"
#include "helpers.h"
#include "platform.h"
#include "maths.h"

#include "resourceFilePath.h"

using namespace mage;

jukebox::jukebox()
{
	volume = 100.f;
	pitchShiftPitch = 1.f;
	pitchShift = 1.f;
	pitchShiftMultiplier = 0.1f;

	playing = false;
}

void jukebox::playSong(std::shared_ptr<resourceFilepath> song)
{
	if (!song) {
		stopPlaying();
		return;
	}

	// check the resource is nontype
	p::info("playing " + theGame()->resources->nameOf(song));

	stopPlaying();

	lp = song;

	player.openFromFile(song->getFilepath());
	altPlayer.setPlayingOffset(sf::seconds(0.f));

	usingAltPlayer = false;

	player.setVolume(volume);

	// play
	resume();
}

void jukebox::stopPlaying(bool fadeOut)
{
	lp.reset();

	if (fadeOut) {
		if (usingAltPlayer) {
			player.stop();
		}
		else {
			altPlayer.stop();
		}

		usingAltPlayer = !usingAltPlayer; // fade to a player that is no longer playing
	}
	else {
		usingAltPlayer = false;
		playing = false;

		player.stop();
		altPlayer.stop();
	}
}

void jukebox::pause()
{
	player.pause();
	altPlayer.pause();

	playing = false;
}

void jukebox::resume()
{
	player.play();
	altPlayer.play();

	playing = true;
}

void jukebox::fadeToSong(std::shared_ptr<resourceFilepath> song)
{
	if (!song) {
		stopPlaying(true);
		return;
	}

	p::info("fading to " + theGame()->resources->nameOf(song));

	if (lp.lock() == song) {
		p::info("already playing!");
		return;
	}

	lp = song;

	if (usingAltPlayer) {
		player.openFromFile(song->getFilepath());

		altPlayer.setVolume(volume);
		player.setVolume(0.f);
		player.play();
	}
	else {
		altPlayer.openFromFile(song->getFilepath());

		player.setVolume(volume);
		altPlayer.setVolume(0.f);
		altPlayer.play();
	}

	usingAltPlayer = !usingAltPlayer;
}

void jukebox::update()
{
	pitchShift = clamp(pitchShift, 0, 100);
	pitchShiftMultiplier = clamp(pitchShiftMultiplier, 0, 1);

	pitchShiftPitch += ((pitchShift - pitchShiftPitch) * pitchShiftMultiplier);

	// fade between the two players based on the boolean.
	float firstV = player.getVolume();

	float secondV = altPlayer.getVolume();

	float maxV = volume;
	float incV = 1.f;

	if (usingAltPlayer) {
		altPlayer.setPitch(pitchShiftPitch / theGame()->tickScale);

		//p::info("equalizing second player");

		// decrease the first volume if it's above 0
		if (firstV > 0.f) {
			float nV = firstV - incV < 0.f ? 0.f : firstV - incV;
			player.setVolume(nV);
		}

		// increase the second volume if it's below maxV
		if (secondV < maxV) {
			float nV = secondV + incV > maxV ? maxV : secondV + incV;
			altPlayer.setVolume(nV);
		}
		else {
			// set second volume to maxV
			altPlayer.setVolume(maxV);
		}
	}
	else {
		player.setPitch(pitchShiftPitch / theGame()->tickScale);

		//p::info("equalizing first player");

		if (secondV > 0.f) {
			float nV = secondV - incV < 0.f ? 0.f : secondV - incV;
			altPlayer.setVolume(nV);
		}

		if (firstV < maxV) {
			float nV = firstV + incV > maxV ? maxV : firstV + incV;
			player.setVolume(nV);
		}
		else {
			// set second volume to maxV
			player.setVolume(maxV);
		}
	}
}

// SE
using namespace chaiscript;

#include "scriptingEngine.h"

MAGE_DeclareScriptingCustom(user_type<jukebox>(), "jukebox");
MAGE_DeclareScriptingCustom(constructor<jukebox()>(), "jukebox");
MAGE_DeclareScriptingCustom(fun(&jukebox::fadeToSong), "fadeToSong");
MAGE_DeclareScriptingCustom(fun(&jukebox::getLastPlayed), "getLastPlayed");
MAGE_DeclareScriptingCustom(fun(&jukebox::pause), "pause");
MAGE_DeclareScriptingCustom(fun(&jukebox::pitchShift), "pitchShift");
MAGE_DeclareScriptingCustom(fun(&jukebox::pitchShiftMultiplier), "pitchShiftMultiplier");
MAGE_DeclareScriptingCustom(fun(&jukebox::playSong), "playSong");
MAGE_DeclareScriptingCustom(fun(&jukebox::resume), "resume");
MAGE_DeclareScriptingCustom(fun(&jukebox::stopPlaying), "stopPlaying");
MAGE_DeclareScriptingCustom(fun([](jukebox& j) { j.stopPlaying(); }), "stopPlaying");
MAGE_DeclareScriptingCustom(fun(&jukebox::update), "update");
MAGE_DeclareScriptingCustom(fun(&jukebox::volume), "volume");