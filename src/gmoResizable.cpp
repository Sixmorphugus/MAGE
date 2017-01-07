#include "gmoResizable.h"

using namespace mage;

gmoResizable::gmoResizable(pointF position, pointF size)
	: gmo(position), transformableBox(position, size)
{
}

// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(gmoResizable);
MAGE_DeclareScriptingBaseClass(gmo, gmoResizable);
MAGE_DeclareScriptingBaseClass(gmo, transformableBox);
MAGE_DeclareScriptingConstructor(gmoResizable(pointF, pointF), "gmoResizable");