#include "timer.h"

using namespace mage;

timer::timer()
{
}

interval timer::getElapsedTime()
{
	return interval(m_clock.getElapsedTime());
}

interval mage::timer::restart()
{
	return interval(m_clock.restart());
}
