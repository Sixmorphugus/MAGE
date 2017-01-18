#pragma once

// mage::gmoResizable
// -------------
// An object that can be resized. Basically a gmo that derives transformableBox rather than transformableObject
// -------------
// Category: Maths
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "gmo.h"
#include "transformableBox.h"

namespace mage {

class gmoResizable :
public transformableBox, public gmo
{
public:
	gmoResizable();
	gmoResizable(pointF position, pointF size = pointF());
	MAGE_GmoOverloads(gmoResizable);
};

}
