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
#include "transformableObject.h"
#include "namable.h"
#include "renderable.h"
#include "scene.h"

namespace mage {

class scene;
class prefab;
class interval;
class resourceSoundBuffer;
class gmoComponent;

class gmo :
	public renamable, // all game objects can have a name
	public propertiesObject, // all game objects get registered properties
	public transformableObject, // all game objects are (basically) transformable
	public hookableLifetimeObject, // onCreated + onDestroyed
	public renderable // produces a drawRecipe
{
public:
	gmo();
	gmo(const pointF& position);
	gmo(const pointF& position, const pointF& size);
	gmo(const gmo& cp);

	void init();

	bool isCloneable();
	virtual std::shared_ptr<gmo> clone();

	gmo& operator=(const gmo& cp);

	virtual void preUpdate(interval elapsedSinceLastUpdate);
	virtual void update(interval elapsedSinceLastUpdate);

	scene* getScene();
	std::shared_ptr<prefab> getPrefabSource();

	bool getRespectsPixelGrid() const;
	void setRespectsPixelGrid(bool yes = true);

	// helpers
	void moveWithCollision(pointF movement);

	// components
	unsigned int addComponent(std::shared_ptr<gmoComponent> newComp);
	void removeComponent(unsigned int id);
	unsigned int indexOfComponent(std::shared_ptr<gmoComponent> newComp) const;
	std::shared_ptr<gmoComponent> getComponent(unsigned int id);
	unsigned int getNumComponents() const;
public:
	hook<gmo*, interval> onPreUpdate;
	hook<gmo*, interval> onUpdate;

	bool test;

protected:
	virtual renderRecipe generateDrawRecipe();

private:
	void copyFrom(const gmo& cp);

	std::shared_ptr<prefab> m_prefabSource;
	scene* m_scene;
	bool m_respectPixelGrid;

	friend class scene;
	friend class prefabMngr;
};

}

// put MAGE_GmoOverloads in your class below the constructors to:
// - allow the class to be cloned without knowing its type in C++
// - allow the class to be a prefab
#define MAGE_GmoOverloads(gmoType)\
virtual std::shared_ptr<gmo> clone() { return std::make_shared<gmoType>(*this); }

#define MAGE_DeclareScriptingGmoType(type)\
MAGE_DeclareScriptingType(type);\
MAGE_DeclareScriptingBaseClass(namable, type);\
MAGE_DeclareScriptingBaseClass(renamable, type);\
MAGE_DeclareScriptingBaseClass(serializable, type);\
MAGE_DeclareScriptingBaseClass(propertiesObject, type);\
MAGE_DeclareScriptingBaseClass(transformableObject, type);\
MAGE_DeclareScriptingBaseClass(renderable, type);\
MAGE_DeclareScriptingBaseClass(scene, type);\
MAGE_DeclareScriptingBaseClass(hookableLifetimeObject, type);