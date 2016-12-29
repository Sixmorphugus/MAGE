#include "basic.h"

#include "Game.h"
#include "platform.h"
#include "mixer.h"
#include "resources.h"
#include "view.h"
#include "gameState.h"
#include "constants.h"
#include "scriptingEngine.h"
#include "textHelpers.h"
#include "prefabs.h"
#include "resourceTexture.h"
#include "stringHelpers.h"
#include "resourceSoundBuffer.h"
#include "resourceFont.h"
#include "viewObj.h"
#include "maths.h"
#include "scheduleWait.h"

using namespace mage;

// BASIC PROPERTY CLASS
// ----------------------------------------------------------------------------
basic::prop::prop() {
	typ = NA;
}

basic::prop::prop(int &intProp) {
	iRef = &intProp;
	typ = INTPROP;
}

basic::prop::prop(float &floatProp) {
	fRef = &floatProp;
	typ = FLOATPROP;
}

basic::prop::prop(std::string &stringProp) {
	sRef = &stringProp;
	typ = STRINGPROP;
}

basic::prop::prop(bool &boolProp) {
	bRef = &boolProp;
	typ = BOOLPROP;
}

basic::prop::prop(sf::Uint8& colProp) {
	cRef = &colProp;
	typ = COLPROP;
}

void basic::prop::update(int iUpdate) {
	if (typ == INTPROP) {
		// just use the int
		*iRef = iUpdate;
	}
	else if (typ == FLOATPROP) {
		// int to float
		*fRef = (float)iUpdate;
	}
	else if (typ == STRINGPROP) {
		// toString the int
		*sRef = std::to_string(iUpdate);
	}
	else if (typ == BOOLPROP) {
		*bRef = (iUpdate != 0);
	}
	else if (typ == COLPROP) {
		*cRef = ((sf::Uint8) iUpdate);
	}
}

void basic::prop::update(float fUpdate) {
	if (typ == INTPROP) {
		// float to int (values were lost that day)
		*iRef = (int)fUpdate;
	}
	else if (typ == FLOATPROP) {
		// float is float.
		*fRef = fUpdate;
	}
	else if (typ == STRINGPROP) {
		// toString the int
		*sRef = std::to_string(fUpdate);
	}
	else if (typ == BOOLPROP) {
		*bRef = (fUpdate != 0.f);
	}
	else if (typ == COLPROP) {
		*cRef = ((sf::Uint8) fUpdate);
	}
}

void basic::prop::update(std::string sUpdate) {
	if (typ == INTPROP) {
		// atoi() the string.
		*iRef = atoi(sUpdate.c_str());
	}
	else if (typ == FLOATPROP) {
		// atof() the string.
		*fRef = (float)atof(sUpdate.c_str());
	}
	else if (typ == STRINGPROP) {
		*sRef = sUpdate;
	}
	else if (typ == BOOLPROP) {
		*bRef = (sUpdate == "1");
	}
	else if (typ == COLPROP) {
		*cRef = (sf::Uint8)atoi(sUpdate.c_str());
	}
}

std::string basic::prop::read()
{
	if (typ == INTPROP) {
		// int
		return std::to_string(*iRef);
	}
	else if (typ == FLOATPROP) {
		// float
		return std::to_string(*fRef);
	}
	else if (typ == STRINGPROP) {
		// string
		return *sRef;
	}
	else if (typ == BOOLPROP) {
		return std::to_string(*bRef);
	}
	else if (typ == COLPROP) {
		return std::to_string(*cRef);
	}

	return "";
}

// textureData
// ----------------------------------------------------------------------------
basic::textureData::textureData()
{
}

basic::textureData::textureData(std::shared_ptr<resourceTexture> spriteResource)
{
	if(spriteResource)
		resource = spriteResource;

	defaultFrameSize = true;
}

basic::textureData::textureData(std::shared_ptr<resourceTexture> spriteResource, unsigned int frameWidth, unsigned int frameHeight)
{
	if (spriteResource)
		resource = spriteResource;

	defaultFrameSize = false;
	frameSize.x = frameWidth;
	frameSize.y = frameHeight;
}

