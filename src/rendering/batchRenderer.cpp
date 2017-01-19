#include "batchRenderer.h"

#include "view.h"
#include "resourceTexture.h"
#include "helpers.h"

using namespace mage;

batchRenderer::batchRenderer()
{
	// make the "page" as large as possible.
	if (!m_page.create(sf::Texture::getMaximumSize(), sf::Texture::getMaximumSize()))
		p::warn("Renderer page allocation failed - reverting to legacy draw system");

	// the page is used to draw things very quickly.
	// It basically caches lots of textures at once for you.
	// It is reset every frame.

	// How many textures can actually be fit into the page depends on your graphics card.

	m_currentPageTextureCol = 0;
	m_nextPageTexturePos = 0;
	m_nextPageTextureCol = 0;
}

void batchRenderer::renderSingleChunk(renderChunk& chunk, view& renderView)
{
	sf::RenderTarget* window = renderView.getTarget();

	// use default view if there's none given
	window->setView(renderView.toSf());

	// do the draw
	window->draw(&chunk.getSfVertexList()[0], chunk.getSfVertexList().size(), sf::Triangles, rendererSfState(chunk.states));
}

void batchRenderer::renderFrame(view& renderView)
{
	sf::RenderTarget* window = renderView.getTarget();

	// use default view if there's none given
	window->setView(renderView.toSf());

	// draw each chunk quickly
	for (unsigned int i = 0; i < m_frameChunks.size(); i++) {
		auto vList = m_frameChunks[i].getSfVertexList();

		/*
		for (unsigned int v = 0; v < vList.size(); v++) {
			p::info(std::to_string(vList[v].position.x) + "," + std::to_string(vList[v].position.y));
		}
		*/

		// sanity
		if (vList.size() == 0) {
			continue;
		}

		// do the draw
		window->draw(&vList[0], vList.size(), sf::Triangles, rendererSfState(m_frameChunks[i].states));
	}
}

void batchRenderer::processFrameRecipe(renderRecipe r)
{
	tryRecipePage(r); // try to make this recipe draw faster

	// is the LAST chunk in this list compatible with our renderStates?
	// if we tried others it would work but things would draw out of order.
	// we're trying to preserve the order.
	if (m_frameChunks.size() > 0) {
		renderChunk& lastChunk = m_frameChunks[m_frameChunks.size() - 1];

		if (lastChunk.states != r.states) {
			// insert into new chunk.
			m_frameChunks.push_back(renderChunk(r.states));
			lastChunk = m_frameChunks[m_frameChunks.size() - 1];
		}

		for (unsigned int i = 0; i < r.triangles.size(); i++) {
			lastChunk.pushTriangle(r.triangles[i]);
		}
	}
	else {
		m_frameChunks.push_back(renderChunk(r.states));

		for (unsigned int i = 0; i < r.triangles.size(); i++) {
			m_frameChunks[0].pushTriangle(r.triangles[i]);
		}
	}
}

void batchRenderer::frameCleanup()
{
	clearFrameChunks();
	clearPage();
}

bool batchRenderer::tryRecipePage(renderRecipe & toPage)
{
	// we have been given a draw recipe that:
	// a) has a texture.
	// b) doesn't already use the page.

	// confirm that to be true.
	auto tex = toPage.states.texture.lock();

	if (!tex)
		return false; // we know that if a recipe hasn't got a texture it either CAN'T use the page or already is

	// is this texture already in the page?
	if (!textureIsInPage(tex)) {
		// we need to take the texture that this object uses and try to paste it into the page in its entirety
		// will it fit?
		if (!pushPageTexture(tex)) {
			return false; // we can't page something that won't fit.
		}
	}

	// we're now good to convert this recipe.
	point2U pagePos = texturePagePosition(tex);

	// make the state a "page state"
	toPage.states.texture.reset();
	toPage.states.usePage = true;

	// move the texCoords to their page position.
	std::vector<triangle>& triangles = toPage.triangles;

	for (unsigned int i = 0; i < triangles.size(); i++) {
		triangles[i].vA.texCoords += pagePos.convertAxis<float>();
	}

	// return true.
	return true;
}

