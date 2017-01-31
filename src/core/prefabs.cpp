#include "prefabs.h"
#include "scene.h"

using namespace mage;

struct prefabDefault {
	prefabDefault() {
		name = "";
		pre = nullptr;
	}

	prefabDefault(std::string n, std::shared_ptr<prefab> p) {
		name = n;
		pre = p;
	}

	std::string name;
	std::shared_ptr<prefab> pre;
};

std::map<size_t, prefabDefault> prefabDefaultsHC;

prefab::prefab()
{
	m_templateObject = nullptr;
}

prefab::prefab(std::shared_ptr<gmo> clonable, std::vector<std::string> t)
{
	setTemplate(clonable);
	tags = t;
}

prefab::prefab(const prefab & p)
{
	setTemplate(p.m_templateObject);
	tags = p.tags;
}

prefab& prefab::operator=(const prefab& p) {
	setTemplate(p.m_templateObject);
	tags = p.tags;

	return *this;
}

void prefab::setTemplate(std::shared_ptr<gmo> clonable)
{
	m_templateObject = clonable;
}

std::shared_ptr<gmo> prefab::copyTemplate()
{
	if (m_templateObject) {
		return std::make_shared<gmo>(*m_templateObject);
	}
	else {
		p::warn("newInstance was called on a prefab that is uninitialized!");
		return nullptr;
	}
}

std::string mage::prefab::getName()
{
	return m_name;
}

// prefab manager
prefabMngr::prefabMngr()
{
	// use hardcoded defaults as defaults
	for (auto i = prefabDefaultsHC.begin(); i != prefabDefaultsHC.end(); i++)
	{
		auto pr = std::make_shared<prefab>(*i->second.pre);

		std::string name = i->second.name;

		pr->tags.push_back("default");
		pr->tags.push_back("hc");

		add(name, pr); // CLONE the hardcoded default entry.
		prefabDefaults[i->first] = name;
	}
}

void prefabMngr::add(std::string name, std::shared_ptr<prefab> input)
{
	if (exists(name)) {
		p::warn("Prefab already exists with name \"" + name + "\" - replacing");
	}

	prefabMap[name] = input;
	prefabMap[name]->m_name = name;

	p::info("Registered prefab \"" + name + "\"");
}

bool prefabMngr::exists(std::string name)
{
	return prefabMap.count(name) != 0;
}

std::string prefabMngr::nameOf(std::shared_ptr<prefab> input)
{
	// now we must iterate
	for (auto i = prefabMap.begin(); i != prefabMap.end(); i++) {
		if (i->second == input) {
			return i->first;
		}
	}

	return "";
}

std::shared_ptr<prefab> prefabMngr::get(std::string name)
{
	if (exists(name))
		return prefabMap[name];
	else
		return nullptr;
}

std::shared_ptr<prefab> prefabMngr::getByIndex(unsigned int index)
{
	auto it = prefabMap.begin();

	for (unsigned int i = 0; i < index; i++) {
		it++;
	}

	return it->second;
}

unsigned int prefabMngr::getCount()
{
	return prefabMap.size();
}

std::vector<std::shared_ptr<prefab>> prefabMngr::list(std::string tag)
{
	std::vector<std::shared_ptr<prefab>> results;

	// now we must iterate
	for (auto i = prefabMap.begin(); i != prefabMap.end(); i++) {
		if (i->second->hasTag(tag)) {
			results.push_back(i->second);
		}
	}

	return results;
}

std::shared_ptr<gmo> prefabMngr::newInstance(std::string name, scene* attachTo)
{
	if (exists(name)) {
		std::shared_ptr<gmo> ni = prefabMap[name]->copyTemplate();
		ni->m_prefabSource = prefabMap[name];

		if (attachTo)
			attachTo->attach(ni);

		return ni;
	}

	return nullptr;
}

std::shared_ptr<prefab> prefabMngr::getDefaultPrefab(const std::type_info& typeId)
{
	return get(getDefaultPrefabName(typeId));
}

std::string prefabMngr::getDefaultPrefabName(const std::type_info& typeId)
{
	if (prefabDefaults.count(typeId.hash_code())) {
		return prefabDefaults[typeId.hash_code()];
	}

	return "";
}

void prefabMngr::setDefaultPrefab(const std::type_info& typeId, std::string name)
{
	if (exists(name)) {
		if (prefabDefaults[typeId.hash_code()] != "") {
			auto t = get(prefabDefaults[typeId.hash_code()])->tags;
			t.erase(std::remove(t.begin(), t.end(), "default"), t.end());
		}

		get(name)->tags.push_back("default");
		prefabDefaults[typeId.hash_code()] = name;
	}
}

// SE Binding
#include "scriptingEngine.h"

using namespace chaiscript;

// for prefab
MAGE_DeclareScriptingType(prefab);
MAGE_DeclareScriptingBaseClass(taggable, prefab);
MAGE_DeclareScriptingConstructor(prefab(std::shared_ptr<gmo> refersTo, std::vector<std::string> strList), "prefab");
MAGE_DeclareScriptingCustom(fun([](std::shared_ptr<gmo> refersTo) { return prefab(refersTo); }), "prefab"); // simplified constructor
MAGE_DeclareScriptingConstructor(prefab(), "prefab"); // very simplified constructor
MAGE_DeclareScriptingFunction(&prefab::copyTemplate, "copyTemplate");
MAGE_DeclareScriptingFunction(&prefab::setTemplate, "setTemplate");
MAGE_DeclareScriptingFunction(&prefab::getName, "getName");

// for prefabMngr
MAGE_DeclareScriptingType(prefabMngr);
MAGE_DeclareScriptingFunction(&prefabMngr::add, "add");
MAGE_DeclareScriptingFunction(&prefabMngr::exists, "exists");
MAGE_DeclareScriptingFunction(&prefabMngr::get, "get");
MAGE_DeclareScriptingFunction(&prefabMngr::getByIndex, "getByIndex");
MAGE_DeclareScriptingFunction(&prefabMngr::getCount, "getCount");
MAGE_DeclareScriptingFunction(&prefabMngr::list, "list");
MAGE_DeclareScriptingFunction(&prefabMngr::nameOf, "nameOf");
MAGE_DeclareScriptingFunction(&prefabMngr::newInstance, "newInstance");
MAGE_DeclareScriptingFunction([](prefabMngr &pm, std::string instName) { return pm.newInstance(instName); }, "newInstance");

// declaration of prefabs (internal)
mage::specialPrefabRegistration::specialPrefabRegistration(const std::type_info& typeinfo, std::shared_ptr<prefab> input, std::string name)
{
	prefabDefaultsHC[typeinfo.hash_code()] = prefabDefault(name, input);
}

mage::specialPrefabRegistration::specialPrefabRegistration(const std::type_info& typeinfo, const std::type_info& typeinfo2, std::string name)
{
	prefabDefaultsHC[typeinfo.hash_code()] = prefabDefaultsHC[typeinfo2.hash_code()];
	prefabDefaultsHC[typeinfo.hash_code()].name = name;
}
