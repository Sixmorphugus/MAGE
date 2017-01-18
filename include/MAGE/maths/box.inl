namespace mage {
	template<typename T>
	inline box<T>::box()
	{
	}

	template<typename T>
	inline box<T>::box(sf::Rect<T> conv)
	{
		position.x = conv.left;
		position.y = conv.top;
		size.x = conv.width;
		size.y = conv.height;
	}

	template<typename T>
	inline box<T>::box(point<T> positionIn)
	{
		position = positionIn;
	}

	template<typename T>
	inline box<T>::box(point<T> corner1OrPosition, point<T> corner2OrSize, shapeInit inm)
	{
		if (inm == POSANDSIZE) {
			position = corner1OrPosition;
			size = corner2OrSize;
		}
		else {
			position = corner1OrPosition;
			size = corner2OrSize - corner1OrPosition;
		}
	}

	template<typename T>
	inline box<T>::box(std::vector<box<T>> fBoxes)
	{
		if (!fBoxes.size())
			return;

		// initialize with just the first values
		point<T> tlMost = point<T>(fBoxes[0].position.x, fBoxes[0].position.y);
		point<T> brMost = point<T>(fBoxes[0].position.x + fBoxes[0].size.x, fBoxes[0].position.y + fBoxes[0].size.y);

		for (unsigned int i = 1; i < fBoxes.size(); i++) {
			sf::Vector2<T> tlCurrent = sf::Vector2<T>(fBoxes[i].position.x, fBoxes[i].position.y);
			sf::Vector2<T> brCurrent = sf::Vector2<T>(fBoxes[i].position.x + fBoxes[i].size.x, fBoxes[i].position.x + fBoxes[i].size.y);

			if (tlCurrent.x < tlMost.x) {
				tlMost.x = tlCurrent.x;
			}
			if (tlCurrent.y < tlMost.y) {
				tlMost.y = tlCurrent.y;
			}

			if (brCurrent.x > brMost.x) {
				brMost.x = brCurrent.x;
			}
			if (brCurrent.y > brMost.y) {
				brMost.y = brCurrent.y;
			}
		}

		// make a rectangle from final values
		position = tlMost;
		size = tlMost - brMost;
	}

	template<typename T>
	inline void box<T>::setCorner1(point<T> p)
	{
		// how much is the topLeft moving by?
		auto movedBy = p - position;
		position = p;

		// therefore we change size by that much
		size -= movedBy;
	}

	template<typename T>
	inline void box<T>::setCorner2(point<T> p)
	{
		// set corner two minus corner one.
		size = p - position;
	}

	template<typename T>
	inline void box<T>::moveCorner1(point<T> m)
	{
		setCorner1(position + m);
	}

	template<typename T>
	inline void box<T>::moveCorner2(point<T> m)
	{
		setCorner2(getSecondCorner() + m);
	}

	template<typename T>
	inline point<T> box<T>::getSecondCorner() const
	{
		return (position + size);
	}

	template<typename T>
	inline void box<T>::scale(point<T> scalar)
	{
		size *= scalar;
	}

	template<typename T>
	inline box<T> box<T>::floor()
	{
		return box<T>(position.floor(), size.floor());
	}

	template<typename T>
	inline box<T> box<T>::ceil()
	{
		return box<T>(position.ceil(), size.ceil());
	}

	template<typename T>
	inline bool box<T>::contains(const point<T>& pointIn) const
	{
		if (pointIn > position && pointIn < getSecondCorner())
			return true;

		return false;
	}

	template<typename T>
	inline bool box<T>::contains(const line<T>& lineIn) const
	{
		if (contains(lineIn.start) && contains(lineIn.getEndPoint()))
			return true;

		return false;
	}

	template<typename T>
	inline bool box<T>::contains(const box<T>& boxIn) const
	{
		if (contains(boxIn.position) && contains(boxIn.getSecondCorner()))
			return true;

		return false;
	}

	template<typename T>
	inline bool box<T>::intersects(const line<T>& lineIn) const
	{
		auto lines = getLines();

		// we know we have 4 lines
		if (lines[0].intersects(lineIn) || lines[1].intersects(lineIn) || lines[2].intersects(lineIn) || lines[3].intersects(lineIn))
			return true;

		// line could also be completely inside
		if (contains(lineIn))
			return true;

		return false;
	}

	template<typename T>
	inline bool box<T>::intersects(const box<T>& boxIn) const
	{
		// sfml's intersection handling code is better for this than what I could write
		auto r = toSf();
		return r.intersects(boxIn.toSf());
	}

	template<typename T>
	inline box<T> box<T>::normalize() const
	{
		box<T> copy(*this);

		if (copy.size.x < 0) {
			copy.size.x = abs(copy.size.x);
			copy.position.x += copy.size.x;
		}

		if (copy.size.y < 0) {
			copy.size.y = abs(copy.size.y);
			copy.position.y += copy.size.y;
		}

		return copy;
	}

	template<typename T>
	inline point<T> box<T>::clampedPoint(point<T>& pointIn)
	{
		auto sc = getSecondCorner();

		return point<T>(clamp(pointIn.x, position.x, sc.x),
			clamp(pointIn.y, position.y, sc.y));
	}

	template<typename T>
	inline point<T> box<T>::distanceToOuterEdge(point<T>& pointIn)
	{
		return clampedPoint(pointIn) - pointIn;
	}

	template<typename T>
	inline sf::Rect<T> box<T>::toSf() const
	{
		return sf::Rect<T>(position.toSf2(), size.toSf2());
	}

	template<typename T>
	inline std::vector<line<T>> box<T>::getLines() const
	{
		std::vector<line<T>> lines = {
			line<T>(position, point2<T>(size.x, 0.f)),
			line<T>(position + point<T>(0.f, size.y), point2<T>(size.x, size.y)),
			line<T>(position, point2<T>(0.f, size.y)),
			line<T>(position + point<T>(size.x, 0.f), point2<T>(size.x, size.y))
		};

		return lines;
	}

	template<typename T>
	inline box<T>& box<T>::operator*=(const T rH)
	{
		scale(point<T>(rH, rH));
		return *this;
	}

	template<typename T>
	inline box<T>& box<T>::operator/=(const T rH)
	{
		scale(point<T>(1 / rH, 1 / rH));
		return *this;
	}

	template<typename T>
	inline box<T>& box<T>::operator*=(const point<T>& rH)
	{
		scale(rH);
		return *this;
	}

	template<typename T>
	inline box<T>& box<T>::operator/=(const point<T>& rH)
	{
		scale(point<T>(1, 1) / rH);
		return *this;
	}

	template<typename T>
	inline box<T> box<T>::operator*(const T rH)
	{
		box<T> newBox(*this);
		newBox *= rH;

		return newBox;
	}

	template<typename T>
	inline box<T> box<T>::operator/(const T rH)
	{
		box<T> newBox(*this);
		newBox /= rH;

		return newBox;
	}

	template<typename T>
	inline box<T> box<T>::operator*(const point<T>& rH)
	{
		box<T> newBox(*this);
		newBox *= rH;

		return newBox;
	}

	template<typename T>
	inline box<T> box<T>::operator/(const point<T>& rH)
	{
		box<T> newBox(*this);
		newBox /= rH;

		return newBox;
	}
	template<typename T>
	inline bool box<T>::operator==(const box<T>& bx)
	{
		return (position == bx.position && size == bx.size);
	}
	template<typename T>
	inline bool box<T>::operator!=(const box<T>& bx)
	{
		return !(*this == bx);
	}
}