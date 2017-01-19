#pragma once

// mage::drawLine
// -------------
// A line that can be drawn to the screen.
// Is a renderable.
// -------------
// Category: Rendering
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "renderable.h"
#include "transformableLine.h"

namespace mage {

	class MAGEDLL drawLine : public renderable, public transformableLine {
	public: 
		drawLine();
		drawLine(const pointF& position, const point2F& endOffset);

	public:
		colour tint1, tint2;
		renderStates states;

	protected:
		virtual renderRecipe generateDrawRecipe();
	};

}