basic::spriteData::spriteData(basic* owner, textureData &texData)
	: animations(texData.resource.lock())
{
	texture = texData;

	if (!texture.defaultFrameSize) {
		if (texture.resource.expired()) {
			p::warn("Texture resource absent from textureData with frameSize");
		}

		animations.generateFrameRects(texData.frameSize.x, texData.frameSize.y);
	}
}

// BASIC OF BASICS.
// ----------------------------------------------------------------------------
basic::basic(float x, float y, textureData sd)
{
	setPosition(sf::Vector2f(x, y));

	curSprite = -1;

	addSprite(sd);
	setCurrentSprite(0);

	gr = nullptr;
	initializedCalled = false;
	enforcePixelGrid = true;
	moved = false;

	hackRot = 0.f;
	rotCheck = 0.f;

	sfSync();
}

basic::~basic()
{
	onDestroyed.notify(this);
}

basic::basic(const basic & cp)
{
	copyFrom(cp);
}

basic & basic::operator=(basic assignedCopy)
{
	copyFrom(assignedCopy);
	return *this;
}

void basic::copyFrom(const basic & cp)
{
	sprites = cp.sprites;

	sfSprite = cp.sfSprite;
	curSprite = cp.curSprite;

	onCreated = cp.onCreated;
	onPreUpdate = cp.onPreUpdate;
	onUpdate = cp.onUpdate;
	onDestroyed = cp.onDestroyed;
	onDrawn = cp.onDrawn;
	onFirstUpdate = cp.onFirstUpdate;

	prefabSource = cp.prefabSource;
	enforcePixelGrid = cp.enforcePixelGrid;

	hackPos = cp.hackPos;
	hackOrigin = cp.hackOrigin;
	hackRot = cp.hackRot;
	hackScale = cp.hackScale;

	posCheck = cp.posCheck;
	orCheck = cp.orCheck;
	rotCheck = cp.rotCheck;
	scaleCheck = cp.scaleCheck;

	gr = nullptr;
	initializedCalled = false;
	moved = false;

	sfSync();
}

bool basic::isCloneSafe()
{
	return (typeid(*this) == typeid(*clone()));
}

void basic::registerProperties() {
	onCreated.notify(this);

	// legacy x and y
	registerProperty("x", prop(hackPos.x));
	registerProperty("y", prop(hackPos.y));
	deleteProperty("x");
	deleteProperty("y");

	// new X and Y (always at top)
	registerProperty("_X", prop(hackPos.x));
	registerProperty("_Y", prop(hackPos.y));

	registerProperty("originX", prop(hackOrigin.x));
	registerProperty("originY", prop(hackOrigin.y));
	registerProperty("scaleX", prop(hackScale.x));
	registerProperty("scaleY", prop(hackScale.y));
	registerProperty("angle", prop(hackRot));

	registerProperty("uiName", prop(uiName));
	registerProperty("enforcePixelGrid", prop(enforcePixelGrid));
}

void basic::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	onDrawn.notify(this, &target);
	applyShader(states, getTexSizeF());

	target.draw(sfSprite, states);
}

void basic::preUpdate(sf::Time elapsed) {
	moved = false;

	// messy position change tracking
	if (hackPos != posCheck) {
		posCheck = hackPos;
		setPosition(hackPos);

		moved = true;
	}

	if (getPosition() != posCheck) {
		posCheck = getPosition();
		hackPos = getPosition();

		moved = true;
	}

	// messy origin change tracking
	if (hackOrigin != orCheck) {
		orCheck = hackOrigin;
		setOrigin(hackOrigin);

		moved = true;
	}

	if (getOrigin() != orCheck) {
		orCheck = getOrigin();
		hackOrigin = getOrigin();

		moved = true;
	}

	// messy rotation change tracking
	if (hackRot != rotCheck) {
		rotCheck = hackRot;
		setRotation(hackRot);

		enforcePixelGrid = false;
	}

	if (getRotation() != rotCheck) {
		rotCheck = getRotation();
		hackRot = getRotation();
	}

	// messy scale change tracking
	if (hackScale != scaleCheck) {
		scaleCheck = hackScale;
		setScale(hackScale);

		enforcePixelGrid = false;
		moved = true;
	}

	if (getScale() != scaleCheck) {
		scaleCheck = getScale();
		hackScale = getScale();

		moved = true;
	}

	doPixelGridCorrection(false);

	// set sfSprite to the not found image if needed
	if (curSprite < 0) {
		if (theGame()->worldObjectsFrozen) {
			sfSprite = sf::Sprite(*theGame()->resources->getAs<resourceTexture>("ui_notfound")->get().get());
		}
		else {
			sfSprite = sf::Sprite();
		}
	}
	else if (getCurrentSprite()->texture.resource.lock()->isLoaded() && spriteLoadFailed) {
		sfSprite.setTexture(*getCurrentSprite()->texture.resource.lock()->get());
		spriteLoadFailed = false;
	}

	// update transform
	sfSync();

	if (!initializedCalled && !theGame()->worldObjectsFrozen) {
		onFirstUpdate.notify(this);
		initializedCalled = true;
	}
	else if (initializedCalled && theGame()->worldObjectsFrozen) {
		initializedCalled = false; // if the game is frozen and unfrozen the function is called again
	}

	onPreUpdate.notify(this);
}

