#include "triangle.h"

#include "platform.h"

using namespace mage;

triangle::triangle()
{
	m_verts.resize(3);
}

triangle::triangle(vertex & v1)
{
	m_verts.resize(3);

	m_verts[0] = v1.toSf();
	m_verts[1] = v1.toSf();
	m_verts[2] = v1.toSf();
}

triangle::triangle(vertex& v1, vertex& v2)
{
	m_verts.resize(3);

	m_verts[0] = v1.toSf();
	m_verts[1] = v2.toSf();
	m_verts[2] = v2.toSf();
}

triangle::triangle(vertex& v1, vertex& v2, vertex& v3)
{
	m_verts.resize(3);

	m_verts[0] = v1.toSf();
	m_verts[1] = v2.toSf();
	m_verts[2] = v3.toSf();
}

void triangle::shiftTexCoords(const point2F& shiftAmnt)
{
	m_verts[0].texCoords += shiftAmnt.toSf2();
	m_verts[1].texCoords += shiftAmnt.toSf2();
	m_verts[2].texCoords += shiftAmnt.toSf2();
}

std::vector<vertex> triangle::getVerteces()
{
	return { m_verts[0], m_verts[1], m_verts[2] };
}

// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(triangle);
MAGE_DeclareScriptingConstructor(triangle(), "triangle");
MAGE_DeclareScriptingConstructor(triangle(vertex&), "triangle");
MAGE_DeclareScriptingConstructor(triangle(vertex&, vertex&), "triangle");
MAGE_DeclareScriptingConstructor(triangle(vertex&, vertex&, vertex&), "triangle");
MAGE_DeclareScriptingFunction(&triangle::getVerteces, "getVerteces");
MAGE_DeclareScriptingCopyOperator(triangle);
