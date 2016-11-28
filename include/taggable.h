#pragma once
#include "StdAfx.h"

// basic taggable object
class taggable {
public:
	taggable();
	bool hasTag(std::string tag);

public:
	std::vector<std::string> tags;
};