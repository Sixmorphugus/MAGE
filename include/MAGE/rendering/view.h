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
#include "colour.h"

namespace mage {

class renderable;

class MAGEDLL view : public transformableBox {
public:
	view();
	view(const pointF& size, std::shared_ptr<scene> gr = nullptr);
	view(const view& copy);

	view& operator=(const view& rhs);

	virtual void render(sf::RenderTarget& target, const colour& bgCol = colour(sf::Color::Transparent));

	sf::View toSf();

	sf::RenderTarget* getCanvas();

	floatBox getViewport() const;
	void setViewport(floatBox vp);

	void setBaseSize(const pointF& bs);

private:
	void setDefaults();
	void copyFrom(const view& from);

protected:
	sf::RenderTexture m_internalRT;
	void resizeCanvas(point2U siz);

public:
	// things that affect rendering
	std::weak_ptr<scene> viewScene;
	renderStates states;

	bool respectPixelGrid;
	floatBox cameraLimits;

	hook<view*> onRender;
	hook<view*> onViewportResized;

private:
	floatBox m_viewport;
};

} // namespace mage
