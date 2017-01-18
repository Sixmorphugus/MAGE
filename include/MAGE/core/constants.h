#pragma once
#include "build.h"

// DO NOT INCLUDE THIS IN HEADER FILES.
// PROJECTS THAT INCLUDE MAGE WILL HAVE THESE VARIABLES IF YOU DO.

// modifiable
#define MAGE_TICKRATE 60 // how many times the game will (try to) update a second
#define MAGE_SHOWCOLLISIONBOXES false // whether to show collision boxes or not.
#define MAGE_SHOWFPS false // whether to show the FPS, TPS and other stuff.
#define MAGE_OPENREGISTRY true // whether to label the main registry file so you can see what's saved.
#define MAGE_PLATFORM_LOGINFOALLOWED true
#define MAGE_CRASHWHENEVALFAILS false
#define MAGE_ZOOMDEFAULT 0.5f

// dependent (do not edit)
#define MAGE_BUILDSTRING std::to_string(MAGE_BUILD)
#define MAGE_TICKMS (1000 / MAGE_TICKRATE)
