#pragma once

// mage::time
// -------------
// Wrapper for sf::Time.
// -------------
// Category: Maths
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "StdAfx.h"
#include "SfmlAfx.h"

namespace mage {
	class MAGEDLL time {
	public:
		enum e_measure
		{
			SECONDS,
			MILLISECONDS,
			MICROSECONDS
		};

	public:
		time();
		time(sf::Time& sfTime);
		time(float timeIn, e_measure measure = SECONDS);

		sf::Time toSf() const;

		float get(e_measure measure = SECONDS) const;

		// comparative operators
		bool operator==(const time& iet);
		bool operator!=(const time& iet);

		bool operator<(const time& iet);
		bool operator>=(const time& iet);
		bool operator<=(const time& iet);
		bool operator>(const time& iet);

		// arithmetic operators
		time& operator*=(float rhs);
		time& operator/=(float rhs);
		time& operator+=(const time& rhs);
		time& operator-=(const time& rhs);
		time& operator%=(const time& rhs);

		time operator*(float rhs);
		time operator/(float rhs);
		time operator+(const time& rhs);
		time operator-(const time& rhs);
		time operator%(const time& rhs);

	private:
		sf::Time m_sfTime;
	};
}