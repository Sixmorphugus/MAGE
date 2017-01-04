#pragma once

// mage::gameObject
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

class gmo :
	public renamable, // all game objects can have a name
	public propertiesObject, // all game objects get registered properties
	public virtual transformable,
	public hookableLifetimeObject
{
public:
	gmo(const pointF& position);
	gmo(const gmo& cp);

	gmo& operator=(const gmo& cp);

	virtual void preUpdate(sf::Time elapsed);
	virtual void update(sf::Time elapsed);

	scene* getScene();
	std::shared_ptr<prefab> getPrefabSource();

	bool getRespectsPixelGrid();
	void setRespectsPixelGrid(bool yes = true);

public:
	hook<gmo*, sf::Time> onPreUpdate;
	hook<gmo*, sf::Time> onUpdate;

private:
	void copyFrom(const gmo& cp);

	std::shared_ptr<prefab> m_prefabSource;
	scene* m_scene;
	bool m_respectPixelGrid;

	friend class scene;
	friend class prefabMngr;
};

}

#define MAGE_DeclareScriptingBasic(type)\
MAGE_DeclareScriptingType(type); \
MAGE_DeclareScriptingBaseClass(mage::basic, type); \
MAGE_DeclareScriptingBaseClass(mage::serializable, type); \
MAGE_DeclareScriptingBaseClass(sf::Transformable, type); \
MAGE_DeclareScriptingBaseClass(mage::shadable, type); \
MAGE_DeclareScriptingCastingFunction("to_" STRING(type), mage::basic, type);
