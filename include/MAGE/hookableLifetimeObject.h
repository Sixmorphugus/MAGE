#pragma once

// mage::hookableLifetimeObject
// -------------
// A basic hookable that calls onCreated when it is constructed and onDestroyed when it is destructed.
// -------------
// Category: Core
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "hook.h"

namespace mage {

class MAGEDLL hookableLifetimeObject {
	hookableLifetimeObject();
	~hookableLifetimeObject();

public:
	hook<hookableLifetimeObject*> onCreated;
	hook<hookableLifetimeObject*> onDestroyed;
};

}