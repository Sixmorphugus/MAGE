#include "objPhysics.h"
#include "Game.h"
#include "maths.h"

#include "resourceTexture.h"

#include "shaders.h"

using namespace mage;

objPhysics::objPhysics(float posX, float posY, textureData sprite)
	: objBasic(posX, posY, sprite)
{
	init();
}

void objPhysics::init()
{
	density = 0.1f;
	weight = 1.f;
}

void objPhysics::preUpdate(sf::Time elapsed)
{
	objBasic::preUpdate(elapsed);
}

void objPhysics::update(sf::Time elapsed)
{
	objBasic::update(elapsed);

	// velocity
	if (density > 1.f)
		density = 1.f; // NOT A GOOD IDEA TO HAVE DRAG OVER 1.f

	if (density < 0.f)
		density = 0.f; // NOT A GOOD IDEA TO HAVE DRAG UNDER 0.f

	float drag = 1.f - density;

	if ((velocity.x != 0 || velocity.y != 0)) {
		collision cDat = resolveMovement(velocity.x, velocity.y);

		if (cDat.hitX || cDat.hitY) {
			impact(cDat);
		}

		velocity.x *= drag;
		velocity.y *= drag;
	}
}

void objPhysics::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	applyShader(states, getTexSizeF());

	// we can use the delta number and the velocity to "predict" where this object will be next.
	sf::Sprite fakeSprite(sfSprite);

	fakeSprite.move(velocity.x * theGame()->getDelta(), velocity.y * theGame()->getDelta());

	if (sfSprite.getTexture())
		target.draw(fakeSprite, states);
}

void objPhysics::impact(collision cDat)
{
	if (cDat.main == this) {
		// we hit something
		for (unsigned int i = 0; i < cDat.involved.size(); i++) {
			objBasic* ob = cDat.involved[i].get();
			objPhysics* po = dynamic_cast<objPhysics*>(ob);

			if (po) {
				po->impact(cDat); // notify any physics objects we hit
			}
			else {
				// solid object, stop dead
				velocity.x = 0.f;
				velocity.y = 0.f;
			}
		}
	}
	else {
		objPhysics* po = dynamic_cast<objPhysics*>(cDat.main);

		float devisor = 2.f * weight;

		velocity.x += po->velocity.x / devisor;
		velocity.y += po->velocity.y / devisor;

		po->velocity.x -= po->velocity.x / devisor;
		po->velocity.y -= po->velocity.y / devisor;

		onImpact.notify(this, po);
		po->onImpact.notify(po, this);
	}

	// derive to do more
}

void objPhysics::impulse(sf::Vector2f position, float force, float radius)
{
	// calculate distance and directional angle
	float dir = directionToVector(getCenter(), position);
	float dis = distanceBetweenVectors(getCenter(), position);

	float d = dis / radius;
	float f = force / d;

	// move away
	velocity += generateVector(dir, f);

	onImpulse.notify(this);
}

void objPhysics::registerProperties()
{
	objBasic::registerProperties();

	registerProperty("xVel", prop(velocity.x));
	registerProperty("yVel", prop(velocity.y));

	registerProperty("drag", prop(density));

	registerProperty("weight", prop(weight));
}

// SE
using namespace chaiscript;

MAGE_DeclareScriptingCustom(user_type<objPhysics>(), "objPhysics");
MAGE_DeclareScriptingCustom(base_class<objBasic, objPhysics>());
MAGE_DeclareScriptingBaseClass(basic, objPhysics);
MAGE_DeclareScriptingBaseClass(sf::Transformable, objPhysics);
MAGE_DeclareScriptingCopyOperator(objPhysics);
MAGE_DeclareScriptingCustom(constructor<objPhysics(float, float, basic::textureData)>(), "physicsObject");
MAGE_DeclareScriptingCustom(fun(&objPhysics::density), "density");
MAGE_DeclareScriptingCustom(fun(&objPhysics::impact), "impact");
MAGE_DeclareScriptingCustom(fun(&objPhysics::impulse), "impulse");
MAGE_DeclareScriptingCustom(fun(&objPhysics::velocity), "velocity");
MAGE_DeclareScriptingCustom(fun(&objPhysics::weight), "weight");