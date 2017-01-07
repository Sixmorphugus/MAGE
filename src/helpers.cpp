#include "helpers.h"

#include "platform.h"
#include "maths.h"

namespace mage {

float randomPitch()
{
	return 1.f + ((float)randomInt(-2, 2) / 10.f);
}

} // namespace mage


#include "scriptingEngine.h"
using namespace mage;
using namespace chaiscript;

MAGE_DeclareScriptingCustom(fun(&interpolateColors), "interpolateColors");
MAGE_DeclareScriptingCustom(fun(&distanceBetweenObjects), "distanceBetweenObjects");