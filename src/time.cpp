#include "time.h"

using namespace mage;

time::time()
{
}

time::time(sf::Time & sfTime)
{
	m_sfTime = sfTime;
}

time::time(float timeIn, e_measure measure)
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

sf::Time time::toSf() const
{
	return m_sfTime;
}

float time::get(e_measure measure) const
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

bool time::operator==(const time& iet)
{
	return m_sfTime == iet.m_sfTime;
}

bool time::operator!=(const time& iet)
{
	return m_sfTime != iet.m_sfTime;
}

bool time::operator<(const time& iet)
{
	return m_sfTime < iet.m_sfTime;
}

bool time::operator>=(const time& iet)
{
	return m_sfTime >= iet.m_sfTime;
}

bool time::operator<=(const time& iet)
{
	return m_sfTime <= iet.m_sfTime;
}

bool time::operator>(const time& iet)
{
	return m_sfTime > iet.m_sfTime;
}

time& time::operator*=(float rhs)
{
	m_sfTime *= rhs;
	return *this;
}

time& time::operator/=(float rhs)
{
	m_sfTime /= rhs;
	return *this;
}

time& time::operator+=(const time& rhs)
{
	m_sfTime += rhs.toSf();
	return *this;
}

time& time::operator-=(const time& rhs)
{
	m_sfTime -= rhs.toSf();
	return *this;
}

time& time::operator%=(const time& rhs)
{
	m_sfTime %= rhs.toSf();
	return *this;
}

time time::operator*(float rhs)
{
	return m_sfTime * rhs;
}

time time::operator/(float rhs)
{
	return m_sfTime / rhs;
}

time time::operator+(const time & rhs)
{
	return m_sfTime + rhs.toSf();
}

time time::operator-(const time & rhs)
{
	return m_sfTime - rhs.toSf();
}

time time::operator%(const time & rhs)
{
	return m_sfTime % rhs.toSf();
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingType(time);
MAGE_DeclareScriptingConstructor(time(), "time");
MAGE_DeclareScriptingConstructor(time(time&), "time");
MAGE_DeclareScriptingConstructor(time(float, time::e_measure), "time");
MAGE_DeclareScriptingFunction(&time::get, "get");
MAGE_DeclareScriptingFunction(&time::operator%, "%");
MAGE_DeclareScriptingFunction(&time::operator%=, "%=");
MAGE_DeclareScriptingFunction(&time::operator*, "*");
MAGE_DeclareScriptingFunction(&time::operator*=, "*=");
MAGE_DeclareScriptingFunction(&time::operator+, "+");
MAGE_DeclareScriptingFunction(&time::operator+=, "+=");
MAGE_DeclareScriptingFunction(&time::operator-, "-");
MAGE_DeclareScriptingFunction(&time::operator-=, "-=");
MAGE_DeclareScriptingFunction(&time::operator/, "/");
MAGE_DeclareScriptingFunction(&time::operator/=, "/=");
MAGE_DeclareScriptingFunction(&time::operator<, "<");
MAGE_DeclareScriptingFunction(&time::operator<=, "<=");
MAGE_DeclareScriptingFunction(&time::operator==, "==");
MAGE_DeclareScriptingFunction(&time::operator>, ">");
MAGE_DeclareScriptingFunction(&time::operator>=, ">=");
MAGE_DeclareScriptingFunction(&time::operator!=, "!=");