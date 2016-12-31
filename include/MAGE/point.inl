#include "point.h"
namespace mage {
	template<typename T>
	inline point<T>::point()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	template<typename T>
	inline point<T>::point(T xi, T yi, T zi)
	{
		x = xi;
		y = yi;
		z = zi;
	}

	template<typename T>
	inline point<T>::point(T xyAngle)
	{
		x = cos(xyAngle);
		y = -sin(xyAngle);
	}

	template<typename T>
	inline point<T>::point(T angleAlpha, T angleBeta)
	{
		x = cos(alpha)*cos(beta);
		z = sin(alpha)*cos(beta);
		y = sin(beta);
	}

	template<typename T>
	inline point<T>::point(const sf::Vector2<T>& in)
	{
		x = in.x;
		y = in.y;
		z = 0;
	}

	template<typename T>
	inline point<T>::point(const sf::Vector3<T>& in)
	{
		x = in.x;
		y = in.y;
		z = in.z;
	}

	template<typename T>
	inline point & point<T>::operator+=(const point<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	template<typename T>
	inline point & point<T>::operator-=(const point<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	template<typename T>
	inline point & point<T>::operator*=(const point<T>& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;

		return *this;
	}

	template<typename T>
	inline point & point<T>::operator/=(const point<T>& rhs)
	{
		// ignore attempts to devide by 0
		if (rhs.x != 0) x /= rhs.x;
		if (rhs.y != 0) y /= rhs.y;
		if (rhs.z != 0) z /= rhs.z;

		return *this;
	}

	template<typename T>
	inline point & point<T>::operator*=(const T rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;

		return *this;
	}

	template<typename T>
	inline point & point<T>::operator/=(const T rhs)
	{
		if (rhs == 0) { // ignore attempts to devide by 0
			return *this;
		}

		x /= rhs;
		y /= rhs;
		z /= rhs;

		return *this;
	}

	template<typename T>
	inline point mage::point<T>::operator+(const point<T>& rhs)
	{
		point<T> newPt(*this);
		newPt += rhs;

		return newPt;
	}

	template<typename T>
	inline point mage::point<T>::operator-(const point<T>& rhs)
	{
		point<T> newPt(*this);
		newPt -= rhs;

		return newPt;
	}

	template<typename T>
	inline point mage::point<T>::operator*(const point<T>& rhs)
	{
		point<T> newPt(*this);
		newPt *= rhs;

		return newPt;
	}

	template<typename T>
	inline point mage::point<T>::operator/(const point<T>& rhs)
	{
		point<T> newPt(*this);
		newPt /= rhs;

		return newPt;
	}

	template<typename T>
	inline point mage::point<T>::operator*(const T rhs)
	{
		point<T> newPt(*this);
		newPt *= rhs;

		return newPt;
	}

	template<typename T>
	inline point mage::point<T>::operator/(const T rhs)
	{
		point<T> newPt(*this);
		newPt /= rhs;

		return newPt;
	}

	template<typename T>
	inline bool mage::point<T>::operator==(const point<T>& rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}

	template<typename T>
	inline bool mage::point<T>::operator!=(const point<T>& rhs) const
	{
		return !(*this == rhs);
	}

	template<typename T>
	inline point point<T>::floor() const
	{
		return point<T>(floor(x), floor(y), floor(z));
	}

	template<typename T>
	inline point point<T>::ceil() const
	{
		return point<T>(ceil(x), ceil(y), ceil(z));
	}

	template<typename T>
	inline T point<T>::getAngle2D() const
	{
		return atan2(x, y);
	}

	template<typename T>
	inline T point<T>::getLength() const
	{
		return sqrt(y*y + x*x + z*z); // this is just pythag again isnt it
	}

	template<typename T>
	inline T point<T>::getDistanceTo(point<T>& v2) const
	{
		// pythagoras method
		float xDist = fabs(x - v2.x);
		float yDist = fabs(y - v2.y);
		float zDist = fabs(y - v2.y);

		return sqrt(xDist*xDist + yDist*yDist + zDist*zDist);
	}

	template<typename T>
	inline T point<T>::getAngle2DTo(point<T>& v2) const
	{
		// toAngle does all the work here already.
		point<T> v3(v1 - v2);
		return v3.toAngle();
	}

	template<typename T>
	inline T point<T>::getDot(point<T>& v2) const
	{
		return ((*this) * v2);
	}

	template<typename T>
	inline T mage::point<T>::getCross(point<T>& v2) const
	{
		return (x + y + z) * (v2.x + v2.y + v2.z);
	}
	template<typename T>
	inline sf::Vector2<T> point<T>::toSf2() const
	{
		return sf::Vector2<T>(x, y);
	}
	template<typename T>
	inline sf::Vector3<T> point<T>::toSf3() const
	{
		return sf::Vector3<T>(x, y, z);
	}
}