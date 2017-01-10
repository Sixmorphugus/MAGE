#include "drawBox.h"

using namespace mage;

drawBox::drawBox()
{
}

drawBox::drawBox(pointF& position)
	: transformableBox(position)
{
}

drawBox::drawBox(pointF & position, pointF & size)
	: transformableBox(position, size)
{
}

renderRecipe drawBox::generateDrawRecipe()
{
	// calculate coords
	auto coord1 = getTransformedPoints()[0];
	auto coord2 = getTransformedPoints()[1];
	auto coord3 = getTransformedPoints()[2];
	auto coord4 = getTransformedPoints()[3];

	// calculate triangles
	triangle tri1(
		vertex(coord1, texCoord1, tint1),
		vertex(coord2, texCoord2, tint2),
		vertex(coord3, texCoord3, tint3)
	);

	triangle tri2(
		vertex(coord2, texCoord2, tint2),
		vertex(coord4, texCoord4, tint4),
		vertex(coord3, texCoord3, tint3)
	);

	states.shaderUpdate();
	return renderRecipe({ tri1, tri2 }, getRealPosition().z, states);
}

void drawBox::setFullTint(colour & t)
{
	tint1 = t;
	tint2 = t;
	tint3 = t;
	tint4 = t;
}

#include "scriptingEngine.h"
MAGE_DeclareScriptingType(drawBox);
MAGE_DeclareScriptingBaseClass(transformableBox, drawBox);
MAGE_DeclareScriptingBaseClass(transformable, drawBox);
MAGE_DeclareScriptingBaseClass(renderable, drawBox);
MAGE_DeclareScriptingConstructor(drawBox(), "drawBox");
MAGE_DeclareScriptingConstructor(drawBox(lineF&), "drawBox");
MAGE_DeclareScriptingFunction(&drawBox::setFullTint, "setFullTint");
MAGE_DeclareScriptingFunction(&drawBox::tint1, "tint1");
MAGE_DeclareScriptingFunction(&drawBox::tint2, "tint2");
MAGE_DeclareScriptingFunction(&drawBox::tint3, "tint3");
MAGE_DeclareScriptingFunction(&drawBox::tint4, "tint4");
MAGE_DeclareScriptingFunction(&drawBox::texCoord1, "texCoord1");
MAGE_DeclareScriptingFunction(&drawBox::texCoord2, "texCoord2");
MAGE_DeclareScriptingFunction(&drawBox::texCoord3, "texCoord3");
MAGE_DeclareScriptingFunction(&drawBox::texCoord4, "texCoord4");
MAGE_DeclareScriptingFunction(&drawBox::states, "states");