#pragma once

#include "StdAfx.h"
#include "SfmlAfx.h"

#include "prefabs.h"
#include "objBasic.h"
#include "uiBasic.h"
#include "serializable.h"
#include "helpers.h"
#include "resourceGroup.h"
#include "shaders.h"
#include "stringHelpers.h"

namespace mage {

// because we're a header that the Game.h file directly uses, templates in here cannot access theGame() like everything else can.
// we can however access other subsystem headers (i.e. prefabs.h)
// to use stuff we declare 3 functions only used in this file
// they're in a namespace so as not to get in the way
// they're accessable outside the engine because group is a TEMPLATE CLASS, so anything using group will need them too.
// don't use them directly.
namespace groupPrivate {
	MAGEDLL bool compareObjects(std::shared_ptr<basic> i, std::shared_ptr<basic> j);
	MAGEDLL prefabMngr* prefabs();
	MAGEDLL void drawLoadingScreenFrame(std::string text, int prog, int maxProg);
}

// A group is a set of unique game objects (by default basics) of any type that derives the group's type.
// This type MUST be a type of basic. If it isn't you get a load of compile errors that don't really give you any info and it serves you right.

// The group class then will automatically seperate objects into lists of objBasic and uiBasic.
// If the group is set to only take objBasics or uiBasics, this won't really matter but will happen anyway because it doesn't really require optimization.
// You can also easily set up a group that only takes tilemaps, only takes players, etc.

// You can access the main group in se through "state" - gameState is a type of object group.


// base type for holding references to group; can't put anything in it, but can do almost everything else
// most notably can't clear a certain type of object :/
class MAGEDLL groupBase : public virtual serializable, public virtual shadable
{
public:
	virtual void drawWorldObjects(sf::RenderTarget &target, sf::RenderStates states) const;
	virtual void drawUiObjects(sf::RenderTarget &target, sf::RenderStates states) const;

	virtual void sortWorldObjects() = 0;

	virtual std::string serialize() const = 0;
	virtual bool deserialize(std::string map) = 0;

	virtual void preUpdateObjects(sf::Time elapsed) = 0;
	virtual void updateObjects(sf::Time elapsed) = 0;

	virtual std::shared_ptr<basic> getObject(unsigned int index) const = 0;
	virtual std::shared_ptr<objBasic> getWorldObject(unsigned int index) const = 0;
	virtual std::shared_ptr<uiBasic> getUiObject(unsigned int index) const = 0;
	virtual unsigned int getNumObjects() const = 0;
	virtual unsigned int getNumWorldObjects() const = 0;
	virtual unsigned int getNumUiObjects() const = 0;

	virtual void bringToFront(unsigned int index) = 0;
	virtual void sendToBack(unsigned int index) = 0;

	virtual void detach(basic* toRemove) = 0;
	virtual void detach(unsigned int index) = 0;

	virtual void clearObjects() = 0;

	virtual int indexOf(basic* object) const = 0;
	virtual int localIndexOf(basic* object) const = 0;

public:
	bool ignoreIncorporation; // for the editor, I guess
	bool objectListDirty; // lets objects holding a groupBase pointer tell any kind of group to rearrange the world objects

	hook<groupBase*> onPreUpdate;
	hook<groupBase*> onUpdate;

	hook<groupBase*, basic*> onAttach;
	hook<groupBase*, basic*> onDetach;

	// format: sender, recipient
	hook<const groupBase*, const groupBase*> onCombined;
	hook<const groupBase*, const groupBase*> onSet;
	hook<const groupBase*, const groupBase*> onIncorporated;

