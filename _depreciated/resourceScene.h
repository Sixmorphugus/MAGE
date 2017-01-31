#pragma once
#include "resourceFilepath.h"
#include "scene.h"

namespace mage {

// group resource is simply a saved group object.
// a group is a load of objects.
class MAGEDLL resourceScene : public resourceFilepath, public scene
{
public:
	resourceScene();
	resourceScene(std::string filepath);

	bool load();
	void unload();

	bool isNeeded();
};

} //namespace mage