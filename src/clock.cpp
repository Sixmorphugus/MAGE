#include "clock.h"

using namespace mage;

clock::clock()
{
}

time clock::getElapsedTime()
{
	return time(m_clock.getElapsedTime());
}

time mage::clock::restart()
{
	return time(m_clock.restart());
}
