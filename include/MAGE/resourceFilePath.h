#pragma once
#include "resources.h"

namespace mage {

// basic class for a resource using ONE file.
class MAGEDLL resourceFilepath : public resource
{
public:
	resourceFilepath();
	resourceFilepath(std::string filepath);

	virtual bool load();
public:
	std::string filepath;
};

}

#define MAGE_DeclareScriptingFilepathResource(name) \
MAGE_DeclareScriptingResource(name) \
MAGE_DeclareScriptingBaseClass(resourceFilepath, name);\
MAGE_DeclareScriptingConstructor(name(std::string), STRING(name));\
MAGE_DeclareScriptingFunction(&resourceMngr::addFolder<name>, "addFolderOf_" STRING(name));\
MAGE_DeclareScriptingCopyOperator(name);
