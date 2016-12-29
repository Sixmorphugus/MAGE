#include "group.h"

#include "helpers.h"
#include "Game.h"

using namespace mage;

namespace mage {
	namespace groupPrivate {
		bool compareObjects(std::shared_ptr<basic> i, std::shared_ptr<basic> j) {
			auto woi = dynamic_cast<objBasic*>(i.get());
			auto woj = dynamic_cast<objBasic*>(j.get());

			if (woi && woj) {
				return woi->operator<(*woj); // two objBasics
			}

			p::fatal("A world object group was found to be malformed during a depth ordering operation.");
			return false; // if this is called, uiObjectStart is in the wrong place
		}

		prefabMngr * prefabs()
		{
			return theGame()->prefabs.get();
		}

		void drawLoadingScreenFrame(std::string text, int prog, int maxProg)
		{
			theGame()->drawLoadingScreenFrame(text, prog, maxProg);
		}
	}
}

void groupBase::drawWorldObjects(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (unsigned int i = 0; i < getNumWorldObjects(); i++) {
		applyShader(states, getWorldObject(i)->getTexSizeF());
		target.draw(*getWorldObject(i), states);
	}

	if (theGame()->colBoxes) {
		for (unsigned int i = 0; i < getNumWorldObjects(); i++) {
			getWorldObject(i)->drawCollisionBoxes(target, states);
		}
	}
}

void groupBase::drawUiObjects(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (unsigned int i = 0; i < getNumUiObjects(); i++) {
		applyShader(states, getUiObject(i)->getTexSizeF());
		target.draw(*getUiObject(i), states);
	}
}


// binding
#include "scriptingEngine.h"

// groupBase
using namespace chaiscript;

MAGE_DeclareScriptingType(groupBase);
MAGE_DeclareScriptingBaseClass(serializable, groupBase);
MAGE_DeclareScriptingBaseClass(shadable, groupBase);
MAGE_DeclareScriptingFunction(&groupBase::bringToFront, "bringToFront");
MAGE_DeclareScriptingFunction(&groupBase::clearObjects, "clearObjects");
MAGE_DeclareScriptingFunction(&groupBase::serialize, "serialize");
MAGE_DeclareScriptingFunction(&groupBase::deserialize, "deserialize");
MAGE_DeclareScriptingCustom(fun<void, groupBase, unsigned int>(&groupBase::detach), "detach");
MAGE_DeclareScriptingCustom(fun<void, groupBase, basic*>(&groupBase::detach), "detach");
MAGE_DeclareScriptingFunction(&groupBase::getNumObjects, "getNumObjects");
MAGE_DeclareScriptingFunction(&groupBase::getNumUiObjects, "getNumUiObjects");
MAGE_DeclareScriptingFunction(&groupBase::getNumWorldObjects, "getNumWorldObjects");
MAGE_DeclareScriptingFunction(&groupBase::getObject, "getObject");
MAGE_DeclareScriptingFunction(&groupBase::getUiObject, "getUiObject");
MAGE_DeclareScriptingFunction(&groupBase::getWorldObject, "getWorldObject");
MAGE_DeclareScriptingFunction(&groupBase::preUpdateObjects, "preUpdateObjects");
MAGE_DeclareScriptingFunction(&groupBase::sortWorldObjects, "sortWorldObjects");
MAGE_DeclareScriptingFunction(&groupBase::updateObjects, "updateObjects");
MAGE_DeclareScriptingFunction(&groupBase::onPreUpdate, "onPreUpdate");
MAGE_DeclareScriptingFunction(&groupBase::onUpdate, "onUpdate");
MAGE_DeclareScriptingFunction(&groupBase::onAttach, "onAttach");
MAGE_DeclareScriptingFunction(&groupBase::onDetach, "onDetach");
MAGE_DeclareScriptingFunction(&groupBase::onSet, "onSet");
MAGE_DeclareScriptingFunction(&groupBase::onCombined, "onCombined");
MAGE_DeclareScriptingFunction(&groupBase::onIncorporated, "onIncorporated");

MAGE_DeclareScriptingHook("groupBaseHook", groupBase*);
MAGE_DeclareScriptingHook("groupBaseObjectHook", groupBase*, basic*);
MAGE_DeclareScriptingHook("groupBaseIntegrateHook", const groupBase*, const groupBase*);

// group types
MAGE_DeclareScriptingGroup(basic, "Group");
MAGE_DeclareScriptingCustom(fun(&group<basic>::combine<basic, uiBasic>), "combineUi"); \
MAGE_DeclareScriptingCustom(fun(&group<basic>::combine<basic, objBasic>), "combineWorld"); \

MAGE_DeclareScriptingGroup(objBasic, "objGroup");
MAGE_DeclareScriptingGroup(uiBasic, "uiGroup");