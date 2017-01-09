#include "view.h"

#include "resourceScene.h"
#include "helpers.h"
#include "gameState.h"
#include "gmo.h"

using namespace mage;

view::view()
{
	setDefaults();
}

view::view(pointF size, std::shared_ptr<scene> gr)
{
	setScale(size);
	resizeInternalRT(sf::Vector2u((unsigned int)getScale().x, (unsigned int)getScale().y));

	setDefaults();
	group = gr;
}

void view::render(sf::RenderTarget & target, sf::Color bgCol)
{
	// respect the pixel grid
	if (respectPixelGrid) {
		setPosition(pointF(floor(getPosition().x), floor(getPosition().y)));

		// the size must be the same ratio as the viewport
		// (todo)
	}

	// respect the bounds rectangle unless the camera is too big for it
	if (getScale().x < cameraLimits.size.x && getScale().y < cameraLimits.size.y) {
		// find how far out the camera is
		auto camRect = getBox();

		if (camRect.size.x < cameraLimits.size.x) {
			float over = camRect.size.x - cameraLimits.size.x;
			move(pointF(-over, 0.f));
		}

		if (camRect.size.y < cameraLimits.position.x) {
			float over = camRect.position.y - cameraLimits.position.y;
			move(pointF(0.f, -over));
		}

		if (camRect.position.x + camRect.size.x > cameraLimits.position.x + cameraLimits.size.x) {
			float over = (camRect.size.x + camRect.size.x) - (cameraLimits.size.x + cameraLimits.size.x);
			move(pointF(-over, 0.f));
		}

		if (camRect.position.y + camRect.size.y > cameraLimits.position.y + cameraLimits.size.y) {
			float over = (camRect.position.y + camRect.size.y) - (cameraLimits.position.y + cameraLimits.size.y);
			move(pointF(0.f, -over));
		}
	}

	sf::View windowView = sf::View(floatBox(pointF(0, 0), pointF(target.getSize().x, target.getSize().y)).toSf());
	target.setView(windowView);

	resizeInternalRT(target.getSize());

	// we're drawing on the internal texture
	m_internalRT.setView(toSf()); // use this view

	if (bgCol != sf::Color::Transparent)
		m_internalRT.clear(bgCol); // this'll only actually happen in the viewport (i think...)
	
	onRender.notify(this);

	// TODO: render with the renderer
}

sf::View view::toSf()
{
	auto v = sf::View(getBox().toSf());

	v.setViewport(m_viewport.toSf());
	v.setRotation(getRotation());
	
	return v;
}

void view::resizeInternalRT(sf::Vector2u siz)
{
	if (m_internalRT.getSize() != siz) {
		m_internalRT.create(siz.x, siz.y);
	}

	m_internalRT.clear(sf::Color::Transparent);
}

sf::RenderTarget* view::getTarget()
{
	return &m_internalRT;
}

floatBox view::getViewport() const
{
	return m_viewport;
}

void view::setViewport(floatBox vp)
{
	m_viewport = vp;
	onViewportResized.notify(this);
}

void view::setZoom(float zoomLevel)
{
	setScale(zoomZero * zoomLevel);
}

void view::setDefaults()
{
	zoomZero = getScale();

	setPosition(pointF(0, 0));

	shader = nullptr;
	respectPixelGrid = true;

	setZoom(MAGE_ZOOMDEFAULT);
}

// SE Bind
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(view);
MAGE_DeclareScriptingBaseClass(transformable, view);
MAGE_DeclareScriptingBaseClass(transformableBox, view);
MAGE_DeclareScriptingBaseClass(shadable, view);
MAGE_DeclareScriptingFunction(&view::render, "render");
MAGE_DeclareScriptingFunction(&view::respectPixelGrid, "respectPixelGrid");
MAGE_DeclareScriptingFunction(&view::group, "group");
MAGE_DeclareScriptingFunction(&view::zoomZero, "zoomZero");
MAGE_DeclareScriptingFunction(&view::setViewport, "setViewport");
MAGE_DeclareScriptingFunction(&view::getViewport, "getViewport");
MAGE_DeclareScriptingFunction(&view::onRender, "onRender");
MAGE_DeclareScriptingFunction(&view::onViewportResized, "onRender");

MAGE_DeclareScriptingHook("viewHook", view*);