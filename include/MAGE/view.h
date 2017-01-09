#pragma once;

// A view draws the entire part of the group it is assigned to that will fit onto the screen.
// it wraps sf::View to do this.
// It will do it with a transparent background unless you tell it otherwise
// It can have a shader applied to everything it draws as an entire-view feature.

// TODO: A view can have render verteces queued. As rendering finishes, it can draw them in.

// The game has 2 it renders with by default - one for UI and one for the world.
// Both of these are autocreated and configured

// You can make your own view - just render it manually in onGameDraw or something

// unlike most objects cameras don't update independently of rendering - instead, their attributes are managed before every render

#include "scene.h"
#include "shaders.h"

#include "point.h"
#include "transformableBox.h"

namespace mage {

class MAGEDLL view : public transformableBox, public shadable {
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
	std::shared_ptr<resourceShader> shader;

	pointF zoomZero;
	bool respectPixelGrid;
	floatBox cameraLimits;

	hook<view*> onRender;
	hook<view*> onViewportResized;

private:
	floatBox m_viewport;
};

} // namespace mage
