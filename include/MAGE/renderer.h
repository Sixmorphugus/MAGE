#pragma once

// mage::renderer
// -------------
// It draws things.
// The renderer will generally be given renderables (though can also take raw drawRecipes) to batch draw to the window.
// A view is optional for the drawing.
// The renderer garentees that it will organize the chunks so that recipes with the same depth are drawn in roughly the order they were passed in.
// -------------
// Category: Rendering
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "renderable.h"
#include "renderChunk.h"

namespace mage {

class view;

class MAGEDLL renderer {
public:
	renderer();

	void renderSingleChunk(renderChunk& chunk, view& renderView);

	void renderFrame(view& renderView);

	void processFrameRecipe(drawRecipe r);

	void pushFrameRecipe(drawRecipe& r);
	void pushFrameRenderable(renderable& r);
	void pushFrameChunk(renderChunk& chunk, unsigned int depth); // messing around with a render in progress is not something you should be doing, but you do have the option.

	void frameCleanup();

	bool tryRecipePage(drawRecipe& toPage); // tries to make a recipe compatible with a chunk that uses the page by pasting its texture into said page.
	// this fails if the page has no room or not enough.

	bool textureFitsPage(std::shared_ptr<resourceTexture> res);
	bool textureIsInPage(std::shared_ptr<resourceTexture> res);
	point2U texturePagePosition(std::shared_ptr<resourceTexture> res);

	bool pushPageTexture(std::shared_ptr<resourceTexture> res);

	void clearPage();
	void clearFrameChunks();

	unsigned int getNumFrameChunks() const;
	renderChunk* getFrameChunk() const;

	bool pushPageTexture(std::shared_ptr<resourceTexture> tex);

	sf::RenderTexture* getPage() const;
	sf::RenderTarget* getTarget() const;

	void setTarget(sf::RenderTarget* targ);

	sf::RenderStates rendererSfState(renderStates& states); // does the same thing as renderStates::toSf with a small change.

private:
	sf::RenderTexture m_page;

	std::map<resourceTexture*, point2U> m_pagePositions;

	std::vector<std::vector<renderChunk>> m_frameChunks;

	unsigned int m_nextPageTexturePos;
	unsigned int m_nextPageTextureCol;
	unsigned int m_currentPageTextureCol;
};

}