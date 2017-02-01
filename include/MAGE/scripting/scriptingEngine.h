#pragma once

// mage::scriptingEngine
// -------------
// An encapsulation of the chaiscript scripting engine.
// A modular object that handles the global scripting context.
// This file also provides black magic macros for registration of scripting classes and functions.
// -------------
// Category: Core
// Has Hooks: No
// Script-Accessable: No
// -------------

#include "StdAfx.h"
#include "SfmlAfx.h"

#include "helperMacros.h"
#include "stringHelpers.h"
#include "hook.h"

#include "resourceCereal.h"

namespace mage {

class Game;

// -------------------------------------------------------------
// BINDING
// -------------------------------------------------------------

MAGEDLL chaiscript::Module& seGetStartupModule();

class MAGEDLL scriptingEngine {
public:
	scriptingEngine();
	~scriptingEngine();

	void initChai();

	void update();

	void bind();

	void initCheck();

	chaiscript::Boxed_Value evalFile(std::string fileName);
	chaiscript::Boxed_Value eval(std::string in);
	void dump(chaiscript::Boxed_Value* in = nullptr);
	void whatIs(chaiscript::Boxed_Value* in);

	chaiscript::Boxed_Value trueForm(chaiscript::Boxed_Value& baseForm);

	std::vector<chaiscript::Type_Info> getUpTypes(const chaiscript::Type_Info& of) const;
	std::vector<chaiscript::Type_Info> getDownTypes(const chaiscript::Type_Info& of) const;

	std::vector<chaiscript::Type_Conversion> getConversions(const chaiscript::Type_Info& of) const;
	std::vector<chaiscript::Type_Conversion> getUpConversions(const chaiscript::Type_Info& of) const;
	std::vector<chaiscript::Type_Conversion> getDownConversions(const chaiscript::Type_Info& of) const;

public:
	chaiscript::ChaiScript* chai;
	std::vector<chaiscript::Type_Conversion> knownTypeConversions;

private:
	void bindSFML();
	void bindMAGE();
	void bindHooks();

	bool init;
};

// hacky object to register things with
class MAGEDLL seScriptingEngineRegistration
{
public:
	seScriptingEngineRegistration(chaiscript::Type_Info ti, std::string name); // registers type_info to the scripting engine when created
	seScriptingEngineRegistration(chaiscript::Type_Conversion d); // registers a type_conversion to the scripting engine when created
	seScriptingEngineRegistration(chaiscript::Proxy_Function f, std::string name); // registers a function to the scripting engine when created
	seScriptingEngineRegistration(const chaiscript::Boxed_Value t_bv, std::string t_name); // registeres a const to the scripting engine when created
	seScriptingEngineRegistration(chaiscript::ModulePtr mp); // registeres a const to the scripting engine when created
};

} // namespace mage

#define MAGE_DeclareScriptingCustom(...) namespace {\
	mage::seScriptingEngineRegistration UNIQUE_IDENTIFIER(se)(__VA_ARGS__);\
}

#define MAGE_DeclareScriptingUnlistableType(arg) \
MAGE_DeclareScriptingCustom(chaiscript::user_type<arg>(), mage::fixChaiName(STRING(arg)));\

#define MAGE_DeclareScriptingType(arg) \
MAGE_DeclareScriptingUnlistableType(arg)\
MAGE_DeclareScriptingCustom(chaiscript::bootstrap::standard_library::vector_type<std::vector<arg>>(mage::fixChaiName(STRING(arg) "Vector"))); \
MAGE_DeclareScriptingCustom(chaiscript::vector_conversion<std::vector<arg>>()); \
MAGE_DeclareScriptingCustom(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::shared_ptr<arg>>>(mage::fixChaiName(STRING(arg) "ShpVector"))); \
MAGE_DeclareScriptingCustom(chaiscript::vector_conversion<std::vector<std::shared_ptr<arg>>>());\
MAGE_DeclareScriptingCustom(chaiscript::bootstrap::standard_library::map_type<std::map<std::string, arg>>(mage::fixChaiName(STRING(arg) "Map"))); \
MAGE_DeclareScriptingCustom(chaiscript::map_conversion<std::map<std::string, arg>>()); \
MAGE_DeclareScriptingCustom(chaiscript::bootstrap::standard_library::map_type<std::map<std::string, std::shared_ptr<arg>>>(mage::fixChaiName(STRING(arg) "ShpMap"))); \
MAGE_DeclareScriptingCustom(chaiscript::map_conversion<std::map<std::string, std::shared_ptr<arg>>>());