	/*
	These hooks are on groupBase rather than the template on purpose to save a lot of memory space.
	These hooks cannot access:
	- any of the attach functions
	- the list function
	- the combine and set functions
	- the typed get function
	- findByUiName
	- ..etc

	You can cast the group to get around this
	*/
};

template<class T = basic> class group : public groupBase
{
public:
	group();
	group(std::shared_ptr<resourceGroup> gr); // creates group with saved objects (grabbing them from the resource when asked to and then forgetting the resource that was attached)
													  // you can of course create a group from another group

	group(std::vector<std::shared_ptr<T>> initialObjects);

	// replace the copy constructor and assignment operator with set
	// does same thing but doesn't swap in old memory which slows things down a bit
	// fortunately this isn't an operation where speed is an issue; we can afford not reusing memory
	group(const group<T> &gr);
	group& operator=(const group<T> &gr);

	void sortWorldObjects();

	virtual std::string serialize() const;
	virtual bool deserialize(std::string map);

	virtual void preUpdateObjects(sf::Time elapsed);
	virtual void updateObjects(sf::Time elapsed);

	std::shared_ptr<T> get(unsigned int index) const;
	std::shared_ptr<basic> getObject(unsigned int index) const;
	std::shared_ptr<objBasic> getWorldObject(unsigned int index) const;
	std::shared_ptr<uiBasic> getUiObject(unsigned int index) const;
	unsigned int getNumObjects() const;
	unsigned int getNumWorldObjects() const;
	unsigned int getNumUiObjects() const;

	std::shared_ptr<T> findByUiName(std::string uiName) const;

	void bringToFront(unsigned int index);
	void sendToBack(unsigned int index);

	virtual unsigned int attach(std::shared_ptr<T> toAdd);
	virtual unsigned int attachNew(T* toAdd);
	virtual void detach(basic* toRemove);
	virtual void detach(unsigned int index);

	int indexOf(basic* obj) const;
	int localIndexOf(basic* obj) const;

	virtual void clearObjects();
	template<class T2 = T> void clearObjects();

	std::vector < std::shared_ptr<T> > list() const;
	template<class T2 = T> std::vector < std::shared_ptr<T2> > list() const;

	// these don't have simple calls because they aren't called directly enough
	// these are also not freely usable in SE unless overloads are manually registered
	template<class T2 = T, class Filter = T> void combine(const group<T2> *gIn);
	template<class T2 = T, class Filter = T> void set(const group<T2> *gIn);

private:
	template<class T2 = T, class Filter = T> void incorporate(const group<T2> *gIn);

private:
	std::vector<std::shared_ptr<T>> objectList;

	unsigned int worldObjectStart;
	unsigned int uiObjectStart;
};

typedef group<> Group;
typedef group<objBasic> groupObj;
typedef group<uiBasic> groupUi;

// implementation
template<class T>
inline group<T>::group()
{
	worldObjectStart = 0;
	uiObjectStart = 0;

	objectListDirty = true;
	ignoreIncorporation = false;
}

template<class T>
inline group<T>::group(std::shared_ptr<resourceGroup> gr)
{
	if (!gr)
		return;

	if (!gr->isLoaded()) {
		p::warn("Unloaded mageGroupResource was passed to state instantiation, so nothing was loaded from the resource");
	}
	else {
		set<>(gr->get().get());
	}
}

template<class T>
inline group<T>::group(std::vector<std::shared_ptr<T>> initialObjects)
{
	worldObjectStart = 0;
	uiObjectStart = 0;

	objectListDirty = true;
	ignoreIncorporation = false;

	for (unsigned int i = 0; i < initialObjects.size(); i++) {
		attach(initialObjects[i]);
	}
}

template<class T>
inline group<T>::group(const group<T>& gr)
{
	worldObjectStart = 0;
	uiObjectStart = 0;

	set<>(&gr);
}

template<class T>
inline group<T>& group<T>::operator=(const group<T> &gr) {
	worldObjectStart = 0;
	uiObjectStart = 0;

	set<>(&gr);
	return *this;
}

template<class T>
inline void group<T>::sortWorldObjects()
{
	// note: this ONLY SORTS THE WORLD OBJECTS
	// the dirty flag must be true (automatically set by members being moved) for this to happen
	if (objectListDirty) {
		std::sort(objectList.begin() + worldObjectStart, objectList.begin() + uiObjectStart, &groupPrivate::compareObjects);
		objectListDirty = false;
	}
}

template<class T>
inline std::string group<T>::serialize() const
{
	std::stringstream saveFile;

	// save objects
	for (unsigned int i = 0; i < getNumObjects(); i++) {
		auto obj = getObject(i);

		if (obj->prefabSource != nullptr) {
			saveFile << "OBJECT " << groupPrivate::prefabs()->nameOf(obj->prefabSource) << ";";
			saveFile << obj->serialize();
			saveFile << "@\n";
		}
		else {
			p::warn("No prefab with which to save basic object " + std::to_string(i) + ". Map file may be incorrect.");
		}
	}

	return saveFile.str();
}

template<class T>
inline bool group<T>::deserialize(std::string saveString)
{
	auto things = splitString(saveString, '@', '#');

	unsigned int lines = things.size();

	for (unsigned int line = 0; line < lines; line++)
	{
		std::string sLine = things[line];

		// there is an initial word in save file line.
		// this defines why the line is there.
		auto spaceSplit = splitStringAtFirst(sLine);

		if (spaceSplit.size() < 2) {
			// skip this line
			p::info("Ignoring wrong space format/blank section at line " + std::to_string(line));
			continue;
		}

		std::string type = spaceSplit[0];
		std::string sData = spaceSplit[1];

		if (type == "OBJECT") {
			// format:
			// OBJECT [prefab];[property1]=[value1],[property2]=[value2],...

			auto data2 = splitStringAtFirst(sData, ';', '"');

			if (data2.size() < 2) {
				p::info("Ignoring invalid object data at line " + std::to_string(line));
				continue;
			}

			std::string pre = data2[0];
			std::string values = data2[1];

			std::shared_ptr<T> n = std::dynamic_pointer_cast<T>(groupPrivate::prefabs()->newInstance(pre));
			if (!n) {
				p::info("Unable to add prefab to " + std::string(typeid(T).name()) + " group: " + pre);
				continue;
			}

			attach(n);

			n->deserialize(values);
		}

		groupPrivate::drawLoadingScreenFrame("Loading objects", line, lines);
	}

	return true;
}

template<class T>
inline void group<T>::preUpdateObjects(sf::Time elapsed)
{
	for (unsigned int i = 0; i < objectList.size(); i++) {
		auto obj = objectList[i]; // an object will now ALWAYS exist while it is being updated.
		obj->preUpdate(elapsed);
	}
}

template<class T>
inline void group<T>::updateObjects(sf::Time elapsed)
{
	for (unsigned int i = 0; i < objectList.size(); i++) {
		auto obj = objectList[i];
		obj->update(elapsed);
	}
}

template<class T>
inline std::shared_ptr<T> group<T>::get(unsigned int index) const
{
	if (index > objectList.size())
		return nullptr;

	return objectList.at(index);
}

template<class T>
inline std::shared_ptr<basic> group<T>::getObject(unsigned int index) const
{
	if (index > objectList.size())
		return nullptr;

	return objectList.at(index);
}

template<class T>
inline std::shared_ptr<objBasic> group<T>::getWorldObject(unsigned int index) const
{
	if (index >= uiObjectStart)
		return nullptr;

	return std::dynamic_pointer_cast<objBasic>(getObject(worldObjectStart + index));
}

template<class T>
inline std::shared_ptr<uiBasic> group<T>::getUiObject(unsigned int index) const
{
	return std::dynamic_pointer_cast<uiBasic>(getObject(uiObjectStart + index));
}

template<class T>
inline unsigned int group<T>::getNumObjects() const
{
	return objectList.size();
}

template<class T>
inline unsigned int group<T>::getNumWorldObjects() const
{
	return uiObjectStart - worldObjectStart;
}

template<class T>
inline unsigned int group<T>::getNumUiObjects() const
{
	return objectList.size() - uiObjectStart;
}

template<class T>
inline std::shared_ptr<T> group<T>::findByUiName(std::string uiName) const
{
	for (unsigned int i = 0; i < objectList.size(); i++) {
		if (objectList[i]->uiName == uiName) {
			return objectList[i];
		}
	}

	return nullptr;
}

template<class T>
inline void group<T>::bringToFront(unsigned int index)
{
	if (!(index < objectList.size())) {
		return;
	}

	auto tmp = objectList[index];

	// the system can do this for us
	detach(tmp.get());
	attach(tmp);
}

template<class T>
inline void group<T>::sendToBack(unsigned int index)
{
	if (!(index < objectList.size())) {
		return;
	}

	auto tmp = objectList[index];

	detach(tmp.get());

	// which of our sublists will tmp be in?
	if (index > uiObjectStart) {
		// tmp is UI
		objectList.insert(objectList.begin() + uiObjectStart, tmp);
	}
	else if (index > worldObjectStart) {
		// tmp is OBJ
		objectList.insert(objectList.begin() + worldObjectStart, tmp);

		uiObjectStart++;
	}
	else {
		// tmp is BASIC
		objectList.insert(objectList.begin(), tmp);

		uiObjectStart++;
		worldObjectStart++;
	}
}

template<class T>
inline unsigned int group<T>::attach(std::shared_ptr<T> toAdd)
{
	// sanity
	if (!toAdd) {
		p::fatal("Attempting to attach an object that does not exist to a group.");
		return 0;
	}

	if (toAdd->gr) {
		toAdd->gr->detach(toAdd.get());
	}

	toAdd->gr = this;

	auto obj = dynamic_cast<objBasic*>(toAdd.get());
	auto ui = dynamic_cast<uiBasic*>(toAdd.get());

	onAttach.notify(this, toAdd.get());

	if (obj) {
		// insert into the object subsection of the list.
		objectList.insert(objectList.begin() + uiObjectStart, toAdd);
		uiObjectStart++; // shift the ui object list by one.

		return uiObjectStart - 1;
	}
	else if (ui) {
		// insert at the end of the list.
		objectList.push_back(toAdd);

		return objectList.size() - 1;
	}
	else {
		// insert at the start of the list.
		objectList.insert(objectList.begin(), toAdd);
		worldObjectStart++;

		return 0;
	}
}

template<class T>
inline unsigned int group<T>::attachNew(T * toAdd)
{
	return attach(std::shared_ptr<T>(toAdd));
}

template<class T>
inline void group<T>::detach(basic * toRemove)
{
	T* compatibleType = dynamic_cast<T*>(toRemove);

	if(compatibleType)
		detach(indexOf(compatibleType));
}

template<class T>
inline void group<T>::detach(unsigned int index)
{
	if (index < worldObjectStart) {
		worldObjectStart--;
	}

	if (index < uiObjectStart) {
		uiObjectStart--;
	}

	onDetach.notify(this, objectList[index].get());

	objectList.at(index)->gr = nullptr;
	objectList.erase(objectList.begin() + index);
}

template<class T>
inline int group<T>::indexOf(basic * obj) const
{
	for (unsigned int i = 0; i < objectList.size(); i++) {
		if (objectList[i].get() == obj) {
			return i;
		}
	}

	return -1;
}

template<class T>
inline int group<T>::localIndexOf(basic * obj) const
{
	int ind = indexOf(obj);

	auto wo = dynamic_cast<objBasic*>(obj);
	auto uio = dynamic_cast<uiBasic*>(obj);

	if (ind != -1) {
		if (wo) {
			ind -= worldObjectStart;
		}
		else if (uio) {
			ind -= uiObjectStart;
		}
	}

	return ind;
}

template<class T>
inline void group<T>::clearObjects()
{
	objectList.clear();

	worldObjectStart = 0;
	uiObjectStart = 0;
}

template<class T>
inline std::vector<std::shared_ptr<T>> group<T>::list() const
{
	return objectList;
}

template<class T>
template<class T2>
inline std::vector<std::shared_ptr<T2>> group<T>::list() const
{
	std::vector<std::shared_ptr<T2>> retList;

	for (unsigned int i = 0; i < objectList.size(); i++) {
		std::shared_ptr<T2> cast = std::dynamic_pointer_cast<T2>(objectList[i]);

		if (cast) {
			retList.push_back(cast);
		}
	}

	return retList;
}

template<class T>
template<class T2, class Filter>
inline void group<T>::combine(const group<T2> *gIn)
{
	if (ignoreIncorporation) return;

	gIn->onCombined.notify(gIn, this);
	incorporate<T2, Filter>(gIn);
}

template<class T>
template<class T2, class Filter>
inline void group<T>::set(const group<T2> *gIn)
{
	if (ignoreIncorporation) return;

	gIn->onSet.notify(gIn, this);
	clearObjects<Filter>();
	incorporate<T2, Filter>(gIn);
}

template<class T>
template<class T2, class Filter>
inline void group<T>::incorporate(const group<T2>* gIn)
{
	if (gIn == nullptr)
		return;

	gIn->onIncorporated.notify(gIn, this);

	// add everything from the group<T2> class that matches the type Filter
	std::vector<std::shared_ptr<Filter>> oList = gIn->list<Filter>();

	for (unsigned int i = 0; i < oList.size(); i++) {
		if (!oList[i]->isCloneSafe())
			p::fatal("Object was not clone safe");

		auto clone = oList[i]->clone();
		attachNew(clone); // the template will not compile if the Filter isn't a compatible type so this is fine
	}
}

template<class T>
template<class T2>
inline void group<T>::clearObjects()
{
	for (int i = 0; i < (int)objectList.size(); i++) {
		auto cast = std::dynamic_pointer_cast<T2>(objectList[i]);

		if (cast) {
			detach(i);
			i--;
		}
	}
}

} // namespace mage

