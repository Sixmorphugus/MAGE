#include "objSpriteless.h"
#include "group.h"

/*
A COMPOSITE object has no appearence itself and is a combination of a group and spriteless object.
Its "child" objects are positioned relative to it.
Its children are saved with it.

Under an objComposite, an object is drawn when the objComposite is. The objects inside are still depth ordered in relation to each other - just not to the world.
Children's collision boxes can't really be kept track of, so they're ignored.
*/

namespace mage {

class MAGEDLL objComposite : public objSpriteless, public groupObj {
public:
	objComposite(float x, float y, std::vector<std::shared_ptr<objBasic>> initialChildList = {});

	// obligatory update/draw.
	virtual void preUpdate(sf::Time elapsed);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	// saving and loading
	virtual std::string serialize();
	virtual bool deserialize(std::string data);

	virtual sf::Vector2f getSize() const; // an objComposite's size is the size of all its children.
	virtual sf::FloatRect getBounds() const;  // an objComposite's bounds is its position + size boxed in with its' collision boxes.
};

}