#include "serializable.h"
#include "platform.h"
#include "stringHelpers.h"

using namespace mage;

mage::serializable::serializable()
{
	// ...
}

std::string serializable::serialize()
{
	return "";
}

bool serializable::deserialize(std::string data)
{
	return true;
}

void serializable::saveToFile(std::string file)
{
	sf::Clock timer;
	std::ofstream saveFile(file);

	if (saveFile.bad()) {
		p::warn("Unable to save serializable object to file - Bad file location.");
		return;
	}

	saveFile << serialize();

	saveFile.close();

	p::info("Saved in " + std::to_string(timer.getElapsedTime().asMilliseconds()));
}

bool serializable::loadFromFile(std::string file)
{
	sf::Clock timer;

	std::ifstream saveFile(file);

	if (!saveFile.good()) {
		p::info("FAILED - bad file");
		return false;
	}

	std::string str;

	saveFile.seekg(0, std::ios::end);
	str.reserve(saveFile.tellg());
	saveFile.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(saveFile)),
		std::istreambuf_iterator<char>());

	str = removeWhitespace(str);

	bool result = deserialize(str);

	if(result)
		p::info("Loaded in " + std::to_string(timer.getElapsedTime().asMilliseconds()));

	return result;
}

std::string serializable::saveToString(std::string file)
{
	return serialize();
}

bool mage::serializable::loadFromString(std::string file)
{
	file = removeWhitespace(file);
	return deserialize(file);
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingType(serializable)
MAGE_DeclareScriptingFunction(&serializable::deserialize, "deserialize");
MAGE_DeclareScriptingFunction(&serializable::serialize, "serialize");
MAGE_DeclareScriptingFunction(&serializable::saveToFile, "saveToFile");
MAGE_DeclareScriptingFunction(&serializable::loadFromFile, "loadFromFile");
MAGE_DeclareScriptingFunction(&serializable::saveToFile, "saveToString");
MAGE_DeclareScriptingFunction(&serializable::loadFromFile, "loadFromString");