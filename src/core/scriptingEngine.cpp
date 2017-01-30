#include "scriptingEngine.h"

using namespace chaiscript;
using namespace mage;

// Chaiscript Scripting Engine Objects and binding

#include "stringHelpers.h"
#include "platform.h"
#include "Game.h"

// -------------------------------------------------------------
// BINDING
// -------------------------------------------------------------

scriptingEngine::scriptingEngine()
{
	init = false;
}

scriptingEngine::~scriptingEngine()
{
	delete chai;
}

void scriptingEngine::initChai()
{
	init = true;

	p::info("Initializing scripting engine.");

	p::log("\tLoad precompiled chai libraries...");
	chai = new ChaiScript();

	bind();
}

void scriptingEngine::update()
{
	initCheck();
}

void scriptingEngine::bind()
{
	try {
		p::log("\tBind base...");
		chai->add(fun(&chaiPrint), "logInfo");
		chai->add(fun(&chaiCrash), "crashGame");
		chai->add(fun([&](std::string in) { return evalFile(in); } ), "evalFile");

		p::log("\tBind SFML...");
		bindSFML();

		p::log("\tBind Engine...");
		bindMAGE();

		p::log("\tBind Hooks...");
		bindHooks();

		p::log("\tReplace some default functions...");
		chai->eval("global print = fun(x){ logInfo(to_string(x), __FILE__, __FUNC__, __LINE__); }");
		chai->eval("global use = fun(x){ return evalFile(x); }");

		p::log("\tFinito!");
	}
	catch (std::runtime_error &e) {
		p::fatal("Unable to start chaiscript engine:\n" + std::string(e.what()));
	}

	//dump();
}

void scriptingEngine::initCheck()
{
	// if we didn't initialize do that now
	if (!init)
		initChai();
}

Boxed_Value scriptingEngine::evalFile(std::string filePath)
{
	initCheck();

	// this means that this script (and any hooks in it) will work from the folder of the script.
	std::string pWorkDir = p::getWorkDir();
	p::setWorkDir(containerDir(filePath));

	p::info("Executing " + filePath);

	Boxed_Value returnValue(NULL);

	try {
		returnValue = chai->eval_file(fileName(filePath));
	}
	catch (chaiscript::exception::eval_error& e) {
		handleEvalError(e);
	}
	catch (chaiscript::exception::file_not_found_error& e) {
		p::warn(std::string(e.what()));
	}
	catch (std::runtime_error &e) {
		p::warn(e.what());
	}
	catch (...) {
		p::warn("Unknown error. Please never do that again.");
	}

	p::setWorkDir(pWorkDir);

	return returnValue;
}

Boxed_Value scriptingEngine::eval(std::string in)
{
	return chai->eval(in);
}

void scriptingEngine::dump(chaiscript::Boxed_Value* in)
{
	initCheck();

	p::log("BEGIN SE DUMP --------------------------");

	auto s = chai->get_state().engine_state;

	std::vector<chaiscript::Type_Info> bases;

	if (in) {
		p::log("C++ Class: " + in->get_type_info().bare_name());
		bases = getUpTypes(in->get_type_info());
	}
	else {
		p::log("You're dumping without a target.\nHOLD ONTO YOUR HATS BOYS");
	}

	p::log("Registered functions and members:");
	for (auto m = s.m_functions.begin(); m != s.m_functions.end(); m++) {
		bool didDisplayThing = false;

		for (auto o = m->second.get()->begin(); o != m->second.get()->end(); o++) {
			auto f = o->get();

			bool displayIt = true;

			if (in) {
				auto ourType = in->get_type_info();

				displayIt = false;

				if (o->get()->get_param_types().size() < 2)
					continue;

				auto funcType = o->get()->get_param_types()[1];

				if (ourType == funcType) {
					displayIt = true;
				}

				for (unsigned int b = 0; b < bases.size(); b++) {
					if (bases[b] == funcType) {
						displayIt = true;
					}
				}
			}

			if (!displayIt)
				continue;

			if (!didDisplayThing) {
				p::log(m->first + " (" + std::to_string((m->second.get()->size())) + " overloads)");
				didDisplayThing = true;
			}

			std::string name = f->get_param_types()[0].bare_name();
			std::string n = "[C++] " + name;

			// find the chai name
			for (auto j = s.m_types.begin(); j != s.m_types.end(); j++) {
				if (j->second == f->get_param_types()[0]) {
					n = j->first;
				}
			}

			p::log("\t" + n + " " + m->first + "(", false);

			for (unsigned int i = 1; i < f->get_param_types().size(); i++) {
				std::string name = f->get_param_types()[i].bare_name();
				std::string n = "[C++] " + name;

				// find the chai name
				for (auto j = s.m_types.begin(); j != s.m_types.end(); j++) {
					if (j->second == f->get_param_types()[i]) {
						n = j->first;
					}
				}

				p::log(n, false);

				if (i + 1 < f->get_param_types().size()) {
					p::log(", ", false);
				}
			}

			p::log(")");
		}
	}

	if (!in) {
		p::log("Registered global variables:");
		for (auto m = s.m_global_objects.begin(); m != s.m_global_objects.end(); m++) {
			p::log(m->first);
		}
	}

	p::log("END SE DUMP ----------------------------");
}

