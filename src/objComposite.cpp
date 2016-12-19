#include "objComposite.h"

#include "Game.h"
#include "viewObj.h"
#include "maths.h"

using namespace mage;

objComposite::objComposite(float x, float y, std::vector<std::shared_ptr<objBasic>> initialChildList)
	: objSpriteless(x, y), groupObj(initialChildList)
{
}

void objComposite::preUpdate(sf::Time elapsed) {
	sortWorldObjects(); // only for this type of composite
	preUpdateObjects(elapsed);
}

void objComposite::update(sf::Time elapsed) {
	updateObjects(elapsed);
}

void objComposite::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	// this uses an interesting little hack to draw relative to the object.
	auto wv = theGame()->worldCamera;
	auto pwv = wv->getCenter();

	wv->setPosition(sf::Vector2f(0, 0));

	drawWorldObjects(target, states);

	wv->setCenter(pwv);
}

std::string objComposite::serialize() {
	std::string us = objSpriteless::serialize();
	std::string objsInside = strReplace(groupObj::serialize(), "\n", "\n\t");

	return us + "#\n" + objsInside + "#";
}

bool objComposite::deserialize(std::string data) {
	// WE are the object tacked onto the front of the string.
	// our OBJECTS are between two #s, seperated as usual by @s.
	// we need to isolate all of this information and pass it to the relevant deserialization functions.
	// the easiest way to do this is split on #.

	auto split = splitString(data, '#', '#', 3); // 3 expectations: us, our objects, and an empty space.

	int suc = 2;

	suc -= objSpriteless::deserialize(split[0]);
	suc -= groupObj::deserialize(split[1]);

	return !suc;
}

sf::Vector2f objComposite::getSize() const {
	// calculate size
	std::vector<sf::FloatRect> boundBoxes;

	for (unsigned int i = 0; i < getNumObjects(); i++) {
		boundBoxes.push_back(get(i)->getMainBounds());
	}

	sf::FloatRect boundingRect = boxIn<float>(boundBoxes);

	return sf::Vector2f(boundingRect.width, boundingRect.height);
}

sf::FloatRect objComposite::getBounds() const {
	// get the object's "main boxes" without taking collision boxes into account yet - makes sure individual objects that make up the composite are taken in into account too
	auto startBounds = getMainBounds();
	auto rectsCopy = collisionBoxes;

	rectsCopy.push_back(startBounds);

	// find the boxIn
	return boxIn(rectsCopy);
}

// se
#include "scriptingEngine.h"

using namespace chaiscript;

DeclareScriptingCustom(user_type<objComposite>(), "objComposite");
DeclareScriptingBaseClass(basic, objComposite);
DeclareScriptingBaseClass(serializable, objComposite);
DeclareScriptingBaseClass(sf::Transformable, objComposite);
DeclareScriptingBaseClass(shadable, objComposite);
DeclareScriptingBaseClass(objSpriteless, objComposite);
DeclareScriptingBaseClass(objBasic, objComposite);
DeclareScriptingBaseClass(groupObj, objComposite);
DeclareScriptingBaseClass(groupBase, objComposite);
DeclareScriptingCastingFunction("to_" STRING(type), basic, objComposite);
