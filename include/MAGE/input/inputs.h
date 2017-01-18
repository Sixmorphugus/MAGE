// Chris's all-fulfilling Input script for SFML
// "Seems legit!"

#pragma once
#include "StdAfx.h"
#include "SfmlAfx.h"
#include "hook.h"

namespace mage {

class MAGEDLL input {
public:
	virtual void processEvent(sf::Event evt) = 0;
	virtual void update() = 0;
};

class MAGEDLL inputManager {
	// TODO:
	// for now, game directly owns all input objects
};

}