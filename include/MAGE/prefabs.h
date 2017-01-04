#pragma once
#include "gmo.h"
#include "taggable.h"

namespace mage {

// class for managing a single saved gmo which is used as a template.
class MAGEDLL prefab : public taggable, public serializable {
public:
	prefab(std::shared_ptr<gmo> clonable, std::vector<std::string> tags = {});

	// "big three" (with default constructor instead of destructor)
	prefab();
	prefab(const prefab& p);
	prefab& operator=(const prefab& p);

	void setTemplate(std::shared_ptr<gmo> clonable);

	std::shared_ptr<gmo> copyTemplate();
	template<typename T> bool castsTo();

	std::string name();

public:
	std::string knownName;

private:
	std::shared_ptr<gmo> templateObject;
};

// class for managing all the saved gmo template objects.
class MAGEDLL prefabMngr {
public:
	prefabMngr();

	void add(std::string name, std::shared_ptr<prefab> input);
	bool exists(std::string name);

	std::string nameOf(std::shared_ptr<prefab> input);

	std::shared_ptr<prefab> get(std::string name);
	std::shared_ptr<prefab> getByIndex(unsigned int index);

	unsigned int getCount();

	std::vector<std::shared_ptr<prefab>> list(std::string tag = "");
	template<typename T> std::vector<std::shared_ptr<prefab>> listType();

	std::shared_ptr<gmo> newInstance(std::string name, scene* attachTo = nullptr);

	// default prefab management (not currently exposed)
	std::shared_ptr<prefab> getDefaultPrefab(const std::type_info& ti);
	std::string getDefaultPrefabName(const std::type_info& ti);
	void setDefaultPrefab(const std::type_info& ti, std::string name);
private:
	std::map<std::string, std::shared_ptr<prefab>> prefabMap;
	std::map<size_t, std::string> prefabDefaults;
};

template<typename T>
inline bool prefab::castsTo()
{
	return std::dynamic_pointer_cast<T>(templateObject) != nullptr; // just try casting
}

template<typename T>
inline std::vector<std::shared_ptr<prefab>> prefabMngr::listType()
{
	std::vector<std::shared_ptr<prefab>> results;

	// now we must iterate
	for (auto i = prefabMap.begin(); i != prefabMap.end(); i++) {
		if (i->second->castsTo<T>()) {
			results.push_back(i->second);
		}
	}

	return results;
}

class MAGEDLL specialPrefabRegistration {
public:
	specialPrefabRegistration(const std::type_info& typeinfo, std::shared_ptr<prefab> input, std::string name = "");
	specialPrefabRegistration(const std::type_info& typeinfo, const std::type_info& typeinfo2, std::string name = "");
};

} // namespace mage

// this creates a default prefab for a type that is used when none is available.
#define MAGE_DeclarePrefabDefault(...) namespace { specialPrefabRegistration UNIQUE_IDENTIFIER(pre)(__VA_ARGS__); }
#define MAGE_DeclarePrefabDefaultObj(type, ...) MAGE_DeclarePrefabDefault(typeid(type), std::make_shared<prefab>(std::make_shared<type>(__VA_ARGS__)), STRING(type))
#define MAGE_DeclarePrefabDefaultMirroring(type, type2) MAGE_DeclarePrefabDefault(typeid(type), typeid(type2), STRING(type))