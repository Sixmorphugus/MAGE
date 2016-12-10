#include "hook.h"

#include "scriptingEngine.h"

// lol this is just an se bind
using namespace mage;
using namespace chaiscript;

DeclareScriptingType(hookBase);
DeclareScriptingFunction(&hookBase::clearObservers, "clearObservers");
DeclareScriptingFunction(&hookBase::enabled, "enabled");
DeclareScriptingFunction(&hookBase::forgetObserver, "forgetObserver");
DeclareScriptingFunction(&hookBase::numObservers, "getNumObservers");
