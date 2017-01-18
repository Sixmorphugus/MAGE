#include "resourceFilepath.h"

using namespace mage;

// FILEPATH RESOURCE
resourceFilepath::resourceFilepath()
{}

resourceFilepath::resourceFilepath(std::string fp)
{
	m_filepath = p::realPath(fp);
}

bool resourceFilepath::load()
{
	p::log("\tPath: " + m_filepath);
	return resource::load();
}

void resourceFilepath::setFilepath(std::string filepath)
{
	bool rld = false;

	if (isLoaded()) {
		unload();
		rld = true;
	}

	m_filepath = filepath;

	if (rld)
		if (load())
			p::warn("filepathResource hot reload failed.");
}

std::string mage::resourceFilepath::getFilepath()
{
	return m_filepath;
}

// filepathResource
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(resourceFilepath);
MAGE_DeclareScriptingBaseClass(taggable, resourceFilepath);
MAGE_DeclareScriptingBaseClass(resource, resourceFilepath);
MAGE_DeclareScriptingConstructor(resourceFilepath(), "filepathResource");
MAGE_DeclareScriptingConstructor(resourceFilepath(std::string), "filepathResource");
MAGE_DeclareScriptingFunction(&resourceFilepath::getFilepath, "getFilepath");
MAGE_DeclareScriptingFunction(&resourceFilepath::setFilepath, "setFilepath");
MAGE_DeclareScriptingCopyOperator(resourceFilepath);

MAGE_DeclareScriptingFunction(&resourceMngr::getAs<resourceFilepath>, "getFilepathResource");
MAGE_DeclareScriptingFunction(&resourceMngr::listType<resourceFilepath>, "listFilepathResources");
MAGE_DeclareScriptingFunction(&resourceMngr::addFolder<resourceFilepath>, "addFolderOfFilepathResources");