void basic::update(sf::Time elapsed) {
	onUpdate.notify(this);

	// update animations
	if (!theGame()->worldObjectsFrozen && curSprite > -1)
		getCurrentSprite()->animations.update(sfSprite, elapsed);
}

std::string basic::serialize()
{
	std::stringstream saveFile;

	auto pList = getPropertyList();
	for (unsigned int j = 0; j < pList.size(); j++) {
		if (getProperty(pList[j]).hidden) {
			continue;
		}

		saveFile << pList[j] << "=\"" << getProperty(pList[j]).read() << "\",";
	}

	return saveFile.str();
}

bool basic::deserialize(std::string data)
{
	// load properties!
	auto valueStrings = splitString(data, ',', '"');

	for (unsigned int i = 0; i < valueStrings.size(); i++) {
		auto pair = splitString(valueStrings[i], '=', '"');

		if (pair.size() != 2)
			continue;

		if (isProperty(pair[0]))
			getProperty(pair[0]).update(strFilter(pair[1], "\""));
	}

	return true;
}

unsigned int basic::addSprite(textureData texture) {
	if (texture.resource.expired())
		return -1;

	// add to list
	sprites.push_back(std::make_shared<spriteData>(this, texture));

	return sprites.size() - 1;
}

int basic::getCurrentSpriteIndex() const
{
	return curSprite;
}

std::shared_ptr<basic::spriteData> basic::getCurrentSprite()
{
	if (curSprite < 0) {
		return nullptr;
	}

	return sprites[curSprite];
}

const std::shared_ptr<basic::spriteData> basic::getCurrentSprite() const
{
	if (curSprite < 0) {
		return nullptr;
	}

	return sprites[curSprite];
}

void basic::setCurrentSprite(int spr)
{
	if (sprites.size() <= spr)
		return;

	if (curSprite == spr)
		return;

	curSprite = spr;

	auto texturePointer = sprites[curSprite]->texture.resource.lock();

	if (!texturePointer->isLoaded()) {
		p::warn("Object's texture was set to that of a resource that ISN'T LOADED!");
		spriteLoadFailed = true;
	}
	else {
		sfSprite.setTexture(*texturePointer->get());
		spriteLoadFailed = false;
	}
	
	sprites[curSprite]->animations.setFrame(sfSprite, 0);
}

void basic::replaceCurrentSprite(textureData texture)
{
	if (texture.resource.expired()) {
		p::warn("No valid replacement texture given!");
		return;
	}

	bool ad = false;

	if (curSprite == -1) {
		setCurrentSprite(0);

		if (sprites.size() == 0) {
			sprites.push_back(std::make_shared<spriteData>(this, texture));
			ad = true;
		}
	}

	if(!ad)
		sprites[curSprite] = std::make_shared<spriteData>(this, texture);

	auto ocs = curSprite;
	curSprite = -1;

	setCurrentSprite(ocs);
}

void basic::sfSync()
{
	sfSprite.setPosition(getPosition());
	sfSprite.setRotation(getRotation());
	sfSprite.setScale(getScale());
	sfSprite.setOrigin(getOrigin());
}

