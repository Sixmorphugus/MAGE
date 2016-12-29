#include "stringHelpers.h"

#include "platform.h"

namespace mage {

std::vector<std::string> splitString(std::string in, char c, char ignoreBetween, unsigned int expectation) {
	std::stringstream test(in);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, c))
	{
		seglist.push_back(segment);
	}

	// ignore between
	if (ignoreBetween != ' ') {
		// go through each character of each segment
		for (int i = 0; i < (int)seglist.size(); i++) {
			bool combineWithNext = false;
			for (unsigned int j = 0; j < seglist[i].size(); j++) {
				if (seglist[i][j] == ignoreBetween) {
					combineWithNext = !combineWithNext;
				}
			}

			if (combineWithNext && i + 1 < (int)seglist.size()) {
				seglist[i] = seglist[i] + c + seglist[i + 1];
				seglist.erase(seglist.begin() + i + 1);

				i--;
			}
		}
	}

	if (seglist.size() < expectation) {
		p::fatal("splitString returned too few values (found " + std::to_string(seglist.size()) + ", expected " + std::to_string(expectation) + ")");
	}

	return seglist;
}

MAGEDLL std::vector<std::string> splitStringAtFirst(std::string in, char c, char ignoreBetween)
{
	auto seglist = splitString(in, c, ignoreBetween);

	if (seglist.size() > 2) {
		std::string sData;
		std::string charString({ c });

		for (unsigned int i = 1; i < seglist.size(); i++) {
			sData += (i != 1 ? charString : "") + seglist[i];
		}

		seglist = { seglist[0], sData };
	}

	return seglist;
}

std::string strUpper(std::string in) {
	std::locale loc;

	for (unsigned int i = 0; i < in.size(); i++) {
		in[i] = std::toupper(in[i], loc);
	}

	return in;
}

std::string strLower(std::string in) {
	std::locale loc;

	for (unsigned int i = 0; i < in.size(); i++) {
		in[i] = std::tolower(in[i], loc);
	}

	return in;
}

std::string strReplace(std::string in, std::string replace, std::string with) {
	std::regex_replace(in, std::regex(replace), with);
	return in;
}

std::string strFilter(std::string in, std::string replace)
{
	return strReplace(in, replace, "");
}

std::string strSub(std::string in, unsigned int start, unsigned int finish)
{
	std::string output;

	for (unsigned int i = start; i < finish, i < in.size(); i++) {
		output.push_back(in[i]);
	}

	// output.push_back('\0'); is unneded in std::string apparently

	return output;
}

std::string fileExtension(std::string path)
{
	auto sp = splitString(path, '.');
	return sp[sp.size() - 1];
}

std::string containerDir(std::string path)
{
	auto split = splitString(path, '/');

	if (split.size() <= 1) {
		// try splitting with windows '\' instead
		split = splitString(path, '\\');
	}

	std::stringstream s;

	for (unsigned int i = 0; i < split.size() - 1; i++) {
		s << split[i] << "/";
	}

	return s.str();
}

std::string fileName(std::string path)
{
	auto split = splitString(path, '/');

	return split[split.size() - 1];
}

void removeUnixNewline(std::string &str) {
#ifndef PLATFORM_WINDOWS
	if (str[str.size() - 1] == '\n' || str[str.size() - 1] == '\r') {
		str.erase(str.size() - 1);
	}
#endif
}

std::string removeWhitespace(std::string in)
{
	in = strFilter(in, "\t");
	in = strFilter(in, "\n");
	// don't remove spaces, they're still important

#ifndef PLATFORM_WINDOWS
	// on UNIX (and other) operating systems, the windows newline "\n\r" is not recognised properly.
	// this is annoying.
	in = strFilter(in, "\r");
#endif

	return in;
}

}

// SE
#include "scriptingEngine.h"
using namespace mage;
using namespace chaiscript;

MAGE_DeclareScriptingCustom(fun(&splitString), "splitString");
MAGE_DeclareScriptingCustom(fun(&strUpper), "strUpper");
MAGE_DeclareScriptingCustom(fun(&strLower), "strLower");
MAGE_DeclareScriptingCustom(fun(&strReplace), "strReplace");
MAGE_DeclareScriptingCustom(fun(&strFilter), "strFilter");
MAGE_DeclareScriptingCustom(fun(&strSub), "strSub");
MAGE_DeclareScriptingCustom(fun(&fileExtension), "fileExtension");