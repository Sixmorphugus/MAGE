#include "objSpriteless.h"

using namespace mage;

objSpriteless::objSpriteless(float x, float y)
	: objBasic(x, y)
{
	// ya
}

sf::Vector2f objSpriteless::getSize()
{
	return sf::Vector2f(0.f, 0.f);
}

MAGE_DeclareScriptingBasic(objSpriteless);
MAGE_DeclareScriptingBaseClass(objBasic, objSpriteless);
MAGE_DeclareScriptingConstructor(objSpriteless(float, float), "objSpriteless");
MAGE_DeclareScriptingCopyOperator(objSpriteless);