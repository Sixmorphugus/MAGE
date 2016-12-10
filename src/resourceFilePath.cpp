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

DeclareScriptingType(resourceFilepath);
DeclareScriptingBaseClass(taggable, resourceFilepath);
DeclareScriptingBaseClass(resource, resourceFilepath);
DeclareScriptingConstructor(resourceFilepath(), "filepathResource");
DeclareScriptingConstructor(resourceFilepath(std::string), "filepathResource");
DeclareScriptingFunction(&resourceFilepath::filepath, "filepath");
DeclareScriptingCopyOperator(resourceFilepath);

DeclareScriptingFunction(&resourceMngr::getAs<resourceFilepath>, "getFilepathResource");
DeclareScriptingFunction(&resourceMngr::listType<resourceFilepath>, "listFilepathResources");
DeclareScriptingFunction(&resourceMngr::addFolder<resourceFilepath>, "addFolderOfFilepathResources");