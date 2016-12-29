#include "point.h"
#include "point.h"
#include "point.h"
#include "point.h"
#include "point.h"
#include "point.h"
#include "point.h"
#include "point.h"
template<typename T>
inline point2<T>::point2()
{
	x = 0;
	y = 0;
}

template<typename T>
inline point2<T>::point2(T xi, T yi)
{
	x = xi;
	y = yi;
}

template<typename T>
inline point2<T>::point2(T angle)
{
	x = cos(angle);
	y = sin(angle);
}

template<typename T>
inline point2<T>::point2(const sf::Vector2<T>& in)
{
	x = in.x;
	y = in.y;
}

template<typename T>
inline point2 & point2<T>::operator+=(const point2<T>& rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

template<typename T>
inline point2 & point2<T>::operator-=(const point2<T>& rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

template<typename T>
inline point2 & point2<T>::operator*=(const point2<T>& rhs)
{
	x *= rhs.x;
	y *= rhs.y;

	return *this;
}

template<typename T>
inline point2 & point2<T>::operator/=(const point2<T>& rhs)
{
	x /= rhs.x;
	y /= rhs.y;

	return *this;
}

template<typename T>
inline point2 & point2<T>::operator*=(const T rhs)
{
	x *= rhs;
	y *= rhs;

	return *this;
}

template<typename T>
inline point2 & point2<T>::operator/=(const T rhs)
{
	x /= rhs;
	y /= rhs;

	return *this;
}

template<typename T>
inline point2 mage::point2<T>::operator+(const point2<T>& rhs)
{
	point2<T> newPt(*this);
	newPt += rhs;

	return newPt;
}

template<typename T>
inline point2 mage::point2<T>::operator-(const point2<T>& rhs)
{
	point2<T> newPt(*this);
	newPt -= rhs;

	return newPt;
}

template<typename T>
inline point2 mage::point2<T>::operator*(const point2<T>& rhs)
{
	point2<T> newPt(*this);
	newPt *= rhs;

	return newPt;
}

template<typename T>
inline point2 mage::point2<T>::operator/(const point2<T>& rhs)
{
	point2<T> newPt(*this);
	newPt /= rhs;

	return newPt;
}

template<typename T>
inline point2 mage::point2<T>::operator*(const T rhs)
{
	point2<T> newPt(*this);
	newPt *= rhs;

	return newPt;
}

template<typename T>
inline point2 mage::point2<T>::operator/(const T rhs)
{
	point2<T> newPt(*this);
	newPt /= rhs;

	return newPt;
}

template<typename T>
inline bool mage::point2<T>::operator==(const point2<T>& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

template<typename T>
inline bool mage::point2<T>::operator!=(const point2<T>& rhs) const
{
	return !(*this == rhs);
}

template<typename T>
inline point2 point2<T>::floor() const
{
	return point2<T>(floor(x), floor(y));
}

template<typename T>
inline point2 point2<T>::ceil() const
{
	return point2<T>(ceil(x), ceil(y));
}

template<typename T>
inline float point2<T>::getAngle() const
{
	return atan2(y, x);;
}

template<typename T>
inline float point2<T>::getLength() const
{
	return sqrt(y*y + x*x); // this is just pythag again isnt it
}

template<typename T>
inline float point2<T>::getDistanceTo(point2<T>& v2) const
{
	// pythagoras method
	float xDist = fabs(x - v2.x);
	float yDist = fabs(y - v2.y);

	return sqrt(pow(xDist, 2) + pow(yDist, 2));
}

template<typename T>
inline float point2<T>::getDirectionTo(point2<T>& v2) const
{
	point2<T> v3(v1 - v2);

	return v3.toAngle();
}

template<typename T>
inline float point2<T>::getDotProduct(point2<T>& v2) const
{
	return (x*v2.x + y*v2.y);
}

template<typename T>
inline sf::Vector2<T> point2<T>::toSf() const
{
	return sf::Vector2<T>(x, y);
}