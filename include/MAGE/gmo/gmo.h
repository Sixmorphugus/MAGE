#pragma once

// mage::gmo (gameObject)
// -------------
// Represents an object in a scene.
// This object is updated each and every frame.
// -------------
// Category: Core
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "hookableLifetimeObject.h"
#include "propertiesObject.h"
#include "transformable.h"
#include "namable.h"

namespace mage {

class scene;
class prefab;
class interval;

// put MAGE_GmoOverloads in your class below the constructors to:
// - allow the class to be cloned without knowing its type in C++
// - allow the class to be a prefab
#define MAGE_GmoOverloads(gmoType)\
virtual std::shared_ptr<gmo> clone() { return std::make_shared<gmoType>(*this); }

class gmo :
	public renamable, // all game objects can have a name
	public propertiesObject, // all game objects get registered properties
	public virtual transformable,
	public hookableLifetimeObject
{
public:
	gmo(const pointF& position);
	gmo(const gmo& cp);

	bool isCloneable();
	virtual std::shared_ptr<gmo> clone();

	gmo& operator=(const gmo& cp);

	virtual void preUpdate(interval elapsedSinceLastUpdate);
	virtual void update(interval elapsedSinceLastUpdate);

	scene* getScene();
	std::shared_ptr<prefab> getPrefabSource();

	bool getRespectsPixelGrid();
	void setRespectsPixelGrid(bool yes = true);

public:
	hook<gmo*, interval> onPreUpdate;
	hook<gmo*, interval> onUpdate;

private:
	void copyFrom(const gmo& cp);

	std::shared_ptr<prefab> m_prefabSource;
	scene* m_scene;
	bool m_respectPixelGrid;

	friend class scene;
	friend class prefabMngr;
};

}