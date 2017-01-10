#pragma once

// mage::renderable
// -------------
// An object that can generate a drawRecipe and therefore be passed to the renderer.
// Also has a property (visible) which can make the renderer ignore it.
// -------------
// Category: Rendering
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "renderRecipe.h"
#include "shaders.h"
#include "hook.h"

namespace mage {

class renderable {
public:
	renderable();

	renderRecipe getDrawRecipe();

	bool getIsVisible() const;
	void setIsVisible(bool visible);

public:
	hook<renderable*> onRendered;

protected:
	virtual renderRecipe generateDrawRecipe();

private:
	bool m_visible;
};

}