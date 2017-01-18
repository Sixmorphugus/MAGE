#pragma once

// mage::propertiesObject
// -------------
// A propertiesObject is a serializable object that can be given imaginary "attributes" as strings - these attributes can then be dynamically read and changed through their
// string.
// Attributes are generally just named combinations of get/set functions. In some cases, however, it may be nessicary to directly change an exposed value - hence lambdas must
// be generated as the get/set combo.
// An object can only list its own methods and members as properties. Attempts to list attributes outside the scope of the object itself will fail spectacularly.
// This is to ensure deletion safety.
// The "point" of this is that once an attribute is dynamically reflected and named, it can be serialized and reloaded without ambiguity. It can also be displayed in editors,
// and tweaked by modders and developers in real interval, per object.
// -------------
// Category: Core
// Has Hooks: Yes
// Script-Accessable: Mostly (see comments)
// -------------

#include "serializable.h"
#include "hook.h"

#include "namable.h"

namespace mage {

class propertiesObject;

class MAGEDLL propBase : public namable {
public:
	propBase(std::string name = "");

	void setHidden(bool hidden);
	bool isHidden() const;

	virtual bool isReadOnly() const = 0;
	virtual const std::type_info& getTypeInfo() const = 0;

	virtual std::string instPropStringGet(propertiesObject* inst) = 0;
	virtual void instPropStringSet(propertiesObject* inst, std::string val) = 0;
public:
	hook<propBase*> onSetHidden;

protected:
	const propBase* getDataInstance() const;

protected:
	propertiesObject* m_whatIsThis; // <- set when a new instance is created of prop
	propBase* m_dataInstance;

private:
	bool m_hidden;

	friend class propertiesObject;
};

template<typename T>
class prop : public propBase {
public:
	template <typename T2> static const std::function<T(propertiesObject*)> makeGetFunc(std::function<T(T2*)> originalGetFunc);
	template <typename T2> static std::function<void(propertiesObject*, T)> makeSetFunc(std::function<void(T2*, T)> originalSetFunc);

public:
	prop(std::string name,
		const std::function<T(propertiesObject*)> getFunction,
		std::function<void(propertiesObject*, T)> setFunction = nullptr,
		std::function<T(std::string&)> strConvFunction1 = nullptr,
		std::function<std::string(T&)> strConvFunction2 = nullptr);

	T get() const;
	void set(T newVal);
	const std::type_info& getTypeInfo() const;
	std::string getString() const;
	void setString(std::string str);
	bool isReadOnly() const;

	std::function<T(std::string&)> getStringFromConverter() const;
	std::function<std::string(T&)> getStringToConverter() const;

	std::string instPropStringGet(propertiesObject* inst);
	void instPropStringSet(propertiesObject* inst, std::string val);

public:
	hook<prop<T>*> onSet;

private:
	const std::function<T(propertiesObject*)> m_getFunction;
	std::function<void(propertiesObject*, T)> m_setFunction;

	std::function<T(std::string&)> m_strConv1;
	std::function<std::string(T&)> m_strConv2;
};

// Virtual inheritance means that this can be combined with other things that derive serializable.
class MAGEDLL propertiesObject : public serializable {
public:
	propertiesObject(); // not a constructor-centric object.
	~propertiesObject(); // the safety list check goes here.

	unsigned int getNumProperties() const;

	// modification of properties
	std::shared_ptr<propBase> getPropertyData(unsigned int prop) const;
	template<typename T> std::shared_ptr<prop<T>> createPropertyProxy(unsigned int prop);

	std::shared_ptr<propBase> getPropertyData(std::string prop) const;
	template<typename T> std::shared_ptr<prop<T>> createPropertyProxy(std::string prop);

	int indexOfProperty(std::shared_ptr<propBase> prop) const;

	// helpers
	std::vector<std::string> getPropertyList() const;

