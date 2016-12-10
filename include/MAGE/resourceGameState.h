#pragma once
#include "resourceFilepath.h"

namespace mage {

class gameState;

// a gameState is a group with extra attributes like the background color, the music etc.
class MAGEDLL resourceGameState : public resourceFilepath
{
public:
	resourceGameState();
	resourceGameState(std::string filepath);

	bool load();
	void unload();

	bool isNeeded();

	std::shared_ptr<gameState> get();

private:
	std::shared_ptr<gameState> rc;
};

} // namespace mage