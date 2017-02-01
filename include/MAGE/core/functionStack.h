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
	functionStack(std::function<_Returns(functionStack&, _Args...)> bottom);
	
	_Returns call(unsigned int stackOffset, _Args...) const;
	unsigned int getNumFunctions() const;
	unsigned int pushFunction(std::function<_Returns(functionStack&, _Args...)> func);
	unsigned int popFunction();

	void clear();

	_Returns operator()(_Args... a) const;
	unsigned int operator+=(std::function<_Returns(functionStack&, _Args...)> func);
	unsigned int operator--();

private:
	std::vector<std::function<_Returns(functionStack&, _Args...)>> m_stack;
	unsigned int m_stackOffset;
};

}

#include "functionStack.inl"