	// serialization
	virtual std::string serialize();
	virtual bool deserialize(std::string data);

protected:
	// management of properties
	unsigned int addProperty(std::shared_ptr<propBase> newProperty);
	void removeProperty(unsigned int index);

private:
	void safetyListCleanup();

public:
	hook<propertiesObject*, unsigned int> onAddProperty;
	hook<propertiesObject*, unsigned int> onRemoveProperty;

private:
	std::vector<std::shared_ptr<propBase>> m_props;
	std::vector<std::weak_ptr<propBase>> m_safetyList;
};

template<typename T>
template<typename T2>
inline const std::function<T(propertiesObject*)> prop<T>::makeGetFunc(std::function<T(T2* obj)> originalGetFunc)
{
	return [&](propertiesObject* po) {
		auto castPo = dynamic_cast<T2*>(po);

		if (!castPo)
			return nullptr;

		return originalGetFunc(castPo);
	};
}

template<typename T>
template<typename T2>
inline std::function<void(propertiesObject*, T)> prop<T>::makeSetFunc(std::function<void(T2* obj, T arg)> originalSetFunc)
{
	return [&](propertiesObject* po, T updatedVar) {
		auto castPo = dynamic_cast<T2*>(po);

		if (!castPo)
			return;

		originalSetFunc(castPo, updatedVar);
	};
}

}

#include "propertiesObject.inl"

// se stuff
#define MAGE_DeclareScriptingPropertiesObjectPropType(type)\
MAGE_DeclareScriptingTypeNamed(prop<type>, mage::fixChaiName(STRING(type)) + "Property");\
MAGE_DeclareScriptingBaseClass(propBase, prop<type>);\
MAGE_DeclareScriptingBaseClass(namable, prop<type>);\
MAGE_DeclareScriptingConstructor(prop<type>(std::string name,\
 std::function<type(propertiesObject*)> getFunction,\
 std::function<void(propertiesObject*, type)> setFunction,\
 std::function<type(std::string&)> strConvFunction1,\
 std::function<std::string(type&)> strConvFunction2),\
 mage::fixChaiName(STRING(type)) + "Property");\
MAGE_DeclareScriptingFunction([](std::string name,\
 std::function<type(propertiesObject*)> getFunction) { return prop<type>(name, getFunction); },\
 mage::fixChaiName(STRING(type)) + "Property");\
MAGE_DeclareScriptingFunction([](std::string name,\
 std::function<type(propertiesObject*)> getFunction,\
 std::function<void(propertiesObject*, type)> setFunction) { return prop<type>(name, getFunction, setFunction); },\
 mage::fixChaiName(STRING(type)) + "Property");\
MAGE_DeclareScriptingFunction(&prop<type>::get, "get");\
MAGE_DeclareScriptingFunction(&prop<type>::getString, "getString");\
MAGE_DeclareScriptingFunction(&prop<type>::getStringFromConverter, "getStringFromConverter");\
MAGE_DeclareScriptingFunction(&prop<type>::getStringToConverter, "getStringToConverter");\
MAGE_DeclareScriptingFunction(&prop<type>::set, "set");\
MAGE_DeclareScriptingFunction(&prop<type>::setString, "setString");\
MAGE_DeclareScriptingFunction(&prop<type>::onSet, "onSet");\
MAGE_DeclareScriptingCustom(chaiscript::fun<std::shared_ptr<prop<type>>, propertiesObject, unsigned int>(&propertiesObject::createPropertyProxy<type>), "createPropertyProxy_" + mage::fixChaiName(STRING(type)));\
MAGE_DeclareScriptingCustom(chaiscript::fun<std::shared_ptr<prop<type>>, propertiesObject, std::string>(&propertiesObject::createPropertyProxy<type>), "createPropertyProxy_" + mage::fixChaiName(STRING(type)));\
MAGE_DeclareScriptingHook(mage::fixChaiName(STRING(type)) + "PropertyHook", prop<type>*)