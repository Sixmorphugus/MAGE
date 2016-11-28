cmake_minimum_required(VERSION 3.6)

project(MAGE)

set(MAGE_SRC "src/Game.cpp")

add_subdirectory(extlibs) # find all externals

# options for externals - use all sfml libs but network
set(MAGE_SFML_LIB sfml-main sfml-system sfml-window sfml-graphics sfml-audio)
set(MAGE_SFML_INCLUDE ${CMAKE_SOURCE_DIR}/extlibs/SFML/include)

set(MAGE_CHAI_INCLUDE ${CMAKE_SOURCE_DIR}/extlibs/chaiscript)

# Multipurpose Arcade Game Engine (Core)
add_library(MAGE SHARED ${MAGE_SRC})
include_directories("./include")

target_compile_definitions(MAGE PRIVATE MAGEDLL_EXPORT) # tell MAGE's header files that they are exporting functions, not importing them

# SFML + chai
target_link_libraries(MAGE ${MAGE_SFML_LIB})
include_directories(${MAGE_SFML_INCLUDE})

include_directories(${MAGE_CHAI_INCLUDE}) # chai is header only

# On any UNIX os, MAGE needs libdl
if(UNIX)
	target_link_libraries(MAGE dl)
endif()

# add the version number
add_custom_command(
    TARGET MAGE
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}" -DPROJECT_BINARY_DIR="${PROJECT_BINARY_DIR}" -P "${CMAKE_CURRENT_LIST_DIR}/version.cmake"
)

# ----
# content of version.cmake

# find git version number
execute_process(
  COMMAND git rev-list HEAD --count
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT GIT_VERSION)
	set(GIT_VERSION 0)
endif()



configure_file (
  "${PROJECT_SOURCE_DIR}/build.h.in"
  "${PROJECT_BINARY_DIR}/build.h"
  )

# ----

include_directories("${PROJECT_BINARY_DIR}") # reason for this is that build.h is created in the engine's compilation folder

# fix msc's working directory
if(MSVC)
	file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vcxproj.user" 
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>
	<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
	  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">
		<LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
		<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	  </PropertyGroup>
	  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">
		<LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
		<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	  </PropertyGroup>
	  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='RelWithDebInfo|Win32'\">
		<LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
		<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	  </PropertyGroup>
	  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">
		<LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
		<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	  </PropertyGroup>
	  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='MinSizeRel|Win32'\">
		<LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
		<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	  </PropertyGroup>
	  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">
		<LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
		<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	  </PropertyGroup>
	</Project>")
endif()

# copy SFML dll files
if(WIN32)
	add_custom_command(TARGET MAGE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SFML_DLLS} $<TARGET_FILE_DIR:MAGE>)
endif()