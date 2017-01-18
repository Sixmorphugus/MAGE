#include "taggable.h"

using namespace mage;

taggable::taggable()
{
}

bool taggable::hasTag(std::string tag)
{
	for (unsigned int i = 0; i < tags.size(); i++) {
		if (tags[i] == tag)
			return true;
	}

	return false;
}

// scripting
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(taggable);
MAGE_DeclareScriptingConstructor(taggable(), "taggable");
MAGE_DeclareScriptingFunction(&taggable::hasTag, "hasTag");
MAGE_DeclareScriptingFunction(&taggable::tags, "tags");