#pragma once
#include "StdAfx.h"
#include "SfmlAfx.h"

#include <chaiscript.hpp>
#include <chaiscript_stdlib.hpp>

#include "hook.h"
#include "helperMacros.h"

namespace mage {

class Game;

// -------------------------------------------------------------
// BINDING
// -------------------------------------------------------------

class MAGEDLL scriptingEngine {
	struct Schedule {
		sf::Time time; // how long to wait
		sf::Time startTime; // how long since we started

		std::function<void()> fun; // call to make
	};
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
	void schedule(sf::Time time, std::function<void()> fun);
public:
	chaiscript::ChaiScript* chai;
	hook<scriptingEngine*> onBind;
	std::vector<Schedule> activeSchedules;

private:
	void bindSFML();
	void bindMAGE();
	void bindHooks();
	void bindLists();

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
	seScriptingEngineRegistration(chaiscript::ModulePtr vt); // registers a module
};

// -------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------

MAGEDLL void chaiPrint(const std::string& data, const std::string& file, const std::string& function, int line);
MAGEDLL void chaiCrash(const std::string &data);

MAGEDLL void handleEvalError(const chaiscript::exception::eval_error &e);

} // namespace mage

#define StartScriptingDeclaration namespace {
#define EndScriptingDeclaration }
#define DeclareScriptingCustom(...) StartScriptingDeclaration mage::seScriptingEngineRegistration UNIQUE_NAME(se)(__VA_ARGS__); EndScriptingDeclaration
#define DeclareScriptingCopyOperator(obj) DeclareScriptingCustom(fun([](obj &o1, obj o2) { o1 = o2; }), "=");
#define DeclareScriptingNamed(arg, name) DeclareScriptingCustom(arg, name);
#define DeclareScriptingBaseClass(base, derived) DeclareScriptingCustom(chaiscript::base_class<base, derived>());
#define DeclareScriptingListable(arg) DeclareScriptingCustom(chaiscript::bootstrap::standard_library::vector_type< std::vector<arg> >(STRING(arg) STRING(List)));
#define DeclareScriptingListableNamed(arg, name) DeclareScriptingCustom(chaiscript::bootstrap::standard_library::vector_type< std::vector<arg> >(name));
#define DeclareScriptingType(arg) DeclareScriptingCustom(chaiscript::user_type<arg>(), STRING(arg));
#define DeclareScriptingTypeNamed(arg, name) DeclareScriptingCustom(chaiscript::user_type<arg>(), name);
#define DeclareScriptingFunction(arg, name) DeclareScriptingCustom(chaiscript::fun(arg), name);
#define DeclareScriptingConstructor(arg, name) DeclareScriptingCustom(chaiscript::constructor<arg>(), name);
#define DeclareScriptingGlobalConst(arg, name) DeclareScriptingCustom(chaiscript::const_var(arg), name);
#define DeclareScriptingEnum(container, enumValue) DeclareScriptingGlobalConst(container::enumValue, STRING(enumValue));
#define DeclareScriptingCastingFunction(name, inp, out) DeclareScriptingCustom(chaiscript::fun([](inp* o1) { return dynamic_cast<out*>(o1); }), name)

#define BIND_COPY_OPERATOR(obj) chai->add(fun([&](obj &o1, obj o2) { o1 = o2; }), "="); // internal legacy system use only
