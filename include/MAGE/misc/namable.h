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

namespace mage {

class MAGEDLL namable {
public:
	namable(std::string name = "");
	virtual std::string getName();

protected:
	std::string m_name;
};

class MAGEDLL renamable : public namable {
public:
	renamable(std::string name = "");
	void setName(std::string name);

public:
	hook<renamable*> onRenamed;
};

}