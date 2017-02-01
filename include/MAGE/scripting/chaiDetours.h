#include "StdAfx.h"
#include "ChaiAfx.h"

namespace mage {
	MAGEDLL void chaiPrint(const std::string& data, const std::string& file, const std::string& function, int line);
	MAGEDLL void chaiCrash(const std::string &data);

	MAGEDLL void handleEvalError(const chaiscript::exception::eval_error &e);
}
