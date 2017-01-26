#include "view.h"

#include "resourceScene.h"
#include "resourceTexture.h"
#include "helpers.h"
#include "gameState.h"
#include "gmo.h"
#include "drawSprite.h"
#include "Game.h"
#include "batchRenderer.h"

using namespace mage;

view::view()
{
	setDefaults();
}

view::view(const pointF& size, std::shared_ptr<scene> gr)
{
	setDefaults();

	setBaseSize(size);
	viewScene = gr;

	resizeCanvas(sf::Vector2u((unsigned int)getScale().x, (unsigned int)getScale().y));
}

view::view(const view& copy)
{
	copyFrom(copy);
}

view& view::operator=(const view& rhs)
{
	copyFrom(rhs);
	return *this;
}

void view::render(sf::RenderTarget& target, const colour& bgCol)
{
	auto lScene = viewScene.lock();

	// respect the pixel grid
	if (respectPixelGrid) {
		pixelLock();
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

	resizeCanvas(target.getSize());

	// we're drawing on the internal texture
	m_internalRT.setView(toSf()); // use this view
	m_internalRT.clear(bgCol.toSf()); // this'll only actually happen in the viewport (i think...)

	timer t1;

	// sfml: 53fps
	// our system: 30fps
	// close enough lol

	if (t_drawTests.size() == 0) {
		for (unsigned int i = 0; i < 100000; i++) {
			auto ds = std::make_shared<drawSprite>(pointF((float)i, (float)i), theGame()->resources->getAs<resourceTexture>("ui_input_keyQ"));

			t_drawTests.push_back(ds);
		}
	}

	// test draws
	for (unsigned int i = 0; i < t_drawTests.size(); i++) {
		theGame()->renderer->pushFrameRenderable(*t_drawTests[i]);
	}

	if(lScene)
		for (unsigned int i = 0; i < lScene->getNumObjects(); i++) {
			auto renObj = lScene->getAs<renderable>(i);

			if (renObj) {
				theGame()->renderer->pushFrameRenderable(*renObj, getBox());
			}
		}

	onRender.notify(this);

	theGame()->renderer->renderFrame(*getCanvas());
	theGame()->renderer->frameCleanup(); // clean up after drawing our content

	// draw the internal RT into the target
	m_internalRT.display();
	sf::Sprite drawSprite(m_internalRT.getTexture());
	
	target.draw(drawSprite, states.toSf());

	p::info(std::to_string(1.f / t1.getElapsedTime().get()));
}

sf::View view::toSf()
{
	auto v = sf::View(getBox().toSf());

	v.setViewport(m_viewport.toSf());
	v.setRotation(getRotation());
	
	return v;
}

void view::resizeCanvas(point2U siz)
{
	if (point2U(m_internalRT.getSize()) != siz) {
		m_internalRT.create(siz.x, siz.y);
	}

	m_internalRT.clear(sf::Color::Transparent);
}

sf::RenderTarget* view::getCanvas()
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

void view::setBaseSize(const pointF& bs)
{
	transformableBox::setBaseSize(bs);
}

void view::setDefaults()
{
	setPosition(pointF(0, 0));
	setScale(MAGE_ZOOMDEFAULT);

	respectPixelGrid = true;
	m_viewport = floatBox(pointF(0.f, 0.f), pointF(1.f, 1.f));
	states = renderStates();
}

void view::copyFrom(const view & from)
{
	viewScene = from.viewScene;
	states = from.states;
	cameraLimits = from.cameraLimits;

	m_viewport = from.m_viewport;
}

// SE Bind
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(view);
MAGE_DeclareScriptingBaseClass(transformable, view);
MAGE_DeclareScriptingBaseClass(transformableBox, view);
MAGE_DeclareScriptingConstructor(view(), "view");
MAGE_DeclareScriptingConstructor(view(const pointF&, std::shared_ptr<scene>), "view");
MAGE_DeclareScriptingConstructor(view(const view&), "view");
MAGE_DeclareScriptingFunction(&view::render, "render");
MAGE_DeclareScriptingFunction(&view::viewScene, "viewScene");
MAGE_DeclareScriptingFunction(&view::respectPixelGrid, "respectPixelGrid");
MAGE_DeclareScriptingFunction(&view::setViewport, "setViewport");
MAGE_DeclareScriptingFunction(&view::getViewport, "getViewport");
MAGE_DeclareScriptingFunction(&view::onRender, "onRender");
MAGE_DeclareScriptingFunction(&view::onViewportResized, "onViewportResized");

MAGE_DeclareScriptingHook("viewHook", view*);