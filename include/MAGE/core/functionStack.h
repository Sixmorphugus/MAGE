#pragma once

// mage::functionStack
// -------------
// An object that lets you put functions on top of each other.
// Mainly used in the scripting engine for overriding functionality; acts as a function otherwise.
// -------------
// Category: Core
// Script-Accessable: Yes
// -------------

#include "StdAfx.h"

namespace mage {
template<typename _Returns, typename ... _Args>
class functionStack {
public:
	functionStack();
};
}