void basic::doPixelGridCorrection(bool force)
{
	// force this object to stay aligned? (nullifies rotation + scale)
	if (enforcePixelGrid || force) {
		setPosition(floor(getPosition().x), floor(getPosition().y));
		setRotation(0.f); // pixel locked objects can't rotate
		setScale(1.f, 1.f); // ..or scale
	}
}

void basic::destroy()
{
	if (gr) {
		gr->detach(this);
	}
}

void basic::scheduleDestroy()
{
	theGame()->scheduler->add(std::make_shared<scheduleWait>(sf::seconds(0.f), std::bind(&basic::destroy, this))); // destroy this object as soon as we're not in the middle of other things
}

sf::Vector2f basic::getCenter() const {
	sf::Vector2f pos(sfSprite.getGlobalBounds().left, sfSprite.getGlobalBounds().top);

	pos.x += sfSprite.getGlobalBounds().width / 2.f;
	pos.y += sfSprite.getGlobalBounds().height / 2.f;

	return pos;
}

sf::Vector2u basic::getFrameSize() const
{
	auto cs = getCurrentSprite();

	if (cs)
		return sf::Vector2u(cs->texture.frameSize.x, cs->texture.frameSize.y);
	else
		return sf::Vector2u(sfSprite.getGlobalBounds().width, sfSprite.getGlobalBounds().height); // fallback
}

sf::Vector2f basic::getRealPosition() const
{
	return getPosition() - sf::Vector2f(getOrigin().x * getScale().x, getOrigin().y * getScale().y);
}

sf::Vector2u basic::getTexSize() const
{
	if(sfSprite.getTexture())
		return sfSprite.getTexture()->getSize();
	else {
		return sf::Vector2u(0U, 0U);
	}
}

sf::Vector2f basic::getTexSizeF() const
{
	return sf::Vector2f(getTexSize().x, getTexSize().y);
}

sf::Vector2f basic::getSize() const
{
	return sf::Vector2f(sfSprite.getGlobalBounds().width, sfSprite.getGlobalBounds().height);
}

sf::FloatRect basic::getMainBounds() const
{
	return sf::FloatRect(getRealPosition(), getSize());
}

sf::FloatRect basic::getBounds() const
{
	return getMainBounds();
}

void basic::setRealPosition(sf::Vector2f p)
{
	setPosition(p + getOrigin());
}

std::shared_ptr<basic> basic::shareThis()
{
	if (getGroup()) {
		return getGroup()->getObject(getGroup()->indexOf(this));
	}

	p::fatal("shareThis() was called on a basic that does not yet have shared ownership.");
	return nullptr;
}

void basic::registerProperty(std::string name, prop p)
{
	properties[name] = p;
}

void basic::deleteProperty(std::string name)
{
	for (auto i = properties.begin(); i != properties.end(); i++) {
		if (i->first == name) {
			//properties.erase(i);
			i->second.hidden = true; // property can still be saved but shouldn't be changable
			break;
		}
	}
}

bool basic::isProperty(std::string name)
{
	for (auto i = properties.begin(); i != properties.end(); i++) {
		if (i->first == name) {
			return true;
		}
	}

	return false;
}

basic::prop& basic::getProperty(std::string name)
{
	for (auto i = properties.begin(); i != properties.end(); i++) {
		if (i->first == name) {
			return i->second;
		}
	}

	std::string whatThereIs;

	for (auto i = properties.begin(); i != properties.end(); i++) {
		whatThereIs += i->first + ", ";
	}

	platform::fatal("getProperty: " + name + " isn't defined!\nThe following do exist: " + whatThereIs);

	// code execution never reaches this point but the compiler requires this
	int dum;
	prop* dummyProp = new prop(dum);

	return *dummyProp;
}

std::vector<std::string> basic::getPropertyList()
{
	std::vector<std::string> ls;

	for (auto i = properties.begin(); i != properties.end(); i++) {
		ls.push_back(i->first);
	}

	return ls;
}

basic::prop& basic::operator[](std::string name)
{
	return getProperty(name);
}

std::shared_ptr<prefab> mage::basic::getPrefab()
{
	if (prefabSource)
		return prefabSource;

	const std::type_info& t = typeid(*this);
	return theGame()->prefabs->getDefaultPrefab(t);
}

