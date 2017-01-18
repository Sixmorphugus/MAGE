#include "inputs.h"
#include "platform.h"

// SCRIPTING ENGINE BINDING
// ---------------------------------------------------
#include "scriptingEngine.h"
using namespace mage;

MAGE_DeclareScriptingUnlistableType(input);
MAGE_DeclareScriptingFunction(&input::processEvent, "processEvent");
MAGE_DeclareScriptingFunction(&input::update, "update");