#include "resourceScriptRoot.h"
#include "Game.h"

using namespace mage;

// script
resourceScriptRoot::resourceScriptRoot()
{}

resourceScriptRoot::resourceScriptRoot(std::string filepath)
	: resourceFilepath(filepath)
{}

bool resourceScriptRoot::load()
{
	run();

	return resourceFilepath::load();
}

chaiscript::Boxed_Value resourceScriptRoot::run()
{
	return theGame()->scripting->evalFile(filepath);
}

// msr
DeclareScriptingFilepathResource(resourceScriptRoot);
DeclareScriptingFunction(&resourceScriptRoot::run, "run");