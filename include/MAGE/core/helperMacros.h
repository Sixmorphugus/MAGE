#pragma once

// Helper Macros
// -------------
// Some helpers for working with macros.
// Makes things seem a little more magic - alternatively, keeps me from having to type preprocessor operators.
// -------------
// Category: Core
// Has Hooks: N/A
// Script-Accessable: N/A
// -------------

#define CAT_I(a,b) a##b
#define CAT(a,b) CAT_I(a, b)

#define STRING(s) #s

#define MAGE_StringMap(type) std::map<std::string, type>

// This is some crazy magic that helps produce __BASE__247
// Vanilla interpolation of __BASE__##__LINE__ would produce __BASE____LINE__
// I still can't figure out why it works, but it has to do with macro resolution ordering
// Obviously it's from stack overflow.
#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res

#define UNIQUE_IDENTIFIER(base) PP_CAT(base, __COUNTER__)