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
		z = 0;
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
	inline point<T>::point(std::string & in)
	{
		auto split = splitString(in);

		if (split.size() > 0) {
			x = atof(split[0].c_str());
		}
		if (split.size() > 1) {
			y = atof(split[1].c_str());
		}
		if (split.size() > 2) {
			z = atof(split[2].c_str());
		}
	}

	template<typename T>
	inline point<T> & point<T>::operator+=(const point<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	template<typename T>
	inline point<T> & point<T>::operator-=(const point<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	template<typename T>
	inline point<T> & point<T>::operator*=(const point<T>& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;

		return *this;
	}

	template<typename T>
	inline point<T> & point<T>::operator/=(const point<T>& rhs)
	{
		// ignore attempts to devide by 0
		if (rhs.x != 0)
			x /= rhs.x;

		if (rhs.y != 0)
			y /= rhs.y;

		if (rhs.z != 0)
			z /= rhs.z;

		return *this;
	}

	template<typename T>
	inline point<T> & point<T>::operator*=(const T rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;

		return *this;
	}

	template<typename T>
	inline point<T> & point<T>::operator/=(const T rhs)
	{
		// ignore attempts to devide by 0
		if (rhs == 0)
			return *this;

		x /= rhs;
		y /= rhs;
		z /= rhs;

		return *this;
	}

	template<typename T>
	inline point<T> point<T>::operator+(const point<T>& rhs) const
	{
		point<T> newPt(*this);
		newPt += rhs;

		return newPt;
	}

	template<typename T>
	inline point<T> point<T>::operator-(const point<T>& rhs) const
	{
		point<T> newPt(*this);
		newPt -= rhs;

		return newPt;
	}

	template<typename T>
	inline point<T> point<T>::operator*(const point<T>& rhs) const
	{
		point<T> newPt(*this);
		newPt *= rhs;

		return newPt;
	}

	template<typename T>
	inline point<T> point<T>::operator/(const point<T>& rhs) const
	{
		point<T> newPt(*this);
		newPt /= rhs;

		return newPt;
	}

	template<typename T>
	inline point<T> point<T>::operator*(const T rhs) const
	{
		point<T> newPt(*this);
		newPt *= rhs;

		return newPt;
	}

	template<typename T>
	inline point<T> point<T>::operator/(const T rhs) const
	{
		point<T> newPt(*this);
		newPt /= rhs;

		return newPt;
	}

	template<typename T>
	inline bool point<T>::operator==(const point<T>& rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}

	template<typename T>
	inline bool point<T>::operator!=(const point<T>& rhs) const
	{
		return !(*this == rhs);
	}

	template<typename T>
	inline bool point<T>::operator<(const point<T>& rhs) const
	{
		return (x < rhs.x && y < rhs.y);
	}

	template<typename T>
	inline bool point<T>::operator>=(const point<T>& rhs) const
	{
		return !(*this < rhs);
	}

	template<typename T>
	inline bool point<T>::operator<=(const point<T>& rhs) const
	{
		return ((*this == rhs) || (*this < rhs));
	}

	template<typename T>
	inline bool point<T>::operator>(const point<T>& rhs) const
	{
		return !(*this <= rhs);
	}

	template<typename T>
	inline point<T> point<T>::getFloor() const
	{
		return point<T>(floor(x), floor(y), floor(z));
	}

	template<typename T>
	inline point<T> point<T>::getCeil() const
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
		point<T> v3(*this - v2);
		return v3.getAngle2D();
	}

	template<typename T>
	inline T point<T>::getDot(point<T>& v2) const
	{
		return ((*this) * v2).getSum();
	}

	template<typename T>
	inline T point<T>::getCross(point<T>& v2) const
	{
		return getSum() * v2.getSum();
	}
	template<typename T>
	inline T point<T>::getSum() const
	{
		return x + y + z;
	}
	template<typename T>
	inline point<T> point<T>::getRotatedAround(point<T>& pivot, T angle) const
	{
		point<T> pos(x, y);

		float s = sin(angle);
		float c = cos(angle);

		// translate point back to origin:
		pos -= pivot;

		// rotate point
		float xnew = pos.x * c - pos.y * s;
		float ynew = pos.x * s + pos.y * c;

		// translate point back:
		pos.x = xnew;
		pos.y = ynew;

		pos += pivot;

		return pos;
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
	template<typename T>
	inline std::string point<T>::toString() const
	{
		return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
	}

	template<typename T>
	inline point2<T>::point2()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	template<typename T>
	inline point2<T>::point2(T xIn, T yIn)
	{
		x = xIn;
		y = yIn;
		z = 0;
	}
	template<typename T>
	inline point2<T>::point2(T angle2D)
	{
		x = cos(angle2D);
		y = -sin(angle2D);
		z = 0;
	}
	template<typename T>
	inline point2<T>::point2(const sf::Vector2<T>& in)
	{
		x = in.x;
		y = in.y;
		z = 0;
	}
	template<typename T>
	inline point2<T>::point2(std::string & in)
	{
		auto split = splitString(in);

		if (split.size() > 0) {
			x = atof(split[0].c_str());
		}
		if (split.size() > 1) {
			y = atof(split[1].c_str());
		}

		z = 0;
	}
	template<typename T>
	inline point2<T>& point2<T>::operator+=(const point2<T>& rhs)
	{
		point<T>::operator+=(rhs);
		return *this;
	}
	template<typename T>
	inline point2<T>& point2<T>::operator-=(const point2<T>& rhs)
	{
		point<T>::operator-=(rhs);
		return *this;
	}
	template<typename T>
	inline point2<T>& point2<T>::operator*=(const point2<T>& rhs)
	{
		point<T>::operator*=(rhs);
		return *this;
	}
	template<typename T>
	inline point2<T>& point2<T>::operator/=(const point2<T>& rhs)
	{
		point<T>::operator/=(rhs);
		return *this;
	}
	template<typename T>
	inline point2<T>& point2<T>::operator*=(const T rhs)
	{
		point<T>::operator*=(rhs);
		return *this;
	}
	template<typename T>
	inline point2<T>& point2<T>::operator/=(const T rhs)
	{
		point<T>::operator/=(rhs);
		return *this;
	}
	template<typename T>
	inline point2<T> point2<T>::operator+(const point2<T>& rhs) const
	{
		return point<T>::operator+(rhs).to2();
	}
	template<typename T>
	inline point2<T> point2<T>::operator-(const point2<T>& rhs) const
	{
		return point<T>::operator-(rhs).to2();
	}
	template<typename T>
	inline point2<T> point2<T>::operator*(const point2<T>& rhs) const
	{
		return point<T>::operator*(rhs).to2();
	}
	template<typename T>
	inline point2<T> point2<T>::operator/(const point2<T>& rhs) const
	{
		return point<T>::operator/(rhs).to2();
	}
	template<typename T>
	inline point2<T> point2<T>::operator*(const T rhs) const
	{
		return point<T>::operator*(rhs).to2();
	}
	template<typename T>
	inline point2<T> point2<T>::operator/(const T rhs) const
	{
		return point<T>::operator/(rhs).to2();
	}
	template<typename T>
	inline bool point2<T>::operator==(const point2<T>& rhs) const
	{
		return point<T>::operator==(rhs);
	}
	template<typename T>
	inline bool point2<T>::operator!=(const point2<T>& rhs) const
	{
		return point<T>::operator!=(rhs);
	}
	template<typename T>
	inline bool point2<T>::operator<(const point2<T>& rhs) const
	{
		return point<T>::operator<(rhs.to3());
	}
	template<typename T>
	inline bool point2<T>::operator>=(const point2<T>& rhs) const
	{
		return point<T>::operator>=(rhs.to3());
	}
	template<typename T>
	inline bool point2<T>::operator<=(const point2<T>& rhs) const
	{
		return point<T>::operator<=(rhs.to3());
	}
	template<typename T>
	inline bool point2<T>::operator>(const point2<T>& rhs) const
	{
		return point<T>::operator>(rhs.to3());
	}
	template<typename T>
	inline point2<T> point2<T>::getFloor() const
	{
		return point2<T>(floor(x), floor(y));
	}
	template<typename T>
	inline point2<T> point2<T>::getCeil() const
	{
		return point2<T>(ceil(x), ceil(y));
	}
	template<typename T>
	inline T point2<T>::getAngle2D() const
	{
		return point<T>::getAngle2D();
	}
	template<typename T>
	inline T point2<T>::getLength() const
	{
		return point<T>::getLength();
	}
	template<typename T>
	inline T point2<T>::getDistanceTo(point2<T>& v2) const
	{
		return point<T>::getDistanceTo(v2.to3());
	}
	template<typename T>
	inline T point2<T>::getAngle2DTo(point2<T>& v2) const
	{
		return point<T>::getAngle2DTo(v2.to3());
	}
	template<typename T>
	inline T point2<T>::getDot(point2<T>& v2) const
	{
		return point<T>::getDot(v2.to3());
	}
	template<typename T>
	inline T point2<T>::getCross(point2<T>& v2) const
	{
		return point<T>::getCross(v2.to3());
	}
	template<typename T>
	inline T point2<T>::getSum() const
	{
		return point<T>::getSum();
	}
	template<typename T>
	inline point2<T> point2<T>::getRotatedAround(point2<T>& pivot, T angle) const
	{
		return point<T>::getRotatedAround(pivot.to3(), angle).to2();
	}
	template<typename T>
	inline std::string point2<T>::toString() const
	{
		std::string pointStr = point<T>::toString();
		auto pointStrSplit = splitString(pointStr);

		return pointStrSplit[0] + " " + pointStrSplit[1];
	}
	template<typename T>
	inline point<T> point2<T>::to3() const
	{
		return point<T>(x, y);
	}
	template<typename T>
	template<typename To>
	inline point<To> point<T>::convertAxis() const
	{
		point<To> newPt;

		newPt.x = (To)x;
		newPt.y = (To)y;
		newPt.z = (To)z;
	}

	template<typename T>
	template<typename To>
	inline point2<To> point2<T>::convertAxis() const
	{
		point2<To> newPt;

		newPt.x = (To)x;
		newPt.y = (To)y;

		return newPt;
	}
	template<typename T>
	inline point2<T> point<T>::to2() const
	{
		return point2<T>(x, y);
	}
}