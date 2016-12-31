// mage platform include
// -------------
// Automatically deduces the compiler's platform and settings.
// Also defines MAGEDLL, which means "export this function/class to modules".
// -------------
// Category: Core
// Has Hooks: N/A
// Script-Accessable: N/A
// -------------

#if defined(_MSC_VER)
#define MAGE_PLATFORM_WINDOWS // MSC only runs on windows
#define MAGE_PLATFORM_STRING std::string("Windows")
#define MAGE_COMPILER_VS
#if defined(MAGEDLL_EXPORT)
#define MAGEDLL __declspec(dllexport)
#else
#define MAGEDLL __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#if defined(WIN32)
#define MAGE_PLATFORM_WINDOWS
#define MAGE_COMPILER_GNUC
#define MAGE_PLATFORM_STRING std::string("Windows")
#endif
#if defined(__linux__)
#define MAGE_PLATFORM_LINUX
#define MAGE_COMPILER_GNUC
#define MAGE_PLATFORM_STRING std::string("Linux")
#endif

#if defined(MAGEDLL_EXPORT)
#define MAGEDLL __attribute__ ((visibility ("default")))
#else
#define MAGEDLL
#endif
#else
#error "Please use either GCC or Microsoft Visual C++ (preferred) to compile this."
#endif