#pragma once
#include "StdAfx.h"
#include "SfmlAfx.h"

#include "animator.h"
#include "hook.h"
#include "serializable.h"
#include "scriptingEngine.h"
#include "shaders.h"

#define CLONEABLE(type) virtual type* clone() const { type* a = new type(*this); a->registerProperties(); return a;}

namespace mage {

class groupBase;
class prefab;

class resourceTexture;
class resourceShader;
class resourceSoundBuffer;
class resourceFont;
class view;

// forward declaration of group:
template<class T> class group;
typedef group<basic> Group;

// BASIC OF BASICS.
// ----------------------------------------------------------------------------
// Template for game objects.
// Don't derive it unless you for some reason need to add a new object type to the engine.
// If you do need to add a new object type, be aware you'll need to modify the Game class.

/*
A basic is the most "basic" type of game object that can exist in MAGE.
It cannot be represented onscreen; only objects that derive either uiBasic or objBasic can.
It can however still be held in a state group and have an effect on the game.
You can therefore go ahead and put a basic in the background of the game to manage something if you want (though it's probably a better idea to use SE hooks)
*/

class MAGEDLL basic:
	public virtual sf::Drawable, // sfml will draw this
	public virtual sf::Transformable, // this object has position, rotation etc
	public virtual serializable, // this object can save its state to text and reload it
	public virtual shadable // this object holds a shader
{
public:
	// Basic Properties.
	// You can reference a member variable (or any variable really) in memory and change it with its string name.
	// The variable must use a basic type, though.

	class MAGEDLL prop {
	public:
		prop();

		prop(int &intRef);
		prop(float &floatRef);
		prop(std::string &stringRef);
		prop(bool &boolRef);
		prop(sf::Uint8 &colRef);

		enum TYPE {
			INTPROP,
			FLOATPROP,
			STRINGPROP,
			BOOLPROP,
			COLPROP,
			NA
		};

		void update(int up);
		void update(float up);
		void update(std::string up);

		std::string read();

		TYPE getType() { return typ; }

		bool hidden = false;
		std::string cat;

	private:
		int *iRef;
		float *fRef;
		std::string *sRef;
		bool *bRef;
		sf::Uint8 *cRef;

		TYPE typ;
	};

	class MAGEDLL textureData {
	public:
		textureData();
		textureData(std::shared_ptr<resourceTexture> spriteResource);
		textureData(std::shared_ptr<resourceTexture> spriteResource, unsigned int frameWidth, unsigned int frameHeight);

	public:
		std::weak_ptr<resourceTexture> resource;
		sf::Vector2u frameSize;
		bool defaultFrameSize;
	};

	class MAGEDLL spriteData {
	public:
		spriteData(basic* owner, textureData &texData);

	public:
		textureData texture;
		animator animations;
	};

public:
	basic(float x, float y, textureData sd = textureData(nullptr));

	// follow the rule of 3
	~basic();
	basic(const basic& cp);
	basic& operator=(basic assignedCopy);

	void copyFrom(const basic& cp);

	virtual basic* clone() const { basic* a = new basic(*this); a->registerProperties(); return a; }

	// can an object be cloned reliably with clone()?
	bool isCloneSafe();

	// this is important because it allows runtime names to be given to object variables
	// it should really be called "init".
	virtual void registerProperties();
	// note that YOU HAVE TO CALL IT MANUALLY.

	// obligatory update/draw.
	virtual void preUpdate(sf::Time elapsed);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	// saving and loading
	virtual std::string serialize();
	virtual bool deserialize(std::string data);

	// all the basic functionality of a visible object.
	unsigned int addSprite(textureData texture);
	int getCurrentSpriteIndex() const;
	const std::shared_ptr<spriteData> getCurrentSprite() const;
	std::shared_ptr<spriteData> getCurrentSprite();
	void setCurrentSprite(int spr);
	void replaceCurrentSprite(textureData texture);
	void sfSync();
	void doPixelGridCorrection(bool force = true);

	void destroy();
	void scheduleDestroy();

	// helpers
	sf::Vector2f getCenter() const;
	const sf::Texture* getTexPointer() const { return sfSprite.getTexture(); }
	std::shared_ptr<resourceTexture> getTexResource() const;
	sf::Vector2u getFrameSize() const;
	sf::Vector2f getRealPosition() const; // an object's "real position" is always its top left regardless of origin.
	virtual sf::Vector2f getTexSizeF() const;
	virtual sf::Vector2u getTexSize() const;
	virtual sf::Vector2f getSize() const; // an object's "size" is usually determined by its sprite, not collision boxes. It is generally used for draw ordering.
	virtual sf::FloatRect getBounds() const; // an object's "bounds" are determined by all components it has and what space they take up. It is generally used for collision and selection in editors.
	virtual sf::FloatRect getMainBounds() const;

	void setRealPosition(sf::Vector2f p);
	std::shared_ptr<basic> shareThis();

	std::shared_ptr<prefab> getPrefabSource() const { return prefabSource; }

	bool isProperty(std::string name);
	prop& getProperty(std::string name);
	std::vector<std::string> getPropertyList();

	prop& operator[](std::string name);

	groupBase* getGroup() const { return gr; }

	bool hasMoved() const { return moved; }

public:
	groupBase* gr;

	std::vector<std::shared_ptr<spriteData>> sprites;

	sf::Sprite sfSprite;

	sf::Vector2f hackPos; // position used for property
	sf::Vector2f hackOrigin; // origin used for property
	sf::Vector2f hackScale; // scale used for property
	float hackRot; // rotation used for property

	hook<basic*> onCreated;
	hook<basic*> onFirstUpdate;
	hook<basic*> onUpdate;
	hook<basic*> onPreUpdate;
	hook<basic*> onDestroyed;
	hook<const basic*, sf::RenderTarget*> onDrawn;

	std::shared_ptr<prefab> prefabSource;
	std::string uiName;
	bool enforcePixelGrid;

	chaiscript::dispatch::Dynamic_Object vars;

protected:
	void registerProperty(std::string name, prop p);
	void deleteProperty(std::string name);

protected:
	sf::Vector2f posCheck;
	sf::Vector2f orCheck;
	sf::Vector2f scaleCheck;
	float rotCheck;
private:
	bool initializedCalled;
	int curSprite;
	std::map<std::string, prop> properties;
	bool moved;
	bool spriteLoadFailed;
};

} // namespace mage

#define DeclareScriptingBasic(type)\
DeclareScriptingType(type); \
DeclareScriptingBaseClass(mage::basic, type); \
DeclareScriptingBaseClass(mage::serializable, type); \
DeclareScriptingBaseClass(sf::Transformable, type); \
DeclareScriptingBaseClass(mage::shadable, type); \
DeclareScriptingCastingFunction("to_" STRING(type), mage::basic, type);
