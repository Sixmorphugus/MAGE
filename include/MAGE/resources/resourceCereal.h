#pragma once

// mage::resourceCereal
// -------------
// A file that will handle loading things serialized with Cereal for you.
// Anything that uses cereal will only load when it is needed.
// -------------
// Category: Resources
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "resourceFilepath.h"
#include "serialization.h"

namespace mage {

template<class T, class LoaderArchive = cereal::PortableBinaryInputArchive>
class resourceCereal : public resourceFilepath {
public:
	resourceCereal();
	resourceCereal(std::string filepath);

	bool load();
	void unload();

	T* get();

	virtual bool isNeeded();

private:
	T data;
};
	
}

#include "resourceCereal.inl"

#define MAGE_DeclareScriptingSerializedResource(type, inputArchive, name)\
MAGE_DeclareScriptingCustom(chaiscript::user_type<mage::resourceCereal<type, inputArchive>>(), name);\
MAGE_DeclareScriptingCustom(chaiscript::constructor<mage::resourceCereal<type, inputArchive>()>(), name);\
MAGE_DeclareScriptingCustom(chaiscript::constructor<mage::resourceCereal<type, inputArchive>(std::string)>(), name);\
MAGE_DeclareScriptingCustom(chaiscript::constructor<mage::resourceCereal<type, inputArchive>(const mage::resourceCereal<type, inputArchive>&)>(), name);\
MAGE_DeclareScriptingCustom(chaiscript::fun(&mage::resourceCereal<type, inputArchive>::get), "get");