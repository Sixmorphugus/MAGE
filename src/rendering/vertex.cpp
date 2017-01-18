#include "vertex.h"

using namespace mage;

vertex::vertex()
{}

vertex::vertex(sf::Vertex & v)
{
	position = point2F(v.position);
	texCoords = point2F(v.texCoords);
	tint = colour(v.color);
}

vertex::vertex(point2F & sc, point2F & tc, colour & t)
{
	position = sc;
	texCoords = tc;
	tint = t;
}

sf::Vertex vertex::toSf()
{
	sf::Vertex vert;

	vert.color = tint.toSf();
	vert.position = position.toSf2();
	vert.texCoords = texCoords.toSf2();
}

// SE
#include "scriptingEngine.h"

using namespace chaiscript;

MAGE_DeclareScriptingType(vertex);
MAGE_DeclareScriptingConstructor(vertex(), "vertex");
MAGE_DeclareScriptingConstructor(vertex(pointF&, pointF&, colour&), "vertex");
MAGE_DeclareScriptingCopyOperator(vertex);
MAGE_DeclareScriptingFunction(&vertex::position, "position");
MAGE_DeclareScriptingFunction(&vertex::texCoords, "texCoords");
MAGE_DeclareScriptingFunction(&vertex::tint, "tint");