// group types
// these all have to be custom because commas in the template list can really mess stuff up
#define DeclareScriptingGroup(heldType, name) \
DeclareScriptingCustom(user_type<mage::group<heldType>>(), name) \
DeclareScriptingCustom(constructor<mage::group<heldType>()>(), name); \
DeclareScriptingCustom(constructor<mage::group<heldType>(std::shared_ptr<mage::resourceGroup>)>(), name); \
DeclareScriptingCustom(constructor<mage::group<heldType>(std::vector<std::shared_ptr<heldType>>)>(), name); \
DeclareScriptingCustom(base_class<mage::groupBase, mage::group<heldType>>()); \
DeclareScriptingCustom(base_class<mage::serializable, mage::group<heldType>>()); \
DeclareScriptingCustom(base_class<mage::shadable, mage::group<heldType>>()); \
DeclareScriptingCustom(fun(&mage::group<heldType>::clearObjects<mage::objBasic>), "clearWorldObjects"); \
DeclareScriptingCustom(fun(&mage::group<heldType>::clearObjects<mage::uiBasic>), "clearUiObjects"); \
DeclareScriptingCustom(fun(&mage::group<heldType>::findByUiName), "findByUiName"); \
DeclareScriptingCustom(fun(&mage::group<heldType>::get), "get"); \
DeclareScriptingCustom(fun(&mage::group<heldType>::indexOf), "indexOf"); \
DeclareScriptingCustom(fun(&mage::group<heldType>::localIndexOf), "localIndexOf"); \
DeclareScriptingCustom(fun(&mage::group<heldType>::combine<>), "combine"); \
DeclareScriptingCustom(fun(&mage::group<heldType>::set<>), "set"); \
DeclareScriptingCustom(fun(&mage::group<heldType>::attach), "attach"); \
DeclareScriptingCopyOperator(mage::group<heldType>); \
DeclareScriptingListableShared(heldType, STRING(heldType)"ShVector");\
DeclareScriptingCastingFunction("to_" name, mage::groupBase, mage::group<heldType>); // broke my rule here but it seems to be fine -c
