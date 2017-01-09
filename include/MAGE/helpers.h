#pragma once
#include "StdAfx.h"
#include "SfmlAfx.h"

namespace mage {

// Helpers! Making life bearable.

// ENGINE
// ---------------------------------------------------------------------
class basic;

MAGEDLL float randomPitch();

// OTHER
// ---------------------------------------------------------------------
template<typename To, typename From> std::vector<To> dynamicVectorCast(From& from)
{
	std::vector<To> output;

	for (unsigned int i = 0; i < from.size(); i++) {
		output.push_back(dynamic_cast<To>(from[i]));
	}

	return output;
}

template<typename To, typename From> std::vector<std::shared_ptr<To>> dynamicShPVectorCast(From& from)
{
	std::vector<std::shared_ptr<To>> output;

	for (unsigned int i = 0; i < from.size(); i++) {
		output.push_back(std::dynamic_pointer_cast<To>(from[i]));
	}

	return output;
}

template<typename VType> std::vector<VType> collapseVectorVector(std::vector<std::vector<VType>>& vectorVector)
{
	// idk if this increases performance but it's here anyway
	size_t allsize = 0;

	for (unsigned int i = 0; i < vectorVector.size(); i++) {
		allsize += vectorVector[i].size();
	}

	// define output vector, reserve its memory
	std::vector<VType> output;
	output.reserve(allsize);

	for (unsigned int i = 0; i < vectorVector.size(); i++) {
		output.insert(output.end(), vectorVector[i].begin(), vectorVector[i].end());
	}

	return output;
}

} // namespace mage