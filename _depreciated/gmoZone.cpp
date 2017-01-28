#include "gmoZone.h"

#include "Game.h"
#include "scene.h"

using namespace mage;

gmoZone::gmoZone()
{
	touchingCountsAsInside = false;
}

gmoZone::gmoZone(const pointF& pos, const pointF& size) :
	gmo(pos, size)
{
	touchingCountsAsInside = false;
}

void gmoZone::update(sf::Time elapsed)
{
	// is anything in this scene in the zone
	zoneTest(getScene()->objectList(), true);

	// should things in the zone be removed
	zoneTest(m_objectsInZone, false);
}

// by default these do nothing, derive the object
void gmoZone::objectEnterZone(std::shared_ptr<gmo> obj)
{
	onObjectEnterZone.notify(this, obj.get());
	m_objectsInZone.push_back(obj);
}

void gmoZone::objectLeaveZone(std::shared_ptr<gmo> obj)
{
	onObjectLeaveZone.notify(this, obj.get());
	m_objectsInZone.erase(m_objectsInZone.begin() + indexInZone(obj));
}

int gmoZone::indexInZone(std::shared_ptr<gmo> obj)
{
	for (unsigned int i = 0; i < m_objectsInZone.size(); i++) {
		if (m_objectsInZone[i] == obj) {
			return i;
		}
	}

	return -1;
}

void gmoZone::zoneTest(std::vector<std::shared_ptr<gmo>> objList, bool into)
{
	for (int i = 0; i < objList.size(); i++) {
		auto obj = objList[i];

		if (indexInZone(obj))
			continue;

		// resizable objects count as inside if their box is inside the zone.
		if (touchingCountsAsInside) {
			if (getBox().intersects(obj->getBox())) {
				objectEnterZone(obj);
			}
		}
		else {
			if (getBox().contains(obj->getBox())) {
				if (into)
					objectEnterZone(obj);
				else
					objectLeaveZone(obj);
			}
		}
	}
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingGmoType(gmoZone);
MAGE_DeclareScriptingConstructor(gmoZone(const pointF& position, const pointF& size), "gmoZone");
MAGE_DeclareScriptingFunction(&gmoZone::indexInZone, "indexInZone");
MAGE_DeclareScriptingFunction(&gmoZone::onObjectEnterZone, "onObjectEnterZone");
MAGE_DeclareScriptingFunction(&gmoZone::onObjectLeaveZone, "onObjectLeaveZone");

MAGE_DeclareScriptingHook("gmoZoneHook", gmoZone*, gmo*);