#include "inputs.h"
#include "platform.h"

// SCRIPTING ENGINE BINDING
// ---------------------------------------------------
#include "scriptingEngine.h"
using namespace mage;

MAGE_DeclareScriptingType(input);
MAGE_DeclareScriptingFunction(&input::processEvent, "processEvent");
MAGE_DeclareScriptingFunction(&input::update, "update");