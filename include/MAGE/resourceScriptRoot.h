#pragma once
#include "resourceFilepath.h"
#include "scriptingEngine.h"

namespace mage {

// Scripts are a resource for the following reasons:
// - execution as part of resource loading
// - scripts themselves load resources
// - scripts are loaded from external files
// - scripts are cool

// In the chai engine, use(script) calls the scripting engine's function to do the same thing
// doFile will still load a script anonymously without adding a resource

class MAGEDLL resourceScriptRoot : public resourceFilepath
{
public:
	resourceScriptRoot();
	resourceScriptRoot(std::string filepath);

	bool load();
	Boxed_Value run();
};

} // namespace mage 