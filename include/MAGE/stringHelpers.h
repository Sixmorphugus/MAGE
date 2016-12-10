#pragma once
#include "StdAfx.h"
#include "platformDeduction.h"

namespace mage {

// STRINGS
// ---------------------------------------------------------------------
MAGEDLL std::vector<std::string> splitString(std::string in, char c = ' ', char ignoreBetween = ' ', unsigned int expectation = 0);
MAGEDLL std::vector<std::string> splitStringAtFirst(std::string in, char c = ' ', char ignoreBetween = ' ');
MAGEDLL std::string strUpper(std::string in);
MAGEDLL std::string strLower(std::string in);
MAGEDLL std::string strReplace(std::string in, char replace, char with);
MAGEDLL std::string strFilter(std::string in, char replace);
MAGEDLL std::string strSub(std::string in, unsigned int start, unsigned int finish);
MAGEDLL void removeNewline(std::string &str);
MAGEDLL std::string fileExtension(std::string path);
MAGEDLL std::string containerDir(std::string path);
MAGEDLL std::string fileName(std::string path);

} //namespace mage