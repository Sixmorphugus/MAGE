#pragma once
#include "StdAfx.h"
#include "SfmlAfx.h"

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

#include "helperMacros.h"
#include "hook.h"

// temporary - chaiscript wrongfully triggers 4503
#ifdef MAGE_COMPILER_VS
#pragma warning(disable : 4503)
#endif

namespace mage {

class Game;

// -------------------------------------------------------------
// BINDING
// -------------------------------------------------------------

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
public:
	chaiscript::ChaiScript* chai;
	hook<scriptingEngine*> onBind;

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

// -------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------

MAGEDLL void chaiPrint(const std::string& data, const std::string& file, const std::string& function, int line);
MAGEDLL void chaiCrash(const std::string &data);

MAGEDLL void handleEvalError(const chaiscript::exception::eval_error &e);

MAGEDLL chaiscript::Module& seGetStartupModule();

} // namespace mage

#define MAGE_DeclareScriptingCustom(...) namespace {\
	mage::seScriptingEngineRegistration UNIQUE_IDENTIFIER(se)(__VA_ARGS__);\
}

#define MAGE_DeclareScriptingCopyOperator(obj) MAGE_DeclareScriptingCustom(chaiscript::fun([](obj &o1, obj o2) { o1 = o2; }), "=");
#define MAGE_DeclareScriptingNamed(arg, name) MAGE_DeclareScriptingCustom(arg, name);
#define MAGE_DeclareScriptingBaseClass(base, derived) MAGE_DeclareScriptingCustom(chaiscript::base_class<base, derived>());
#define MAGE_DeclareScriptingType(arg) MAGE_DeclareScriptingCustom(chaiscript::user_type<arg>(), STRING(arg));
#define MAGE_DeclareScriptingTypeNamed(arg, name) MAGE_DeclareScriptingCustom(chaiscript::user_type<arg>(), name);
#define MAGE_DeclareScriptingFunction(arg, name) MAGE_DeclareScriptingCustom(chaiscript::fun(arg), name);
#define MAGE_DeclareScriptingConstructor(arg, name) MAGE_DeclareScriptingCustom(chaiscript::constructor<arg>(), name);
#define MAGE_DeclareScriptingGlobalConst(arg, name) MAGE_DeclareScriptingCustom(chaiscript::const_var(arg), name);
#define MAGE_DeclareScriptingEnum(container, enumValue) MAGE_DeclareScriptingGlobalConst(container::enumValue, STRING(enumValue));
#define MAGE_DeclareScriptingCastingFunction(name, inp, out) MAGE_DeclareScriptingCustom(chaiscript::fun([](inp* o1) { return dynamic_cast<out*>(o1); }), name)

#define MAGE_DeclareScriptingListableNamed(type, name) MAGE_DeclareScriptingCustom(chaiscript::bootstrap::standard_library::vector_type<std::vector<type>>(name)); \
MAGE_DeclareScriptingCustom(chaiscript::vector_conversion<std::vector<type>>());

#define MAGE_DeclareScriptingListableShared(type, name) MAGE_DeclareScriptingListableNamed(std::shared_ptr<type>, name);
#define MAGE_DeclareScriptingListable(type) MAGE_DeclareScriptingListableNamed(type, STRING(type) "Vector")

#define BIND_COPY_OPERATOR(obj) chai->add(fun([&](obj &o1, obj o2) { o1 = o2; }), "="); // internal legacy system use only