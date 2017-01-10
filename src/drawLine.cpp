#include "drawLine.h"

using namespace mage;

drawLine::drawLine()
{
}

drawLine::drawLine(pointF position, point2F endOffset)
	: transformableLine(position, endOffset)
{
}

drawRecipe drawLine::generateDrawRecipe()
{
	states.shaderUpdate();

	return drawRecipe({ triangle(
		vertex(getTransformedPoints()[0], point2F(), tint1),
		vertex(getTransformedPoints()[1], point2F(), tint2)
	) }, getRealPosition().z, states);
}

#include "scriptingEngine.h"
MAGE_DeclareScriptingType(drawLine);
MAGE_DeclareScriptingBaseClass(transformable, drawLine);
MAGE_DeclareScriptingBaseClass(transformableLine, drawLine);
MAGE_DeclareScriptingBaseClass(renderable, drawLine);
MAGE_DeclareScriptingConstructor(drawLine(), "drawLine");
MAGE_DeclareScriptingConstructor(drawLine(lineF&), "drawLine");
MAGE_DeclareScriptingFunction(&drawLine::tint1, "tint1");
MAGE_DeclareScriptingFunction(&drawLine::tint2, "tint2");
MAGE_DeclareScriptingFunction(&drawLine::states, "states");