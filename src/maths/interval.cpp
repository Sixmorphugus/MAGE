#include "interval.h"

using namespace mage;

interval::interval()
{
}

interval::interval(const sf::Time & sfTime)
{
	m_sfTime = sfTime;
}

interval::interval(float timeIn, e_measure measure)
{ 
	switch (measure) {
	case SECONDS:
		m_sfTime = sf::seconds(timeIn);
		break;
	case MILLISECONDS:
		m_sfTime = sf::milliseconds((sf::Int32)timeIn);
		break;
	case MICROSECONDS:
		m_sfTime = sf::microseconds((sf::Int64)timeIn);
		break;
	}
}

sf::Time interval::toSf() const
{
	return m_sfTime;
}

float interval::get(e_measure measure) const
{
	switch (measure) {
	case SECONDS:
		return m_sfTime.asSeconds();
	case MILLISECONDS:
		return (float)m_sfTime.asMilliseconds();
	case MICROSECONDS:
		return (float)m_sfTime.asMicroseconds();
	}

	return m_sfTime.asSeconds();
}

bool interval::operator==(const interval& iet)
{
	return m_sfTime == iet.m_sfTime;
}

bool interval::operator!=(const interval& iet)
{
	return m_sfTime != iet.m_sfTime;
}

bool interval::operator<(const interval& iet)
{
	return m_sfTime < iet.m_sfTime;
}

bool interval::operator>=(const interval& iet)
{
	return m_sfTime >= iet.m_sfTime;
}

bool interval::operator<=(const interval& iet)
{
	return m_sfTime <= iet.m_sfTime;
}

bool interval::operator>(const interval& iet)
{
	return m_sfTime > iet.m_sfTime;
}

interval& interval::operator*=(float rhs)
{
	m_sfTime *= rhs;
	return *this;
}

interval& interval::operator/=(float rhs)
{
	m_sfTime /= rhs;
	return *this;
}

interval& interval::operator/=(const interval & rhs)
{
	m_sfTime = sf::milliseconds(get(interval::MILLISECONDS) / rhs.get(interval::MILLISECONDS));
	return *this;
}

interval& interval::operator+=(const interval& rhs)
{
	m_sfTime += rhs.toSf();
	return *this;
}

interval& interval::operator-=(const interval& rhs)
{
	m_sfTime -= rhs.toSf();
	return *this;
}

interval& interval::operator%=(const interval& rhs)
{
	m_sfTime %= rhs.toSf();
	return *this;
}

interval interval::operator*(float rhs)
{
	return m_sfTime * rhs;
}

interval interval::operator/(float rhs)
{
	return m_sfTime / rhs;
}

interval interval::operator/(const interval & rhs)
{
	interval thClone = *this;
	thClone /= rhs;

	return thClone;
}

interval interval::operator+(const interval & rhs)
{
	return m_sfTime + rhs.toSf();
}

interval interval::operator-(const interval & rhs)
{
	return m_sfTime - rhs.toSf();
}

interval interval::operator%(const interval & rhs)
{
	return m_sfTime % rhs.toSf();
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingType(interval);
MAGE_DeclareScriptingConstructor(interval(), "interval");
MAGE_DeclareScriptingConstructor(interval(interval&), "interval");
MAGE_DeclareScriptingConstructor(interval(float, interval::e_measure), "interval");
MAGE_DeclareScriptingFunction(&interval::get, "get");
MAGE_DeclareScriptingFunction(&interval::operator%, "%");
MAGE_DeclareScriptingFunction(&interval::operator%=, "%=");
MAGE_DeclareScriptingFunction(&interval::operator*, "*");
MAGE_DeclareScriptingFunction(&interval::operator*=, "*=");
MAGE_DeclareScriptingFunction(&interval::operator+, "+");
MAGE_DeclareScriptingFunction(&interval::operator+=, "+=");
MAGE_DeclareScriptingFunction(&interval::operator-, "-");
MAGE_DeclareScriptingFunction(&interval::operator-=, "-=");
MAGE_DeclareScriptingFunction(&interval::operator<, "<");
MAGE_DeclareScriptingFunction(&interval::operator<=, "<=");
MAGE_DeclareScriptingFunction(&interval::operator==, "==");
MAGE_DeclareScriptingFunction(&interval::operator>, ">");
MAGE_DeclareScriptingFunction(&interval::operator>=, ">=");
MAGE_DeclareScriptingFunction(&interval::operator!=, "!=");
MAGE_DeclareScriptingCustom(chaiscript::fun<interval&, interval, const interval&>(&interval::operator/=), "/=");
MAGE_DeclareScriptingCustom(chaiscript::fun<interval, interval, const interval&>(&interval::operator/), "/");
MAGE_DeclareScriptingCustom(chaiscript::fun<interval&, interval, float>(&interval::operator/=), "/=");
MAGE_DeclareScriptingCustom(chaiscript::fun<interval, interval, float>(&interval::operator/), "/");