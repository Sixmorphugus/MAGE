#pragma once

// mage::gmo (gameObject)
// -------------
// Represents an object in a scene.
// This object is updated each and every tick.
// -------------
// Category: Core
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "hookableLifetimeObject.h"
#include "transformableObject.h"
#include "namable.h"
#include "renderable.h"
#include "scene.h"

namespace mage {

class scene;
class prefab;
class interval;
class resourceSoundBuffer;
class gmo;

class MAGEDLL gmoComponent :
	public renderable
{
public:
	gmoComponent();

	virtual void preUpdate(interval elapsedSinceLastUpdate);
	virtual void update(interval elapsedSinceLastUpdate);

	gmo* getParent();

protected:
	virtual renderRecipe generateDrawRecipe();

public:
	hook<gmoComponent*, interval> onPreUpdate;
	hook<gmoComponent*, interval> onUpdate;

private:
	gmo* m_gmo;
	friend class gmo;

public:
	MAGE_DeclareSerializationList(
		MAGE_SerializedBase(renderable)
	);
};

class MAGEDLL gmo final:
	public renderable, // produces a drawRecipe
	public renamable, // all game objects can have a name
	public transformableObject, // all game objects are (basically) transformable
	public hookableLifetimeObject // onCreated + onDestroyed
{
public:
	gmo();
	gmo(const pointF& position);
	gmo(const pointF& position, const pointF& size);
	gmo(const gmo& cp);

	void init();

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
	void removeComponent(std::shared_ptr<gmoComponent> rComp);
	void removeComponent(unsigned int id);
	int indexOfComponent(std::shared_ptr<gmoComponent> newComp) const;
	std::shared_ptr<gmoComponent> getComponent(unsigned int id);
	unsigned int getNumComponents() const;
public:
	hook<gmo*, interval> onPreUpdate;
	hook<gmo*, interval> onUpdate;

protected:
	virtual renderRecipe generateDrawRecipe();
	void ownChildren();

private:
	void copyFrom(const gmo& cp);

	std::shared_ptr<prefab> m_prefabSource;
	std::vector<std::shared_ptr<gmoComponent>> m_components;
	scene* m_scene;
	bool m_respectPixelGrid;

	friend class scene;
	friend class prefabMngr;

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			MAGE_SerializedBase(renderable),
			MAGE_SerializedBase(renamable),
			MAGE_SerializedBase(transformableObject),
			MAGE_SerializedNVP("components", m_components),
			MAGE_SerializedNVP("respectPixelGrid", m_respectPixelGrid)
		);

		ownChildren();
	}
};

}

#define MAGE_DeclareScriptingGmoTypeG(type)\
MAGE_DeclareScriptingType(type);\
MAGE_DeclareScriptingBaseClass(namable, type);\
MAGE_DeclareScriptingBaseClass(renamable, type);\
MAGE_DeclareScriptingBaseClass(transformable, type);\
MAGE_DeclareScriptingBaseClass(transformableBox, type);\
MAGE_DeclareScriptingBaseClass(transformableObject, type);\
MAGE_DeclareScriptingBaseClass(renderable, type);\
MAGE_DeclareScriptingBaseClass(hookableLifetimeObject, type);\
MAGE_DeclareScriptingSerializable(type, cereal::JSONInputArchive);

#define MAGE_DeclareScriptingGmoType(type)\
MAGE_DeclareScriptingGmoTypeG(type)\
MAGE_DeclareScriptingBaseClass(gmo, type);