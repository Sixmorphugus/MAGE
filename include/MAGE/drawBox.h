#pragma once

// mage::drawBox
// -------------
// A box that can be drawn to the screen.
// Is a renderable.
// -------------
// Category: Rendering
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "transformableBox.h"
#include "renderable.h"

namespace mage {

	class MAGEDLL drawBox : public renderable, public transformableBox {
	public:
		drawBox();
		drawBox(pointF& position);
		drawBox(pointF& position, pointF& size);

		void setFullTint(colour& t);

	public:
		colour tint1;
		colour tint2;
		colour tint3;
		colour tint4;

		renderStates states;

		point2F texCoord1;
		point2F texCoord2;
		point2F texCoord3;
		point2F texCoord4;

	protected:
		virtual renderRecipe generateDrawRecipe();
	};

}