#include "objSpriteless.h"

using namespace mage;

objSpriteless::objSpriteless(float x, float y)
	: objBasic(x, y)
{
	// ya
}

DeclareScriptingBasic(objSpriteless);
DeclareScriptingBaseClass(objBasic, objSpriteless);
DeclareScriptingConstructor(objSpriteless(float, float), "objSpriteless");
DeclareScriptingCopyOperator(objSpriteless);