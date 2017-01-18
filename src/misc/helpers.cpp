#include "helpers.h"

#include "platform.h"
#include "maths.h"

namespace mage {

float randomPitch()
{
	return 1.f + ((float)randomInt(-2, 2) / 10.f);
}

} // namespace mage