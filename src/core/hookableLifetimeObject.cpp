#include "hookableLifetimeObject.h"

using namespace mage;

hookableLifetimeObject::hookableLifetimeObject()
{
	onCreated.notify(this);
}

hookableLifetimeObject::~hookableLifetimeObject()
{
	onDestroyed.notify(this);
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingHook("hookableLifetimeObjectHook", hookableLifetimeObject*);
MAGE_DeclareScriptingType(hookableLifetimeObject);
MAGE_DeclareScriptingConstructor(hookableLifetimeObject(), "hookableLifetimeObject");
MAGE_DeclareScriptingCopyOperator(hookableLifetimeObject);
MAGE_DeclareScriptingFunction(&hookableLifetimeObject::onCreated, "onCreated");
MAGE_DeclareScriptingFunction(&hookableLifetimeObject::onDestroyed, "onDestroyed");