#pragma once
#include "basic.h"

namespace mage {

// BASIC WORLD OBJECT
// ----------------------------------------------------------------------------
// Supports depth rendering and has collision handling built in (implemented in the Game class)
// Use for objects that exist in the world.

class MAGEDLL objBasic
	: public basic
{
public:
	struct MAGEDLL collision {
		bool hitX, hitY;

		objBasic* main;
		std::vector<std::shared_ptr<objBasic>> involved;
	};

public:
	objBasic(float x, float y, textureData sd = textureData(nullptr));
	CLONEABLE(objBasic);

	virtual void registerProperties();

	virtual void update(sf::Time elapsed);

	virtual void drawShadows(sf::RenderTarget &target) const;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	virtual void drawCollisionBoxes(sf::RenderTarget &target, sf::RenderStates states) const;

	void setPositionC(sf::Vector2f newPosition);
	void setPositionC(float x, float y);

	objBasic::collision resolveMovement(float byX, float byY);

	void generateCollisionBox(float offL = 0.f, float offR = 0.f, float offT = 0.f, float offB = 0.f);
	void generateCollisionBoxWithPercentages(float offL = 0.f, float offR = 0.f, float offT = 0.f, float offB = 0.f);
	sf::FloatRect transformedCollisionBox(int id) const;

	virtual float getDrawBottom();

	virtual bool operator<(objBasic& right)
	{
		return (getDrawBottom() < right.getDrawBottom());
	}

	void playAudio(std::shared_ptr<resourceSoundBuffer> sound, bool replaceSame = false, bool generatePitch = true);

	void pullCamera(std::shared_ptr<view> toPull, float multiplier = 0.1f);
	void stopPullingCamera();

	virtual sf::FloatRect getBounds() const;
public:
	std::string displayText;

	float drawBottomOffset;
	std::vector<sf::FloatRect> collisionBoxes;
private:
	std::weak_ptr<view> pulledCamera;
	float pulledCameraMultiplier;
};

MAGEDLL bool compareObjects(std::shared_ptr<objBasic> i, std::shared_ptr<objBasic> j);

} //namespace mage