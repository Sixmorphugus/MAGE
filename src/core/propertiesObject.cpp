#include "propertiesObject.h"

using namespace mage;

propBase::propBase(std::string name)
	: namable(name)
{
	m_dataInstance = nullptr;
	m_whatIsThis = nullptr;
}

void propBase::setHidden(bool hidden)
{
	m_hidden = hidden;
	onSetHidden.notify(this);
}

bool propBase::isHidden() const
{
	return m_hidden;
}

const propBase* propBase::getDataInstance() const
{
	if (m_dataInstance)
		return m_dataInstance;
	else
		return this;
}

propertiesObject::propertiesObject()
{
}

propertiesObject::~propertiesObject()
{
	// quickly go through the safety list
	// this is why propertiesObjects can take a while to destruct if indirectly messed with a lot.
	for (unsigned int i = 0; i < m_safetyList.size(); i++) {
		if (!m_safetyList[i].expired()) {
			// this object that refers to us will still exist once we're gone.
			// make sure it can't try to do anything to the memory where we were before.
			auto lck = m_safetyList[i].lock();

			lck->m_whatIsThis = nullptr;
			lck->m_dataInstance = nullptr;
		}
	}
}

unsigned int propertiesObject::getNumProperties() const
{
	return m_props.size();
}

std::shared_ptr<propBase> propertiesObject::getPropertyData(unsigned int prop) const
{
	if (m_props.size() < prop) {
		return nullptr;
	}

	// in the case of property data we can simply return a reference to the property and we're fine - a propBase can't affect any specific propertiesObject alone.
	return m_props[prop];
}

std::shared_ptr<propBase> propertiesObject::getPropertyData(std::string prop) const
{
	for (unsigned int i = 0; i < m_props.size(); i++) {
		if (m_props[i]->getName() == prop) {
			return getPropertyData(i);
		}
	}

	return nullptr;
}

int propertiesObject::indexOfProperty(std::shared_ptr<propBase> prop) const
{
	for (unsigned int i = 0; i < m_props.size(); i++) {
		if (m_props[i].get() == prop->getDataInstance()) {
			return i;
		}
	}

	return -1;
}

std::vector<std::string> propertiesObject::getPropertyList() const
{
	std::vector<std::string> propNames;

	for (unsigned int i = 0; i < m_props.size(); i++) {
		propNames.push_back(m_props[i]->getName());
	}

	return propNames;
}

std::string propertiesObject::serialize() const
{
	std::stringstream saveFile;

	auto pList = getPropertyList();
	for (unsigned int j = 0; j < pList.size(); j++) {
		if (getPropertyData(pList[j])->isHidden()) {
			continue;
		}

		saveFile << pList[j] << "=\"" << getPropertyData(pList[j])->instPropStringGet(*this) << "\",";
	}

	return saveFile.str();
}

bool propertiesObject::deserialize(std::string data)
{
	// load properties!
	auto valueStrings = splitString(data, ',', '"');

	for (unsigned int i = 0; i < valueStrings.size(); i++) {
		auto pair = splitString(valueStrings[i], '=', '"');

		if (pair.size() != 2)
			continue;

		if (getPropertyData(pair[0]))
			getPropertyData(pair[0])->instPropStringSet(*this, strFilter(pair[1], "\""));
	}

	return true;
}

unsigned int propertiesObject::addProperty(std::shared_ptr<propBase> newProperty)
{
	unsigned int thisPropIndex = getNumProperties();
	m_props.push_back(newProperty);

	onAddProperty.notify(this, thisPropIndex);

	return thisPropIndex;
}

void propertiesObject::removeProperty(unsigned int index)
{
	m_props.erase(m_props.begin() + index);
	onRemoveProperty.notify(this, index);
}

void propertiesObject::safetyListCleanup()
{
	for (int i = 0; i < (int)m_safetyList.size(); i++) {
		if (m_safetyList[i].expired()) {
			m_safetyList.erase(m_safetyList.begin() + i);
			i--;
		}
	}
}

// The public SE binding (groan)
#include "scriptingEngine.h"
using namespace chaiscript;

MAGE_DeclareScriptingTypeNamed(propBase, "propertiesObjectProperty");
MAGE_DeclareScriptingBaseClass(namable, propBase);
MAGE_DeclareScriptingFunction(&propBase::getName, "getName");
MAGE_DeclareScriptingFunction(&propBase::getTypeInfo, "getTypeInfo");
MAGE_DeclareScriptingFunction(&propBase::instPropStringGet, "instPropStringGet");
MAGE_DeclareScriptingFunction(&propBase::instPropStringSet, "instPropStringSet");
MAGE_DeclareScriptingFunction(&propBase::isHidden, "isHidden");
MAGE_DeclareScriptingFunction(&propBase::isReadOnly, "isReadOnly");
MAGE_DeclareScriptingFunction(&propBase::setHidden, "setHidden");
MAGE_DeclareScriptingFunction(&propBase::onSetHidden, "onSetHidden");

MAGE_DeclareScriptingHook("propertiesObjectPropertyHook", propBase*);

MAGE_DeclareScriptingType(propertiesObject);
MAGE_DeclareScriptingBaseClass(serializable, propertiesObject);
MAGE_DeclareScriptingConstructor(propertiesObject(), "propertiesObject");
MAGE_DeclareScriptingConstructor(propertiesObject(const propertiesObject&), "propertiesObject");
MAGE_DeclareScriptingCopyOperator(propertiesObject);
MAGE_DeclareScriptingFunction(&propertiesObject::deserialize, "deserialize");
MAGE_DeclareScriptingFunction(&propertiesObject::getNumProperties, "getNumProperties");
MAGE_DeclareScriptingCustom(fun<std::shared_ptr<propBase>, propertiesObject, unsigned int>(&propertiesObject::getPropertyData), "getPropertyData");
MAGE_DeclareScriptingCustom(fun<std::shared_ptr<propBase>, propertiesObject, std::string>(&propertiesObject::getPropertyData), "getPropertyData");
MAGE_DeclareScriptingFunction(&propertiesObject::getPropertyList, "getPropertyList");
MAGE_DeclareScriptingFunction(&propertiesObject::indexOfProperty, "indexOfProperty");
MAGE_DeclareScriptingFunction(&propertiesObject::onAddProperty, "onAddProperty");
MAGE_DeclareScriptingFunction(&propertiesObject::onRemoveProperty, "onRemoveProperty");

MAGE_DeclareScriptingHook("propertiesObjectHook", propertiesObject*, unsigned int);

MAGE_DeclareScriptingPropertiesObjectPropType(float);
MAGE_DeclareScriptingPropertiesObjectPropType(int);
MAGE_DeclareScriptingPropertiesObjectPropType(unsigned int);
MAGE_DeclareScriptingPropertiesObjectPropType(std::string);

#include "point.h"
MAGE_DeclareScriptingPropertiesObjectPropType(pointF);
MAGE_DeclareScriptingPropertiesObjectPropType(pointI);
MAGE_DeclareScriptingPropertiesObjectPropType(pointU);