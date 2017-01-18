#include "gmoResizable.h"

using namespace mage;

gmoResizable::gmoResizable()
{
}

gmoResizable::gmoResizable(pointF position, pointF size)
	: gmo(position), transformableBox(position, size)
{
}

// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingGmoType(gmoResizable);
MAGE_DeclareScriptingBaseClass(gmo, gmoResizable);
MAGE_DeclareScriptingBaseClass(transformableBox, gmoResizable);
MAGE_DeclareScriptingConstructor(gmoResizable(pointF, pointF), "gmoResizable");