#define MAGE_DeclareScriptingCopyOperator(obj) MAGE_DeclareScriptingCustom(chaiscript::fun([](obj &o1, obj o2) { o1 = o2; }), "=");
#define MAGE_DeclareScriptingNamed(arg, name) MAGE_DeclareScriptingCustom(arg, name);
#define MAGE_DeclareScriptingBaseClass(base, derived) MAGE_DeclareScriptingCustom(chaiscript::base_class<base, derived>());
#define MAGE_DeclareScriptingTypeNamed(arg, name) MAGE_DeclareScriptingCustom(chaiscript::user_type<arg>(), name);
#define MAGE_DeclareScriptingFunction(arg, name) MAGE_DeclareScriptingCustom(chaiscript::fun(arg), name);
#define MAGE_DeclareScriptingConstructor(arg, name) MAGE_DeclareScriptingCustom(chaiscript::constructor<arg>(), name);
#define MAGE_DeclareScriptingGlobalConst(arg, name) MAGE_DeclareScriptingCustom(chaiscript::const_var(arg), name);
#define MAGE_DeclareScriptingEnum(container, enumValue) MAGE_DeclareScriptingGlobalConst(container::enumValue, STRING(enumValue));
#define MAGE_DeclareScriptingCastingFunction(name, inp, out) MAGE_DeclareScriptingCustom(chaiscript::fun([](inp* o1) { return dynamic_cast<out*>(o1); }), name);

#define MAGE_DeclareScriptingListableNamed(type, name) // removed
#define MAGE_DeclareScriptingListableShared(type, name) // removed
#define MAGE_DeclareScriptingListable(type) // removed

#define MAGE_DeclareScriptingSerializableNamed(type, name, resourceSerializer)\
MAGE_DeclareScriptingFunction(&mage::s::saveBinaryFile<type>, "saveBinaryFile");\
MAGE_DeclareScriptingFunction(&mage::s::saveXmlFile<type>, "saveXmlFile");\
MAGE_DeclareScriptingFunction(&mage::s::saveJsonFile<type>, "saveJsonFile");\
MAGE_DeclareScriptingFunction(&mage::s::loadBinaryFile<type>, "loadBinaryFile");\
MAGE_DeclareScriptingFunction(&mage::s::loadXmlFile<type>, "loadXmlFile");\
MAGE_DeclareScriptingFunction(&mage::s::loadJsonFile<type>, "loadJsonFile");\
MAGE_DeclareScriptingFunction(&mage::s::getXml<type>, "getXml");\
MAGE_DeclareScriptingFunction(&mage::s::getJson<type>, "getJson");\
MAGE_DeclareScriptingFunction(&mage::p::loadTextFile, "loadTextFile");\
MAGE_DeclareScriptingFunction(&mage::p::saveTextFile, "saveTextFile");\
MAGE_DeclareScriptingSerializedResource(type, resourceSerializer, "resource" + mage::capitalise(mage::fixChaiName(name)));

#define MAGE_DeclareScriptingSerializable(type, resourceSerializer) MAGE_DeclareScriptingSerializableNamed(type, STRING(type), resourceSerializer);

#define BIND_COPY_OPERATOR(obj) chai->add(fun([&](obj &o1, obj o2) { o1 = o2; }), "="); // internal legacy system use only