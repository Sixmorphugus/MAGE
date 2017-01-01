#include "namable.h"

using namespace mage;

namable::namable(std::string name)
{
	m_name = name;
}

std::string namable::getName()
{
	return m_name;
}

renamable::renamable(std::string name)
	: namable(name)
{
}

void renamable::setName(std::string name)
{
	m_name = name;
	onRenamed.notify(this);
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingType(namable);
MAGE_DeclareScriptingConstructor(namable(std::string n), "namable");
MAGE_DeclareScriptingFunction([]() { return namable(); }, "namable");
MAGE_DeclareScriptingFunction(&namable::getName, "getName");

MAGE_DeclareScriptingType(renamable);
MAGE_DeclareScriptingBaseClass(namable, renamable);
MAGE_DeclareScriptingFunction(&renamable::setName, "setName");
MAGE_DeclareScriptingFunction(&renamable::onRenamed, "onRenamed");

MAGE_DeclareScriptingHook("renamableHook", renamable*);