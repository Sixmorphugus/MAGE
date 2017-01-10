#pragma once

// mage:: maths functions
// -------------
// bunch of templated maths functions for doing things the C++ stdlib doesn't define.
// -------------
// Category: Maths
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "SfmlAfx.h"
#include "StdAfx.h"
#include "platformDeduction.h"
#include "point.h"

namespace mage {

enum shapeInit {
	POSANDSIZE,
	CORNERS
};

// MATHS
// ---------------------------------------------------------------------
MAGEDLL float lerp(float a, float b, float f); // linear interpolation
MAGEDLL int interpolate(int startValue, int endValue, int stepNumber, int lastStepNumber); // linear interpolation with intigers
MAGEDLL int randomInt(int low, int high); // random intiger
MAGEDLL float randomFloat(float low, float high); // random float
MAGEDLL bool isMultiple(float testNum, float of); // is this number a multiple of "of"?
MAGEDLL float clamp(float in, float min, float max); // clamp a number between two values.

} // namespace mage