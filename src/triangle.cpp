#include "triangle.h"

using namespace mage;

triangle::triangle(vertex & v1)
{
	vA = v1;
	vB = v1;
	vC = v1;
}

triangle::triangle(vertex & v1, vertex & v2)
{
	vA = v1;
	vB = v2;
	vC = v2;
}

triangle::triangle(vertex & v1, vertex & v2, vertex & v3)
{
	vA = v1;
	vB = v2;
	vC = v3;
}

// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(triangle);
MAGE_DeclareScriptingConstructor(triangle(vertex&), "triangle");
MAGE_DeclareScriptingConstructor(triangle(vertex&, vertex&), "triangle");
MAGE_DeclareScriptingConstructor(triangle(vertex&, vertex&, vertex&), "triangle");
MAGE_DeclareScriptingCopyOperator(triangle);
MAGE_DeclareScriptingFunction(&triangle::vA, "vA");
MAGE_DeclareScriptingFunction(&triangle::vB, "vB");
MAGE_DeclareScriptingFunction(&triangle::vB, "vC");
