#pragma once
#include "inputs.h"
#include "point.h"

namespace mage {

// MOUSE
// ---------------------------------------------------
class MAGEDLL inputMouse : public input
{
public:
	inputMouse();

	void processEvent(sf::Event evt);
	void update();

	void scrollReset();
	void reset();

	bool getLeftDown();
	bool getRightDown();
	bool getMiddleDown();

	bool getLeftPressed();
	bool getRightPressed();
	bool getMiddlePressed();

	bool getLeftReleased();
	bool getRightReleased();
	bool getMiddleReleased();

	void setDefaultButtonTextures();

public:
	hook<inputMouse*> onLeftPressed;
	hook<inputMouse*> onRightPressed;
	hook<inputMouse*> onMiddlePressed;
	hook<inputMouse*> onLeftReleased;
	hook<inputMouse*> onRightReleased;
	hook<inputMouse*> onMiddleReleased;

	point2F position;

	point2F lpPosition;
	point2F lrPosition;

	std::string mouseLeftTexture, mouseRightTexture, mouseMiddleTexture;

	int scroll;
	bool justUsed;

private:
	int leftState, rightState, middleState;
};

} // namespace mage