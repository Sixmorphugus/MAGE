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

namespace mage {

// Virtual inheritance means that this can be combined with other things that derive serializable.
// This may be premature. I have a feeling, though.
class MAGEDLL propertiesObject : public virtual serializable {
public:
	struct propBase {
	public:
		propBase();

		virtual bool isReadOnly() = 0;
		virtual std::type_info& getTypeInfo() = 0;
		virtual std::string toString() = 0;
		virtual void setString(std::string str) = 0;

	protected:
		propertiesObject* m_whatIsThis; // <- set when a new instance is created of prop
		friend class propertiesObject;
	};

	template<typename T>
	struct prop : public propBase {
	public:
		prop(std::function<T(propertiesObject*)> getFunction, 
			std::function<void(propertiesObject*, T)> setFunction = nullptr, 
			std::function<T(std::string&)> strConvFunction1 = nullptr, 
			std::function<std::string(T&)> strConvFunction2 = nullptr);

		T get();
		void set(T newVal);
		std::type_info& getTypeInfo();
		std::string getString();
		void setString(std::string str);
		bool isReadOnly();

		std::function<T(std::string&)> getStringFromConverter();
		std::function<std::string(T&)> getStringToConverter();

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
	propertiesObject(const propertiesObject& copy);
	propertiesObject& operator=(const propertiesObject& copy);

	unsigned int getNumProperties();
	std::string getPropertyName(unsigned int i);

	// modification of properties
	std::shared_ptr<propBase> getProperty(std::string prop);
	template<typename T> std::shared_ptr<prop<T>> getProperty(std::string prop);

	unsigned int indexOfProperty(std::shared_ptr<propBase> prop);
	std::string nameOfProperty(std::shared_ptr<propBase> prop);

	// serialization
	virtual std::string serialize();
	virtual bool deserialize(std::string data);

public:
	hook<propertiesObject*, unsigned int> onAddProperty;
	hook<propertiesObject*, unsigned int> onHideProperty;
	hook<propertiesObject*, unsigned int> onRemoveProperty;

protected:
	unsigned int addProperty(std::shared_ptr<propBase> newProperty);
	void hideProperty(unsigned int prop);
	void removeProperty(unsigned int prop);

private:
	void copyPropertiesObject(const propertiesObject& from);

private:
	std::map<std::string, std::shared_ptr<propBase>> m_props;
};

}

#include "propertiesObject.inl"

// se stuff
#define MAGE_DeclareScriptingPropertiesObjectPropType(type); // to-do