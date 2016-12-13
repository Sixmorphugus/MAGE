#include "uiSpriteless.h"

using namespace mage;

uiSpriteless::uiSpriteless(float x, float y)
	: uiBasic(x, y)
{
	// ya
}

DeclareScriptingBasic(uiSpriteless);
DeclareScriptingBaseClass(uiBasic, uiSpriteless);
DeclareScriptingConstructor(uiSpriteless(float, float), "uiSpriteless");
DeclareScriptingCopyOperator(uiSpriteless);