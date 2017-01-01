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
// and tweaked by modders and developers in real time, per object.
// -------------
// Category: Core
// Has Hooks: Yes
// Script-Accessable: Mostly (see comments)
// -------------

#include "serializable.h"
#include "hook.h"

#include "namable.h"

namespace mage {

// Virtual inheritance means that this can be combined with other things that derive serializable.
// This may be premature. I have a feeling, though.
class MAGEDLL propertiesObject : public virtual serializable {
public:
	struct MAGEDLL propBase : public namable {
	public:
		propBase(std::string name = "");

		void setHidden(bool hidden);
		bool isHidden() const;

		virtual bool isReadOnly() const = 0;
		virtual std::type_info& getTypeInfo() const = 0;

		virtual std::string instPropStringGet(propertiesObject* inst) const = 0;
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
	struct prop : public propBase {
	public:
		prop(std::string name, std::function<T(propertiesObject*)> getFunction, 
			std::function<void(propertiesObject*, T)> setFunction = nullptr, 
			std::function<T(std::string&)> strConvFunction1 = nullptr, 
			std::function<std::string(T&)> strConvFunction2 = nullptr);

		T get() const;
		void set(T newVal);
		std::type_info& getTypeInfo() const;
		std::string getString() const;
		void setString(std::string str);
		bool isReadOnly() const;

		std::function<T(std::string&)> getStringFromConverter() const;
		std::function<std::string(T&)> getStringToConverter() const;

		std::string instPropStringGet(propertiesObject* inst) const;
		void instPropStringSet(propertiesObject* inst, std::string val);

	public:
		hook<prop<T>*> onSet;

	private:
		std::function<propType(propertiesObject*)> m_getFunction;
		std::function<void (propertiesObject*, propType)> m_setFunction;

		std::function<T(std::string&)> m_strConv1;
		std::function<std::string(T&)> m_strConv2;
	};

public:
	propertiesObject(); // not a constructor-centric object.
	~propertiesObject(); // the safety list check goes here.

	unsigned int getNumProperties() const;

	// modification of properties
	std::shared_ptr<propBase> getPropertyData(unsigned int prop) const;
	template<typename T> std::shared_ptr<prop<T>> getProperty(unsigned int prop) const;

	std::shared_ptr<propBase> getPropertyData(std::string prop) const;
	template<typename T> std::shared_ptr<prop<T>> getProperty(std::string prop) const;

	int indexOfProperty(std::shared_ptr<propBase> prop) const;

	// helpers
	std::vector<std::string> getPropertyList() const;

	// serialization
	virtual std::string serialize();
	virtual bool deserialize(std::string data);

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

}

#include "propertiesObject.inl"

// se stuff
#define MAGE_DeclareScriptingPropertiesObjectPropType(type)\
MAGE_DeclareScriptingType(propertiesObject::prop<type>, mage::fixChaiName(STRING(type)) + "Property");\
MAGE_DeclareScriptingBaseClass(propertiesObject::propBase, propertiesObject::prop<type>);\
MAGE_DeclareScriptingBaseClass(namable, propertiesObject::prop<type>);\
MAGE_DeclareScriptingConstructor(propertiesObject::prop<type>(std::string name,\
 std::function<type(propertiesObject*)> getFunction,\
 std::function<void(propertiesObject*, type)> setFunction,\
 std::function<type(std::string&)> strConvFunction1,\
 std::function<std::string(type&)> strConvFunction2),\
 mage::fixChaiName(STRING(type)) + "Property");\
MAGE_DeclareScriptingFunction([](std::string name,\
 std::function<type(propertiesObject*)> getFunction) { return propertiesObject::prop<type>(name, getFunction); },\
 mage::fixChaiName(STRING(type)) + "Property");\
MAGE_DeclareScriptingFunction([](std::string name,\
 std::function<type(propertiesObject*)> getFunction,\
 std::function<void(propertiesObject*, type)> setFunction) { return propertiesObject::prop<type>(name, getFunction, setFunction); },\
 mage::fixChaiName(STRING(type)) + "Property");\
MAGE_DeclareScriptingFunction(&propertiesObject::prop<type>::get, "get");\
MAGE_DeclareScriptingFunction(&propertiesObject::prop<type>::getString, "getString");\
MAGE_DeclareScriptingFunction(&propertiesObject::prop<type>::getStringFromConverter, "getStringFromConverter");\
MAGE_DeclareScriptingFunction(&propertiesObject::prop<type>::getStringToConverter, "getStringToConverter");\
MAGE_DeclareScriptingFunction(&propertiesObject::prop<type>::set, "set");\
MAGE_DeclareScriptingFunction(&propertiesObject::prop<type>::setString, "setString");\
MAGE_DeclareScriptingFunction(&propertiesObject::prop<type>::onSet, "onSet");\
MAGE_DeclareScriptingHook(mage::fixChaiName(STRING(type)) + "PropertyHook", propertiesObject::prop<type>*)