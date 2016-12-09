#include "inputs.h"
#include "platform.h"

// SCRIPTING ENGINE BINDING
// ---------------------------------------------------
#include "scriptingEngine.h"
using namespace mage;

DeclareScriptingType(input);
DeclareScriptingFunction(&input::processEvent, "processEvent");
DeclareScriptingFunction(&input::update, "update");