#pragma once

// mage::batchRenderer
// -------------
// It draws things.
// The batchRenderer will generally be given renderables (though can also take raw drawRecipes) to batch draw to the window.
// A view is optional for the drawing.
// The batchRenderer guarantees that it will organize the chunks so that recipes with the same depth are drawn in roughly the order they were passed in.
// -------------
// Category: Rendering
// Has Hooks: Yes
// Script-Accessible: Yes
// -------------

#include "renderable.h"
#include "renderChunk.h"

namespace mage {

class view;

class MAGEDLL batchRenderer {
public:
	batchRenderer();

	void renderSingleChunk(sf::RenderTarget& target, renderChunk& chunk);

	void renderFrame(sf::RenderTarget& target);

	void pushFrameRecipe(renderRecipe& r);
	void pushFrameRenderable(renderable& r);
	void pushFrameChunk(renderChunk& chunk); // messing around with a render in progress is not something you should be doing, but you do have the option.

	bool textureFitsPage(std::shared_ptr<resourceTexture> res, bool swapCol = false); // unless you know how the batchRenderer works you don't need swapCol.
	bool textureIsInPage(std::shared_ptr<resourceTexture> res);
	point2F texturePagePosition(std::shared_ptr<resourceTexture> res);

	bool pushPageTexture(std::shared_ptr<resourceTexture> res);

	const sf::Texture& getPageSfTexture() const;

	void clearPage();
	void clearFrameChunks();

	unsigned int getNumFrameChunks() const;

private:
	sf::RenderTexture m_page;

	std::map<resourceTexture*, point2U> m_pagePositions;

	std::vector<renderChunk> m_frameChunks;

	unsigned int m_nextPageTexturePos;
	unsigned int m_nextPageTextureCol;
	unsigned int m_currentPageTextureCol;
};

}
