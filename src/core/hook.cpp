#include "hook.h"

#include "scriptingEngine.h"

using namespace mage;

void hookBase::hEE(const chaiscript::exception::eval_error & e) const
{
	handleEvalError(e);
}

// lol this is just an se bind
using namespace mage;
using namespace chaiscript;

MAGE_DeclareScriptingUnlistableType(hookBase);
MAGE_DeclareScriptingFunction(&hookBase::clearObservers, "clearObservers");
MAGE_DeclareScriptingFunction(&hookBase::enabled, "enabled");
MAGE_DeclareScriptingFunction(&hookBase::forgetObserver, "forgetObserver");
MAGE_DeclareScriptingFunction(&hookBase::numObservers, "getNumObservers");