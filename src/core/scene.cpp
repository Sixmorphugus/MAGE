#include "scene.h"

#include "helpers.h"
#include "Game.h"

#include "gmo.h"
#include "prefabs.h"

using namespace mage;

// implementation
scene::scene()
{
	m_objectListDirty = true;
	ignoreIncorporation = false;
}

scene::scene(std::vector<std::shared_ptr<gmo>> initialObjects)
{
	m_objectListDirty = true;
	ignoreIncorporation = false;

	for (unsigned int i = 0; i < initialObjects.size(); i++) {
		attach(initialObjects[i]);
	}
}

scene::scene(const scene& gr)
{
	set<>(&gr);
}

scene& scene::operator=(const scene &gr) {
	set<>(&gr);
	return *this;
}

void scene::preUpdateObjects(interval elapsed)
{
	for (unsigned int i = 0; i < m_objectList.size(); i++) {
		auto obj = m_objectList[i]; // an object will now ALWAYS exist while it is being updated.
		obj->preUpdate(elapsed);
	}
}

void scene::updateObjects(interval elapsed)
{
	for (unsigned int i = 0; i < m_objectList.size(); i++) {
		auto obj = m_objectList[i];
		obj->update(elapsed);
	}
}

std::shared_ptr<gmo> scene::get(unsigned int index) const
{
	if (index > m_objectList.size())
		return nullptr;

	return m_objectList.at(index);
}

std::vector<std::shared_ptr<gmo>> scene::objectList() const
{
	return m_objectList;
}

unsigned int scene::getNumObjects() const
{
	return m_objectList.size();
}

void scene::bringToFront(unsigned int index)
{
	if (!(index < m_objectList.size())) {
		return;
	}

	auto tmp = m_objectList[index];

	// the system can do this for us
	detach(tmp.get());
	attach(tmp);
}

void scene::sendToBack(unsigned int index)
{
	if (!(index < m_objectList.size())) {
		return;
	}

	auto tmp = m_objectList[index];

	detach(tmp.get());
	attachBack(tmp);
}

unsigned int scene::attach(std::shared_ptr<gmo> toAdd)
{
	// sanity
	if (!toAdd) {
		p::fatal("Attempting to attach an object that does not exist to a scene.");
		return 0;
	}

	if (toAdd->m_scene) {
		toAdd->m_scene->detach(toAdd.get());
	}

	toAdd->m_scene = this;

	onAttach.notify(this, toAdd.get());

	// insert at the end of the list.
	m_objectList.push_back(toAdd);

	m_objectListDirty = true;

	return 0;
}

unsigned int scene::attachBack(std::shared_ptr<gmo> toAdd)
{
	// sanity
	if (!toAdd) {
		p::fatal("Attempting to attach an object that does not exist to a scene.");
		return 0;
	}

	if (toAdd->m_scene) {
		toAdd->m_scene->detach(toAdd.get());
	}

	toAdd->m_scene = this;

	onAttach.notify(this, toAdd.get());

	// insert at the start of the list.
	m_objectList.insert(m_objectList.begin(), toAdd);

	m_objectListDirty = true;

	return 0;
}

void scene::detach(gmo * toRemove)
{
	detach(indexOf(toRemove));
}

void scene::detach(unsigned int index)
{
	onDetach.notify(this, m_objectList[index].get());

	m_objectList.at(index)->m_scene = nullptr;
	m_objectList.erase(m_objectList.begin() + index);
}

int scene::indexOf(gmo* obj) const
{
	for (unsigned int i = 0; i < m_objectList.size(); i++) {
		if (m_objectList[i].get() == obj) {
			return i;
		}
	}

	return -1;
}

unsigned int mage::scene::indexOfName(std::string uiName) const
{
	for (unsigned int i = 0; i < m_objectList.size(); i++) {
		if (m_objectList[i].get()->getName() == uiName) {
			return i;
		}
	}

	return -1;
}

void scene::clearObjects()
{
	m_objectList.clear();
}

std::vector<std::shared_ptr<gmo>> scene::list() const
{
	return m_objectList;
}

// binding
#include "scriptingEngine.h"

// sceneBase
using namespace chaiscript;

MAGE_DeclareScriptingType(scene);
MAGE_DeclareScriptingConstructor(scene(), "scene");
MAGE_DeclareScriptingConstructor(scene(const scene&), "scene");
MAGE_DeclareScriptingCopyOperator(scene);
MAGE_DeclareScriptingSerializable(scene, cereal::JSONInputArchive);
MAGE_DeclareScriptingFunction(&scene::bringToFront, "bringToFront");
MAGE_DeclareScriptingCustom(fun<void, scene, unsigned int>(&scene::detach), "detach");
MAGE_DeclareScriptingCustom(fun<void, scene, gmo*>(&scene::detach), "detach");
MAGE_DeclareScriptingFunction(&scene::getNumObjects, "getNumObjects");
MAGE_DeclareScriptingFunction(&scene::get, "getObject");
MAGE_DeclareScriptingFunction(&scene::preUpdateObjects, "preUpdateObjects");
MAGE_DeclareScriptingFunction(&scene::updateObjects, "updateObjects");
MAGE_DeclareScriptingFunction(&scene::onPreUpdate, "onPreUpdate");
MAGE_DeclareScriptingFunction(&scene::onUpdate, "onUpdate");
MAGE_DeclareScriptingFunction(&scene::onAttach, "onAttach");
MAGE_DeclareScriptingFunction(&scene::onDetach, "onDetach");
MAGE_DeclareScriptingFunction(&scene::onSet, "onSet");
MAGE_DeclareScriptingFunction(&scene::onCombined, "onCombined");
MAGE_DeclareScriptingFunction(&scene::onIncorporated, "onIncorporated");

MAGE_DeclareScriptingHook("sceneBaseHook", scene*);
MAGE_DeclareScriptingHook("sceneBaseObjectHook", scene*, basic*);
MAGE_DeclareScriptingHook("sceneBaseIntegrateHook", const scene*, const scene*);