std::shared_ptr<resourceTexture> basic::getTexResource() const {
	auto cs = getCurrentSprite();

	if (cs) {
		return cs->texture.resource.lock();
	}
	else {
		return nullptr;
	}
}

// SE
using namespace chaiscript;

MAGE_DeclareScriptingCustom(user_type<basic>(), "basic");
MAGE_DeclareScriptingCustom(base_class<sf::Transformable, basic>());
MAGE_DeclareScriptingCustom(base_class<sf::Drawable, basic>());
MAGE_DeclareScriptingCustom(base_class<serializable, basic>());
MAGE_DeclareScriptingCustom(base_class<shadable, basic>());
MAGE_DeclareScriptingCustom(fun(&basic::clone), "clone");
MAGE_DeclareScriptingCustom(fun(&basic::getCenter), "getCenter");
MAGE_DeclareScriptingCustom(fun(&basic::isProperty), "isProperty");
MAGE_DeclareScriptingCustom(fun(&basic::getProperty), "getProperty");
MAGE_DeclareScriptingCustom(fun(&basic::getPropertyList), "getPropertyList");
MAGE_DeclareScriptingCustom(fun<std::shared_ptr<basic::spriteData>, basic>(&basic::getCurrentSprite), "getCurrentSprite");
MAGE_DeclareScriptingCustom(fun(&basic::replaceCurrentSprite), "replaceCurrentSprite");
MAGE_DeclareScriptingCustom(fun(&basic::setCurrentSprite), "setCurrentSprite");
MAGE_DeclareScriptingCustom(fun(&basic::sprites), "sprites");
MAGE_DeclareScriptingCustom(fun(&basic::sfSprite), "sfSprite");
MAGE_DeclareScriptingCustom(fun(&basic::getGroup), "getGroup");
MAGE_DeclareScriptingCustom(fun(&basic::scheduleDestroy), "destroy");
MAGE_DeclareScriptingCustom(fun(&basic::vars), "vars");
MAGE_DeclareScriptingCustom(fun(&basic::getTexResource), "getTexResource");
MAGE_DeclareScriptingCustom(fun(&basic::getFrameSize), "getFrameSize");
MAGE_DeclareScriptingCustom(fun(&basic::getRealPosition), "getRealPosition");
MAGE_DeclareScriptingCustom(fun(&basic::setRealPosition), "setRealPosition");
MAGE_DeclareScriptingCustom(fun(&basic::getTexSize), "getTexSize");
MAGE_DeclareScriptingCustom(fun(&basic::getSize), "getSize");
MAGE_DeclareScriptingCustom(fun(&basic::getBounds), "getMainBounds");
MAGE_DeclareScriptingCustom(fun(&basic::getMainBounds), "getMainBounds");
MAGE_DeclareScriptingCustom(fun(&basic::getPrefabSource), "getPrefabSource");
MAGE_DeclareScriptingCustom(fun(&basic::hasMoved), "hasMoved");
MAGE_DeclareScriptingCopyOperator(basic);

MAGE_DeclareScriptingCustom(user_type<basic::textureData>(), "textureData");
MAGE_DeclareScriptingCustom(constructor<basic::textureData()>(), "textureData");
MAGE_DeclareScriptingCustom(constructor<basic::textureData(std::shared_ptr<resourceTexture>)>(), "textureData");
MAGE_DeclareScriptingCustom(constructor<basic::textureData(std::shared_ptr<resourceTexture>, unsigned int, unsigned int)>(), "textureData");
MAGE_DeclareScriptingCustom(fun(&basic::textureData::defaultFrameSize), "defaultFrameSize");
MAGE_DeclareScriptingCustom(fun(&basic::textureData::resource), "resource");
MAGE_DeclareScriptingCustom(fun(&basic::textureData::frameSize), "frameSize");
MAGE_DeclareScriptingCopyOperator(basic::textureData);

MAGE_DeclareScriptingCustom(user_type<basic::spriteData>(), "spriteData");
MAGE_DeclareScriptingCustom(fun(&basic::spriteData::animations), "animations");
MAGE_DeclareScriptingCustom(fun(&basic::spriteData::texture), "texture");
MAGE_DeclareScriptingCopyOperator(basic::spriteData);
MAGE_DeclareScriptingListableShared(basic::spriteData, "spriteVector");