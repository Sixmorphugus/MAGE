#pragma once
#include "resourceFilepath.h"
#include "group.h"

namespace mage {

// group resource is simply a saved group object.
// a group is a load of objects.
class MAGEDLL resourceGroup : public resourceFilepath
{
public:
	resourceGroup();
	resourceGroup(std::string filepath);

	bool load();
	void unload();

	bool isNeeded();

	std::shared_ptr<Group> get();

private:
	std::shared_ptr<Group> rc;
};

} //namespace mage