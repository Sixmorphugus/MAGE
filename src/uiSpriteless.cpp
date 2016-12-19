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

DeclareScriptingBasic(uiSpriteless);
DeclareScriptingBaseClass(uiBasic, uiSpriteless);
DeclareScriptingConstructor(uiSpriteless(float, float), "uiSpriteless");
DeclareScriptingCopyOperator(uiSpriteless);