bool batchRenderer::textureFitsPage(std::shared_ptr<resourceTexture> res, bool swapCol)
{
	point2U nextPasteLocation(m_currentPageTextureCol, m_nextPageTexturePos);

	if (nextPasteLocation.x >= m_page.getSize().x || nextPasteLocation.y >= m_page.getSize().y) {
		// well shit, it won't fit in the current column
		// will it fit in the next one?

		nextPasteLocation = point2U(m_nextPageTextureCol, 0);

		if (nextPasteLocation.x >= m_page.getSize().x || nextPasteLocation.y >= m_page.getSize().y) {
			// nope.
			return false;
		}
		else if (swapCol) {
			// swap m_currentPageTextureCol to this column.
			m_currentPageTextureCol = m_nextPageTextureCol;
		}
	}

	return true;
}

bool batchRenderer::textureIsInPage(std::shared_ptr<resourceTexture> res)
{
	return m_pagePositions.count(res.get());
}

point2U batchRenderer::texturePagePosition(std::shared_ptr<resourceTexture> res)
{
	if(!textureIsInPage(res))
		return point2U(m_page.getSize());

	return m_pagePositions[res.get()];
}

bool batchRenderer::pushPageTexture(std::shared_ptr<resourceTexture> res)
{
	if (textureIsInPage(res) || !textureFitsPage(res, true))
		return false;

	// paste the texture into the batchRenderer's page
	point2U nextPasteLocation(m_currentPageTextureCol, m_nextPageTexturePos);

	// use a sprite to draw the texture into the page
	sf::Sprite sfSprite(*res->get());

	sfSprite.setPosition(nextPasteLocation.convertAxis<float>().toSf2());
	m_page.draw(sfSprite);

	return true;
}

void batchRenderer::clearPage()
{
	m_currentPageTextureCol = 0;
	m_nextPageTexturePos = 0;
	m_nextPageTextureCol = 0;
}

void batchRenderer::clearFrameChunks()
{
	m_pagePositions.clear();
	m_frameChunks.clear();
}

unsigned int batchRenderer::getNumFrameChunks() const
{
	return m_frameChunks.size();
}

void batchRenderer::pushFrameRecipe(renderRecipe& r)
{
	processFrameRecipe(r);
}

void batchRenderer::pushFrameRenderable(renderable& r)
{
	if(r.getIsVisible())
		pushFrameRecipe(r.getDrawRecipe());
}

void batchRenderer::pushFrameChunk(renderChunk & chunk, float depth)
{
	m_frameChunks.push_back(chunk);
}

sf::RenderStates batchRenderer::rendererSfState(renderStates & states)
{
	auto sfState = states.toSf();

	if (!sfState.texture && states.usePage)
		sfState.texture = &m_page.getTexture();

	return sfState;
}

// SE
#include "scriptingEngine.h"
MAGE_DeclareScriptingUnlistableType(batchRenderer);
MAGE_DeclareScriptingConstructor(batchRenderer(), "batchRenderer");
MAGE_DeclareScriptingFunction(&batchRenderer::clearFrameChunks, "clearFrameChunks");
MAGE_DeclareScriptingFunction(&batchRenderer::clearPage, "clearPage");
MAGE_DeclareScriptingFunction(&batchRenderer::frameCleanup, "frameCleanup");
MAGE_DeclareScriptingFunction(&batchRenderer::getNumFrameChunks, "getNumFrameChunks");
MAGE_DeclareScriptingFunction(&batchRenderer::processFrameRecipe, "processFrameRecipe");
MAGE_DeclareScriptingFunction(&batchRenderer::pushFrameChunk, "pushFrameChunk");
MAGE_DeclareScriptingFunction(&batchRenderer::pushFrameRecipe, "pushFrameRecipe");
MAGE_DeclareScriptingFunction(&batchRenderer::pushFrameRenderable, "pushFrameRenderable");
MAGE_DeclareScriptingFunction(&batchRenderer::renderFrame, "renderFrame");
MAGE_DeclareScriptingFunction(&batchRenderer::renderSingleChunk, "renderSingleChunk");
MAGE_DeclareScriptingFunction(&batchRenderer::textureFitsPage, "textureFitsPage");
MAGE_DeclareScriptingFunction([](batchRenderer* r, std::shared_ptr<resourceTexture> tex) {return r->textureFitsPage(tex); }, "textureFitsPage");
MAGE_DeclareScriptingFunction(&batchRenderer::textureIsInPage, "textureIsInPage");
MAGE_DeclareScriptingFunction(&batchRenderer::texturePagePosition, "texturePagePosition");