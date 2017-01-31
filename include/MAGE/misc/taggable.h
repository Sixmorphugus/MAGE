#pragma once
#include "platform.h"
#include "serialization.h"

namespace mage {

// basic taggable object
class MAGEDLL taggable {
public:
	taggable();

	bool hasTag(std::string tag);

public:
	std::vector<std::string> tags;

	MAGE_DeclareSerializationList(
		MAGE_SerializedN(tags)
	);
};

} // namespace mage