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