void scriptingEngine::whatIs(chaiscript::Boxed_Value * in)
{
	initCheck();

	p::log("C++ Class: " + in->get_type_info().bare_name());

	auto s = chai->get_state().engine_state;

	// find the chai name
	for (auto j = s.m_types.begin(); j != s.m_types.end(); j++) {
		if (j->second == in->get_type_info()) {
			p::log("known by Chaiscript as " + j->first);
		}
	}
}

chaiscript::Boxed_Value scriptingEngine::trueForm(chaiscript::Boxed_Value& baseForm)
{
	// they said I was mad
	// they said it couldn't be done
	// here it is.

	auto convs = getDownConversions(baseForm.get_type_info());

	// try each conversion
	// if one is successful, run this function again on the returned box value before breaking out

	for (unsigned int i = 0; i < convs.size(); i++) {
		try {
			auto result = convs[i]->convert_down(baseForm);

			if (!result.is_null()) { // not sure if this is needed - no harm in checking
				return trueForm(result);
			}
		}
		catch (std::bad_cast &e) {
			// we expect several of these so ignore them all.
		}
	}

	return baseForm;

	// NOTE:
	// This can be an incredibly slow function to run.
	// It is only fully usable from within the scripting engine.
	// It should be your last resort, not your first.
}

std::vector<chaiscript::Type_Info> scriptingEngine::getUpTypes(const chaiscript::Type_Info& of) const
{
	std::vector<Type_Info> upTypes;
	auto upConvs = getUpConversions(of);

	for (unsigned int i = 0; i < upConvs.size(); i++) {
		upTypes.push_back(upConvs[i]->to());
	}

	return upTypes;
}

std::vector<chaiscript::Type_Info> scriptingEngine::getDownTypes(const chaiscript::Type_Info& of) const
{
	std::vector<Type_Info> upTypes;
	auto upConvs = getDownConversions(of);

	for (unsigned int i = 0; i < upConvs.size(); i++) {
		upTypes.push_back(upConvs[i]->from());
	}

	return upTypes;
}

std::vector<chaiscript::Type_Conversion> scriptingEngine::getConversions(const chaiscript::Type_Info& of) const
{
	std::vector<chaiscript::Type_Conversion> list;

	for (unsigned int i = 0; i < knownTypeConversions.size(); i++) {
		if (knownTypeConversions[i]->from() == of || 
		(knownTypeConversions[i]->to() == of && knownTypeConversions[i]->bidir())) {
			list.push_back(knownTypeConversions[i]);
		}
	}

	return list;
}

std::vector<chaiscript::Type_Conversion> scriptingEngine::getUpConversions(const chaiscript::Type_Info & of) const
{
	std::vector<chaiscript::Type_Conversion> list;

	for (unsigned int i = 0; i < knownTypeConversions.size(); i++) {
		if (knownTypeConversions[i]->from() == of) {
			list.push_back(knownTypeConversions[i]);
		}
	}

	return list;
}

std::vector<chaiscript::Type_Conversion> scriptingEngine::getDownConversions(const chaiscript::Type_Info & of) const
{
	std::vector<chaiscript::Type_Conversion> list;

	for (unsigned int i = 0; i < knownTypeConversions.size(); i++) {
		if (knownTypeConversions[i]->to() == of && knownTypeConversions[i]->bidir()) {
			list.push_back(knownTypeConversions[i]);
		}
	}

	return list;
}

// -------------------------------------------------------------
// FUNCTIONS
// -------------------------------------------------------------
namespace mage {
	void chaiPrint(const std::string& data, const std::string& file, const std::string& function, int line)
	{
		p::diagnosticLog(p::CHAISCRIPT, file, function, line, data);
	}

	void chaiCrash(const std::string &data)
	{
		p::fatal(data);
	}

	void handleEvalError(const chaiscript::exception::eval_error &e)
	{
		p::warn("Eval exception was thrown;");

		std::string errorToLog = e.pretty_print();

		auto lines = splitString(errorToLog, '\n');

		for (unsigned int i = 0; i < lines.size(); i++) {
			if (lines[i] == "")
				continue;

			p::log("\t" + lines[i]);
		}

		if (MAGE_CRASHWHENEVALFAILS) {
			p::fatal(errorToLog);
		}
	}
}

MAGE_DeclareScriptingSerializable(std::string);