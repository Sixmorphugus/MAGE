#pragma once
#include "SfmlAfx.h"
#include "StdAfx.h"
#include "platformDeduction.h"

namespace mage {

// MATHS
// ---------------------------------------------------------------------
MAGEDLL float lerp(float a, float b, float f); // linear interpolation
MAGEDLL int interpolate(int startValue, int endValue, int stepNumber, int lastStepNumber); // linear interpolation with intigers
MAGEDLL int randomInt(int low, int high); // random intiger
MAGEDLL float randomFloat(float low, float high); // random float
MAGEDLL bool isMultiple(float testNum, float of); // is this number a multiple of "of"?
MAGEDLL float clamp(float in, float min, float max); // clamp a number between two values.

} // namespace mage