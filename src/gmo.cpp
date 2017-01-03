#include "gmo.h"

using namespace mage;

gmo::gmo(const pointF & position)
	: transformable(position)
{
	// transformable object properties.
	addProperty(std::make_shared<prop<pointF>>("position",
		prop<pointF>::makeGetFunc<transformable>(&getPosition),
		prop<pointF>::makeSetFunc<transformable>(&setPosition)
		));

	addProperty(std::make_shared<prop<pointF>>("anchor",
		prop<pointF>::makeGetFunc<transformable>(&getAnchor),
		prop<pointF>::makeSetFunc<transformable>(&setAnchor)
		));

	addProperty(std::make_shared<prop<pointF>>("scale",
		prop<pointF>::makeGetFunc<transformable>(&getScale),
		prop<pointF>::makeSetFunc<transformable>(&setScale)
		));

	addProperty(std::make_shared<prop<float>>("rotation",
		prop<float>::makeGetFunc<transformable>(&getRotation),
		prop<float>::makeSetFunc<transformable>(&setRotation)
		));
}