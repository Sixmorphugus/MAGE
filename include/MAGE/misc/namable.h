#pragma once

// mage::namable
// -------------
// A namable object can be created with a name that can be accessed at any interval.
// A renamable's name can even be changed later on.
// -------------
// Category: Core
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "hook.h"
#include "serialization.h"

namespace mage {

class MAGEDLL namable {
public:
	namable(std::string name = "");
	virtual std::string getName() const;

protected:
	std::string m_name;

public:
	MAGE_DeclareSerializationList(m_name);
};

class MAGEDLL renamable : public namable {
public:
	renamable(std::string name = "");
	void setName(std::string name);

	MAGE_DeclareSerializationList(MAGE_SerializedBase(namable));

public:
	hook<renamable*> onRenamed;
};

}