#pragma once

#include "gmoResizable.h"

namespace mage {

// zone object - keeps track of objects inside it, and can do stuff to them.
class MAGEDLL gmoZone : public gmoResizable {
public:
	gmoZone(pointF pos, pointF size);

	virtual void update(sf::Time elapsed);

	virtual void objectEnterZone(std::shared_ptr<gmo> obj);
	virtual void objectLeaveZone(std::shared_ptr<gmo> obj);

	int indexInZone(std::shared_ptr<gmo> obj);

public:
	hook<gmoZone*, gmo*> onObjectEnterZone;
	hook<gmoZone*, gmo*> onObjectLeaveZone;

	bool touchingCountsAsInside;

private:
	void zoneTest(std::vector<std::shared_ptr<gmo>> objList, bool into);

private:
	std::vector<std::shared_ptr<gmo>> m_objectsInZone;
};

} // namespace mage