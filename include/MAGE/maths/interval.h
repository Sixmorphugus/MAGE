#pragma once

// mage::interval
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
	class MAGEDLL interval {
	public:
		enum e_measure
		{
			SECONDS,
			MILLISECONDS,
			MICROSECONDS
		};

	public:
		interval();
		interval(const sf::Time& sfTime);
		interval(float timeIn, e_measure measure = SECONDS);

		sf::Time toSf() const;

		float get(e_measure measure = SECONDS) const;

		// comparative operators
		bool operator==(const interval& iet);
		bool operator!=(const interval& iet);

		bool operator<(const interval& iet);
		bool operator>=(const interval& iet);
		bool operator<=(const interval& iet);
		bool operator>(const interval& iet);

		// arithmetic operators
		interval& operator*=(float rhs);
		interval& operator/=(float rhs);
		interval& operator/=(const interval& rhs);
		interval& operator+=(const interval& rhs);
		interval& operator-=(const interval& rhs);
		interval& operator%=(const interval& rhs);

		interval operator*(float rhs);
		interval operator/(float rhs);
		interval operator/(const interval& rhs);
		interval operator+(const interval& rhs);
		interval operator-(const interval& rhs);
		interval operator%(const interval& rhs);

	private:
		sf::Time m_sfTime;
	};
}