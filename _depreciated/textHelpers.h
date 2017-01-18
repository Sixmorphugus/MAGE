#pragma once
#include "SfmlAfx.h"
#include "resourceFont.h"
#include "platform.h"

namespace mage {

// Universal draw functions
MAGEDLL void renderBindPrompt(sf::RenderTarget &target, sf::RenderStates states, int yPosition, std::vector<std::string> keybindNames, std::string caption);
MAGEDLL float renderInfo(sf::RenderTarget &target, sf::RenderStates states, sf::Vector2f screenPos, std::string info);
MAGEDLL float renderInfo(sf::RenderTarget &target, sf::RenderStates states, sf::Vector2f screenPos, std::string 
	info, sf::Color col, bool center = false, unsigned int charSize = 16, float rot = 0.f, std::shared_ptr<resourceFont> customFont = nullptr);

} // namespace mage