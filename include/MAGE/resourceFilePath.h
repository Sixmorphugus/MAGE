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

#define DeclareScriptingFilepathResource(name) \
DeclareScriptingResource(name) \
DeclareScriptingBaseClass(resourceFilepath, name);\
DeclareScriptingConstructor(name(std::string), STRING(name));\
DeclareScriptingFunction(&resourceMngr::addFolder<name>, "addFolderOf_" STRING(name));\
DeclareScriptingCopyOperator(name);