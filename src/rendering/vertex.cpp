#include "vertex.h"

using namespace mage;

vertex::vertex()
{}

vertex::vertex(const sf::Vertex& v)
{
	position = point2F(v.position);
	texCoords = point2F(v.texCoords);
	tint = colour(v.color);
}

vertex::vertex(const point2F& sc, const point2F& tc, const colour& t)
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

	return vert;
}

// SE
#include "scriptingEngine.h"

using namespace chaiscript;

MAGE_DeclareScriptingType(vertex);
MAGE_DeclareScriptingConstructor(vertex(), "vertex");
MAGE_DeclareScriptingConstructor(vertex(const point2F&, const point2F&, const colour&), "vertex");
MAGE_DeclareScriptingCopyOperator(vertex);
MAGE_DeclareScriptingFunction(&vertex::position, "position");
MAGE_DeclareScriptingFunction(&vertex::texCoords, "texCoords");
MAGE_DeclareScriptingFunction(&vertex::tint, "tint");
