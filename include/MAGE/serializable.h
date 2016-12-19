#pragma once
#include "StdAfx.h"

namespace mage {

// a serializable is an object that can save its state and reload it later.
// a serializable is NOT an object that can save its state and be entirely reconstructed from
// the saved state later.

// this is why you cannot save an object to a file and have it be a prefab.
// you CAN save a prefab to a file, though (as of build 366)

class MAGEDLL serializable {
public:
	serializable();

	virtual std::string serialize();
	virtual bool deserialize(std::string data);

	virtual void saveToFile(std::string file);
	virtual bool loadFromFile(std::string file);
	virtual std::string saveToString(std::string file);
	virtual bool loadFromString(std::string file);
};

} // namespace mage