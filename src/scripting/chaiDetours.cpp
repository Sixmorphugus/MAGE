#include "chaiDetours.h"

#include "platform.h"
#include "stringHelpers.h"

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