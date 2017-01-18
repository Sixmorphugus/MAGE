#include "inputs.h"

namespace mage {

// CONTROLLA (COCO-COLA)
// ---------------------------------------------------
class inputGamePad : public input {
	inputGamePad(int listenToIndex);

	void processEvent(sf::Event evt);
	void update();
	// TODO
};

}