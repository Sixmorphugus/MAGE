#include "uiSpriteless.h"

using namespace mage;

uiSpriteless::uiSpriteless(float x, float y)
	: uiBasic(x, y)
{
	// ya
}

sf::Vector2f uiSpriteless::getSize()
{
	return sf::Vector2f(0.f, 0.f);
}

MAGE_DeclareScriptingBasic(uiSpriteless);
MAGE_DeclareScriptingBaseClass(uiBasic, uiSpriteless);
MAGE_DeclareScriptingConstructor(uiSpriteless(float, float), "uiSpriteless");
MAGE_DeclareScriptingCopyOperator(uiSpriteless);