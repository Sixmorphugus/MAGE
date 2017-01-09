#include "renderer.h"
#include "view.h"
#include "resourceTexture.h"
#include "helpers.h"

using namespace mage;

renderer::renderer()
{
	// make the "page" as large as possible.
	if (!m_page.create(sf::Texture::getMaximumSize(), sf::Texture::getMaximumSize()))
		p::fatal("Renderer page init failed!");

	// the page is used to draw things very quickly.
	// It basically caches lots of textures at once for you.
	// It is reset every frame.

	// How many textures can actually be fit into the page depends on your graphics card.

	m_currentPageTextureCol = 0;
	m_nextPageTexturePos = 0;
	m_nextPageTextureCol = 0;
}

void renderer::renderSingleChunk(renderChunk& chunk, view& renderView)
{
	sf::RenderTarget* window = renderView.getTarget();

	// use default view if there's none given
	window->setView(renderView.toSf());

	// do the draw
	window->draw(&chunk.getSfVertexList()[0], chunk.getSfVertexList().size(), sf::Triangles, rendererSfState(chunk.states));
}

void renderer::renderFrame(view& renderView)
{
	sf::RenderTarget* window = renderView.getTarget();

	// use default view if there's none given
	window->setView(renderView.toSf());

	auto frameChunks = collapseVectorVector<renderChunk>(m_frameChunks);

	// draw each chunk quickly
	for (unsigned int i = 0; i < frameChunks.size(); i++) {
		auto vList = frameChunks[i].getSfVertexList();
		// sanity
		if (vList.size() == 0)
			continue;

		// do the draw
		window->draw(&vList[0], vList.size(), sf::Triangles, rendererSfState(frameChunks[i].states));
	}
}

void renderer::processFrameRecipe(drawRecipe r)
{
	if (m_frameChunks.size() < r.depth) {
		m_frameChunks.resize(r.depth);
	}

	tryRecipePage(r); // try to make this recipe draw faster

	// is the LAST chunk in this list compatible with our renderStates?
	auto thisList = m_frameChunks[r.depth];
	renderChunk& lastChunk = thisList[thisList.size() - 1];

	if (lastChunk.states == r.states) {

	}
}

void renderer::frameCleanup()
{
	clearFrameChunks();
	clearPage();
}

bool renderer::tryRecipePage(drawRecipe & toPage)
{
	// we have been given a draw recipe that:
	// a) has a texture.
	// b) doesn't already use the page.

	// confirm that to be true.
	auto tex = toPage.states.texture;

	if (!tex)
		return false; // we know that if a recipe hasn't got a texture it CAN'T use the page

	// is this texture already in the page?
	if (!textureIsInPage(tex)) {
		// we need to take the texture that this object uses and try to paste it into the page in its entirety
		// will it fit?
		if (!pushPageTexture(tex)) {
			return false; // we can't page something that won't fit.
		}
	}

	// we're now good to convert this recipe.
	point2U pagePos = texturePagePosition(toPage.states.texture);

	// make the state a "page state"
	toPage.states.texture = nullptr;
	toPage.states.usePage = true;

	// move the texCoords to their page position.
	std::vector<triangle>& triangles = toPage.triangles;

	for (unsigned int i = 0; i < triangles.size(); i++) {
		triangles[i].vA.texCoords += pagePos.convertAxis<float>();
	}

	// return true.
	return true;
}

bool renderer::textureIsInPage(std::shared_ptr<resourceTexture> res)
{
	return m_pagePositions.count(res.get());
}

point2U renderer::texturePagePosition(std::shared_ptr<resourceTexture> res)
{
	if(!textureIsInPage(res))
		return point2U(m_page.getSize());

	return m_pagePositions[res.get()];
}

void renderer::clearPage()
{
	m_currentPageTextureCol = 0;
	m_nextPageTexturePos = 0;
	m_nextPageTextureCol = 0;
}

void renderer::clearFrameChunks()
{
	m_pagePositions.clear();
	m_frameChunks.clear();
}

void renderer::pushFrameRecipe(drawRecipe& r)
{
	processFrameRecipe(r);
}

void renderer::pushFrameRenderable(renderable& r)
{
	if(r.getIsVisible())
		pushFrameRecipe(r.getDrawRecipe());
}

void renderer::pushFrameChunk(renderChunk & chunk, unsigned int depth)
{
	// if you need to use this function you're not using the renderer properly
	if (m_frameChunks.size() < depth) {
		m_frameChunks.resize(depth);
	}

	m_frameChunks[depth].push_back(chunk);
}

sf::RenderStates mage::renderer::rendererSfState(renderStates & states)
{
	auto sfState = states.toSf();

	if (!sfState.texture && states.usePage)
		sfState.texture = &getPage()->getTexture();

	return sfState;
}
