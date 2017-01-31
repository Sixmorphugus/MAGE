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

void batchRenderer::renderSingleChunk(sf::RenderTarget& target, renderChunk& chunk) const
{
	// do the draw
	auto vList = chunk.getSfVertexList();

	// sanity
	if (vList.size() == 0) {
		return;
	}

	target.draw(&vList[0], vList.size(), sf::Triangles, chunk.states.toSf());
}

void batchRenderer::renderFrame(sf::RenderTarget& target)
{
	m_page.display();

	// draw each chunk quickly
	for (unsigned int i = 0; i < m_frameChunks.size(); i++) {
		renderSingleChunk(target, m_frameChunks[i]);
	}

	// housekeeping
	for (int i = 0; i < (int)m_frameChunks.size(); i++) {
		if (m_frameChunks[i].isEmpty()) {
			m_frameChunks.erase(m_frameChunks.begin() + i);
		}
	}
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

point2F batchRenderer::texturePagePosition(std::shared_ptr<resourceTexture> res)
{
	if (!textureIsInPage(res)) {
		return point2U(m_page.getSize()).convertAxis<float>();
	}

	return m_pagePositions[res.get()].convertAxis<float>();
}

bool batchRenderer::pushPageTexture(std::shared_ptr<resourceTexture> res)
{
	if (!textureFitsPage(res, true))
		return false;

	if (textureIsInPage(res)) 
		return true;
	
	// paste the texture into the batchRenderer's page
	point2U nextPasteLocation(m_currentPageTextureCol, m_nextPageTexturePos);

	// use a sprite to draw the texture into the page
	sf::Sprite sfSprite(*res->get());

	sfSprite.setPosition(nextPasteLocation.convertAxis<float>().toSf2());
	m_page.draw(sfSprite);

	m_pagePositions[res.get()] = nextPasteLocation;

	m_nextPageTexturePos += nextPasteLocation.y;

	return true;
}

const sf::Texture& mage::batchRenderer::getPageSfTexture() const
{
	return m_page.getTexture();
}

void batchRenderer::clearPage()
{
	m_currentPageTextureCol = 0;
	m_nextPageTexturePos = 0;
	m_nextPageTextureCol = 0;

	m_pagePositions.clear();
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
	// is the LAST chunk in this list compatible with our renderStates?
	// if we tried others it would work but things would draw out of order.
	// we're trying to preserve the order.

	if (r.getChunk())
		return; // already being rendered by one of the chunks.

	// push children first
	for (unsigned int i = 0; i < r.children.size(); i++) {
		pushFrameRecipe(r.children[i]);
	}

	if (r.triangles.size() == 0)
		return; // no triangles, nothing to display

	if (m_frameChunks.size() > 0) {
		renderChunk& lastChunk = m_frameChunks[m_frameChunks.size() - 1];

		if (lastChunk.states != r.states) {
			// insert into new chunk.
			// things become slightly more complicated to keep depth.
			// thankfully, that is now the pushFrameChunk function's problem.
			renderChunk newChunk(r.states);
			newChunk.pushRecipe(r);
			pushFrameChunk(newChunk);
		}
		else {
			lastChunk.pushRecipe(r);
		}
	}
	else {
		// create "chunk 1".
		renderChunk newChunk(r.states);
		newChunk.pushRecipe(r);
		pushFrameChunk(newChunk);
	}
}

void batchRenderer::pushFrameRenderable(renderable& r)
{
	if(r.getIsVisible())
		pushFrameRecipe(*r.getDrawRecipe());
}

void batchRenderer::pushFrameChunk(renderChunk& chunk)
{
	//m_frameChunks.push_back(chunk); (you wish)

	for (unsigned int i = 0; i < m_frameChunks.size(); i++) {
		// two things to deal with:
		// - the issue of having chunks that need to be drawn AFTER the new one (minDepth higher than ours)
		// - the issue of having chunks that need to be drawn AT THE SAME TIME as ours (overlapping)

		// we can assume that the exclusive use of this function to add to the frame chunk list keeps it ordered

		// first issue's easy:
		if (m_frameChunks[i].getMinDepth() >= chunk.getMaxDepth()) {
			// insert BEFORE this chunk.
			m_frameChunks.insert(m_frameChunks.begin() + i, chunk);
			return;
		}

		// second issue's more of a problem
		if (chunk.overlaps(m_frameChunks[i])) {
			auto sp = m_frameChunks[i].splice(chunk);

			m_frameChunks.erase(m_frameChunks.begin() + i);
			
			for (unsigned int j = 0; j < sp.size(); j++) {
				m_frameChunks.push_back(sp[j]);
			}

			return;
		}
	}

	// end of the list
	m_frameChunks.push_back(chunk);
}

// SE
#include "scriptingEngine.h"
MAGE_DeclareScriptingUnlistableType(batchRenderer);
MAGE_DeclareScriptingConstructor(batchRenderer(), "batchRenderer");
MAGE_DeclareScriptingFunction(&batchRenderer::clearFrameChunks, "clearFrameChunks");
MAGE_DeclareScriptingFunction(&batchRenderer::clearPage, "clearPage");
MAGE_DeclareScriptingFunction(&batchRenderer::getNumFrameChunks, "getNumFrameChunks");
MAGE_DeclareScriptingFunction(&batchRenderer::pushFrameChunk, "pushFrameChunk");
MAGE_DeclareScriptingFunction(&batchRenderer::pushFrameRecipe, "pushFrameRecipe");
MAGE_DeclareScriptingFunction(&batchRenderer::pushFrameRenderable, "pushFrameRenderable");
MAGE_DeclareScriptingFunction(&batchRenderer::renderFrame, "renderFrame");
MAGE_DeclareScriptingFunction(&batchRenderer::renderSingleChunk, "renderSingleChunk");
MAGE_DeclareScriptingFunction(&batchRenderer::textureFitsPage, "textureFitsPage");
MAGE_DeclareScriptingFunction([](batchRenderer* r, std::shared_ptr<resourceTexture> tex) {return r->textureFitsPage(tex); }, "textureFitsPage");
MAGE_DeclareScriptingFunction(&batchRenderer::textureIsInPage, "textureIsInPage");
MAGE_DeclareScriptingFunction(&batchRenderer::texturePagePosition, "texturePagePosition");