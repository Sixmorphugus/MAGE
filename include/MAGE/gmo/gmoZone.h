#pragma once

#include "gmo.h"

namespace mage {

// zone object - keeps track of objects inside it, and can do stuff to them.
class MAGEDLL gmoZone : public gmo {
public:
	gmoZone();
	gmoZone(const pointF& pos, const pointF& size);

	virtual void update(sf::Time elapsed);
	int indexInZone(std::shared_ptr<gmo> obj);

public:
	hook<gmoZone*, gmo*> onObjectEnterZone;
	hook<gmoZone*, gmo*> onObjectLeaveZone;

	bool touchingCountsAsInside;

private:
	void zoneTest(std::vector<std::shared_ptr<gmo>> objList, bool into);
	virtual void objectEnterZone(std::shared_ptr<gmo> obj);
	virtual void objectLeaveZone(std::shared_ptr<gmo> obj);

private:
	std::vector<std::shared_ptr<gmo>> m_objectsInZone;
};

} // namespace mage