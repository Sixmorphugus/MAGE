#include "resourceFilepath.h"

using namespace mage;

// FILEPATH RESOURCE
resourceFilepath::resourceFilepath()
{}

resourceFilepath::resourceFilepath(std::string fp)
{
	filepath = p::realPath(fp);
}

bool resourceFilepath::load()
{
	p::log("\tPath: " + filepath);
	return resource::load();
}

// filepathResource
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(resourceFilepath);
MAGE_DeclareScriptingBaseClass(taggable, resourceFilepath);
MAGE_DeclareScriptingBaseClass(resource, resourceFilepath);
MAGE_DeclareScriptingConstructor(resourceFilepath(), "filepathResource");
MAGE_DeclareScriptingConstructor(resourceFilepath(std::string), "filepathResource");
MAGE_DeclareScriptingFunction(&resourceFilepath::filepath, "filepath");
MAGE_DeclareScriptingCopyOperator(resourceFilepath);

MAGE_DeclareScriptingFunction(&resourceMngr::getAs<resourceFilepath>, "getFilepathResource");
MAGE_DeclareScriptingFunction(&resourceMngr::listType<resourceFilepath>, "listFilepathResources");
MAGE_DeclareScriptingFunction(&resourceMngr::addFolder<resourceFilepath>, "addFolderOfFilepathResources");