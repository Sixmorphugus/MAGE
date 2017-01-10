#pragma once

// mage::scene
// -------------
// A scene is a collection of objects that are rendered and updated together.
// It has a renderer for drawing any renderable GMOs that make it up.
// You can derive scene with a gmo to make composite objects.
// -------------
// Category: Maths
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "StdAfx.h"
#include "SfmlAfx.h"

#include "hook.h"

#include "serializable.h"
#include "shaders.h"

namespace mage {

class resourceScene;
class gmo;

class MAGEDLL scene : public serializable
{
public:
	scene();
	scene(std::shared_ptr<resourceScene> gr); // creates scene with saved objects (grabbing them from the resource when asked to and then forgetting the resource that was attached)
											  // you can of course create a group from another group

	scene(std::vector<std::shared_ptr<gmo>> initialObjects);

	// replace the copy constructor and assignment operator with set
	// does same thing but doesn't swap in old memory which slows things down a bit
	// fortunately this isn't an operation where speed is an issue; we can afford not reusing memory
	scene(const scene &gr);
	scene& operator=(const scene &gr);

	virtual std::string serialize() const;
	virtual bool deserialize(std::string map);

	virtual void preUpdateObjects(sf::Time elapsed);
	virtual void updateObjects(sf::Time elapsed);

	template <class T>
	std::shared_ptr<T> getAs(unsigned int index) const;

	std::shared_ptr<gmo> get(unsigned int index) const;

	std::vector<std::shared_ptr<gmo>> objectList() const;

	unsigned int getNumObjects() const;

	void bringToFront(unsigned int index);
	void sendToBack(unsigned int index);

	virtual unsigned int attach(std::shared_ptr<gmo> toAdd);
	virtual unsigned int attachBack(std::shared_ptr<gmo> toAdd);
	virtual void detach(gmo* toRemove);
	virtual void detach(unsigned int index);

	int indexOf(gmo* obj) const;
	unsigned int indexOfName(std::string uiName) const;

	virtual void clearObjects();
	template<class T2 = T> void clearObjects();

	std::vector < std::shared_ptr<gmo> > list() const;
	template<class T2 = T> std::vector < std::shared_ptr<T2> > list() const;

	// these don't have simple calls because they aren't called directly enough
	// these are also not freely usable in SE unless overloads are manually registered
	template<class Filter = gmo> void combine(const scene *gIn);
	template<class Filter = gmo> void set(const scene *gIn);

private:
	template<class Filter = gmo> void incorporate(const scene *gIn);

private:
	std::vector<std::shared_ptr<gmo>> m_objectList;
	bool m_objectListDirty;

public:
	bool ignoreIncorporation; // for the editor, I guess (prevents combine/set being used)

	hook<scene*> onPreUpdate;
	hook<scene*> onUpdate;

	hook<scene*, gmo*> onAttach;
	hook<scene*, gmo*> onDetach;

	// format: sender, recipient
	hook<const scene*, const scene*> onCombined;
	hook<const scene*, const scene*> onSet;
	hook<const scene*, const scene*> onIncorporated;
};

} // namespace mage

#include "scene.inl"