#include "line.h"
namespace mage {
	template<typename T>
	inline line<T>::line()
	{
	}

	template<typename T>
	inline line<T>::line(const point<T>& s, const point2<T>& endOrSize, shapeInit initMode)
	{
		start = s;

		if (initMode == POSANDSIZE)
			endOffset = endOrSize;
		else
			endOffset = endOrSize - start.to2();
	}

	template<typename T>
	inline line<T>::line(const point2<T>& zeroVector)
	{
		setEndOffset(zeroVector);
	}

	template<typename T>
	inline line<T>::line(T zeroAngle)
	{
		auto zeroVector = point2<T>(zeroAngle);
		setEndOffset(zeroVector);
	}

	template<typename T>
	inline int line<T>::orientation(point2<T>& r) const
	{
		auto end = getEndPoint();

		// from geeksforgeeks.com
		// http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
		int val = (end.y - start.y) * (r.x - end.x) -
			(end.x - start.x) * (r.y - end.y);

		if (val == 0) return 0;  // colinear

		return (val > 0) ? 1 : 2; // clock or counterclock wise
	}

	template<typename T>
	inline bool line<T>::intersects(const point<T>& point) const
	{
		auto end = getEndPoint();

		// from geeksforgeeks.com
		// http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

		if (end.x <= std::max(start.x, point.x) && end.x >= std::min(start.x, point.x) &&
			end.y <= std::max(end.y, point.y) && end.y >= std::min(start.y, point.y))
			return (point.z == start.z);

		return false;
	}

	template<typename T>
	inline bool line<T>::intersects(const line<T>& point) const
	{
		// from geeksforgeeks.com
		// http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

		// Find the four orientations needed for general and
		// special cases
		int o1 = orientation(point.start.to2());
		int o2 = orientation(point.getEndPoint().to2());
		int o3 = point.orientation(start.to2());
		int o4 = point.orientation(getEndPoint().to2());

		// General case
		if (o1 != o2 && o3 != o4)
			return true;

		// Special Cases
		// start, end and point.start are colinear and point.start lies on segment startend
		if (o1 == 0 && intersects(point.start)) return (point.start.z == start.z);

		// start, end and point.start are colinear and point.end lies on segment startend
		if (o2 == 0 && intersects(point.getEndPoint())) return (point.start.z == start.z);

		// point.start, point.end and start are colinear and start lies on segment point.startpoint.end
		if (o3 == 0 && intersects(start)) return (point.start.z == start.z);

		// point.start, point.end and end are colinear and end lies on segment point.startpoint.end
		if (o4 == 0 && intersects(getEndPoint())) return (point.start.z == start.z);

		return false; // Doesn't fall in any of the above cases
	}

	template<typename T>
	inline point<T> line<T>::getPointDifference() const
	{
		return getEndPoint() - start;
	}

	template<typename T>
	inline float line<T>::getDistance() const
	{
		return start.getDistanceTo(getEndPoint());
	}

	template<typename T>
	inline float line<T>::getAngle() const
	{
		return start.getAngle2DTo(getEndPoint());
	}

	template<typename T>
	inline void line<T>::setStartOffset(point<T>& offset)
	{
		start = (getEndPoint() + offset);
	}

	template<typename T>
	inline void line<T>::setEndPoint(point<T>& offset)
	{
		endOffset = offset.to2() - start.to2();
		start.z = offset.z;
	}

	template<typename T>
	inline point<T> line<T>::getEndPoint() const
	{
		return start + endOffset.to3();
	}
}