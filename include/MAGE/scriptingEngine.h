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

#define DeclareScriptingCustom(...) namespace {\
	mage::seScriptingEngineRegistration UNIQUE_IDENTIFIER(se)(__VA_ARGS__);\
}

#define DeclareScriptingCopyOperator(obj) DeclareScriptingCustom(chaiscript::fun([](obj &o1, obj o2) { o1 = o2; }), "=");
#define DeclareScriptingNamed(arg, name) DeclareScriptingCustom(arg, name);
#define DeclareScriptingBaseClass(base, derived) DeclareScriptingCustom(chaiscript::base_class<base, derived>());
#define DeclareScriptingType(arg) DeclareScriptingCustom(chaiscript::user_type<arg>(), STRING(arg));
#define DeclareScriptingTypeNamed(arg, name) DeclareScriptingCustom(chaiscript::user_type<arg>(), name);
#define DeclareScriptingFunction(arg, name) DeclareScriptingCustom(chaiscript::fun(arg), name);
#define DeclareScriptingConstructor(arg, name) DeclareScriptingCustom(chaiscript::constructor<arg>(), name);
#define DeclareScriptingGlobalConst(arg, name) DeclareScriptingCustom(chaiscript::const_var(arg), name);
#define DeclareScriptingEnum(container, enumValue) DeclareScriptingGlobalConst(container::enumValue, STRING(enumValue));
#define DeclareScriptingCastingFunction(name, inp, out) DeclareScriptingCustom(chaiscript::fun([](inp* o1) { return dynamic_cast<out*>(o1); }), name)

#define DeclareScriptingListableNamed(type, name) DeclareScriptingCustom(chaiscript::bootstrap::standard_library::vector_type<std::vector<type>>(name)); \
DeclareScriptingFunction([](const std::vector<chaiscript::Boxed_Value> &v) {\
	std::vector<type> lst;\
	for(unsigned int i = 0; i < v.size(); i++) {\
		lst.push_back(chaiscript::boxed_cast<type>(v[i]));\
	}\
	return lst;\
}, name)\
DeclareScriptingFunction([](const std::vector<type> &v) {\
	std::vector<chaiscript::Boxed_Value> lst;\
	for(unsigned int i = 0; i < v.size(); i++) {\
		lst.push_back(chaiscript::Boxed_Value(v[i]));\
	}\
	return lst;\
}, "Vector")
// ^ new constructor from chai "Vector" type

#define DeclareScriptingListableShared(type, name) DeclareScriptingListableNamed(std::shared_ptr<type>, name);
#define DeclareScriptingListable(type) DeclareScriptingListableNamed(type, STRING(type) "Vector")

#define BIND_COPY_OPERATOR(obj) chai->add(fun([&](obj &o1, obj o2) { o1 = o2; }), "="); // internal legacy system use only