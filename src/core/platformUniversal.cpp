#include "platform.h"
#include "Game.h"

using namespace mage;

std::string lastLogged;

std::string platform::getLastLogged() {
	return lastLogged;
}

MAGEDLL void platform::loadTextFile(std::string & fileData, const std::string& fileName)
{
	std::ifstream is(fileName);

	std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
	is.close();

	fileData = str;
}

MAGEDLL void platform::saveTextFile(const std::string & fileData, const std::string & fileName)
{
	std::ofstream os(fileName);

	os << fileData;
	os.close();
}

void platform::log(std::string info, bool nl)
{
	// Message
	writeToLogfile(info);
	writeToConsole(info, 3);

	// Line terminator
	if (nl) {
		writeToLogfile("\n");
		writeToConsole("\n", 3);
	}
}

void platform::diagnosticLog(LOGTYPE type, std::string file, std::string func, int line, std::string info, bool nl)
{
	std::string typewords[] = { "INFO", "WARNING", "FATAL", "CHAISCRIPT" };

	// Level and source code information
	#if MAGE_PLATFORM_LOGINFOALLOWED
	writeToLogfile(typewords[type] + "[" + file + ":" + std::to_string(line) + " (" + func + ")]: " + info + (nl ? "\n" : ""));
	#else
	writeToLogfile(type + "[unavailable]: " + info + (nl ? "\n" : ""));
	#endif

	lastLogged = info;

	writeToConsole(info + (nl ? "\n" : ""), type);
}

void platform::diagnosticError(std::string file, std::string func, int line, std::string info) {
	platform::diagnosticLog(FATAL, file, func, line, info, true);

	// show a message to the user
	platform::showMessageBox("Alchemist has crashed!", info + " (" + func + ")", true);

	// make sure not to forget the console info
	platform::deInit();

	// crash
	abort();
}

#ifdef MAGEDLL_EXPORT
MAGE_DeclareGame(Game); // base engine main function.
#endif