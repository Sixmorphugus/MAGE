#pragma once
#include "uiBasic.h"

namespace mage {

	// menu.
	// A selection menu with a selector character (@ by default).
	class MAGEDLL uiMenu : public uiBasic
	{
	public:
		// defs
		enum alignMode {
			LEFT, // align text to the left (default)
			CENTER, // center each line of text
			RIGHT // align text to the right
		};

		// stuff
		uiMenu(float x, float y);
		uiMenu(float x, float y, std::vector<std::string> opList);

		void update(sf::Time elapsed);
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

		void validateSelected();

		void init();

		bool isMouseOver() { return mouseOver; }
		bool wasDismissed() { return dismissed; }

		//void setSelectorPosition(int op);

		void reset();

	public:
		int selected, superSelected;
		float width;
		bool selectionChanged;
		std::string selectorText;
		alignMode alignMode;
		std::vector<std::string> optionList;
		bool ready;
		bool disabled;
		int maxOptions;
		int optionStart;
		bool noKbd;

		hook<basic*, unsigned int> onSelectionChanged;
		hook<basic*> onDismissed;

	protected:
		sf::Vector2f scrollDragOffset;
		bool dragMode;

		bool mouseOver;
		bool scrollBarGlow;
		bool dismissed; // was esc hit?
		bool skipDown;
		float selectorPos, selectorPos2;
	};

} // namespace mage