#pragma once;

// mage::view
// -------------
// A view draws the entire part of the group it is assigned to that will fit onto the screen.
// it wraps sf::View to do this.
// It will do it with a transparent background unless you tell it otherwise
// It can have a shader applied to everything it draws as an entire-view feature.
//
// You can make your own view - just render it manually in onGameDraw or something
// Alternatively attach it to the game state to have the engine do that for you
// -------------
// Category: Rendering
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "scene.h"
#include "shaders.h"

#include "point.h"
#include "transformableBox.h"
#include "renderStates.h"

namespace mage {

class MAGEDLL view : public transformableBox {
public:
	view();
	view(pointF size, std::shared_ptr<scene> gr = nullptr);

	virtual void render(sf::RenderTarget& target, sf::Color bgCol = sf::Color::Transparent);

	sf::View toSf();

	sf::RenderTarget* getTarget();

	floatBox getViewport() const;
	void setViewport(floatBox vp);

	void setZoom(float zoomLevel);

private:
	void setDefaults();

protected:
	sf::RenderTexture m_internalRT;
	void resizeInternalRT(sf::Vector2u siz);

public:
	// things that affect rendering
	std::weak_ptr<scene> group;
	renderStates states;

	pointF zoomZero;
	bool respectPixelGrid;
	floatBox cameraLimits;

	hook<view*> onRender;
	hook<view*> onViewportResized;

private:
	floatBox m_viewport;
};

} // namespace mage
