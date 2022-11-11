cmake_minimum_required(VERSION 3.23.2 FATAL_ERROR)
project(BotTaws VERSION 22.11.0 LANGUAGES CXX)

# Disable developer warnings for CMake
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")

# Disable Conan spitting a lot of logs
set(CONAN_CMAKE_SILENT_OUTPUT TRUE)

# Export all the compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Build an executable with a WinMain entry point on windows.
set(CMAKE_WIN32_EXECUTABLE ON)

# Set C++ to version 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME "app")

# Enable ccache
find_program(CCACHE_FOUND ccache HINTS /bin /usr/bin)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

add_compile_options(-Wall
    -Wextra
    -Werror
    -pedantic
    -pedantic-errors
    -ftrivial-auto-var-init=zero
    -flto=auto
    -ffunction-sections
    -fdata-sections
    -fasynchronous-unwind-tables
    -fno-common
    -fno-semantic-interposition
    -fstack-protector-all
    -fstack-clash-protection
    -fvisibility=hidden
    -fdiagnostics-color=auto
    -grecord-gcc-switches
    -g3
    -pipe
    -pthread)

add_link_options(-Wl,-O2
    -Wl,-z,noexecstack
    -Wl,-z,relro
    -Wl,-z,now
    -Wl,--no-as-needed
    -Wl,--gc-sections
    -Wl,--discard-all)

add_definitions(-D_GLIBCXX_ASSERTIONS
    -D_FORTIFY_SOURCE=3)

find_package(SelfPackers)
if(SELF_PACKER_FOR_EXECUTABLE)
    message(STATUS "Binaries will be compressed using ${SELF_PACKER_FOR_EXECUTABLE}")
    set(CMAKE_STRIP ${SELF_PACKER_FOR_EXECUTABLE})
endif()

include(conan)

conan_cmake_configure(
    REQUIRES fmt/9.1.0
    REQUIRES pybind11/2.10.0
    REQUIRES qt/6.3.1
    IMPORTS "lib, *.so* -> ./conanlibs"
    OPTIONS fmt:shared=True
    OPTIONS qt:shared=True
    OPTIONS qt:with_pq=False
    OPTIONS qt:with_odbc=False
    OPTIONS qt:with_md4c=False
    GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
    BUILD missing
    REMOTE conancenter
    SETTINGS ${settings}
    SETTINGS build_type=Release
    SETTINGS compiler.cppstd=gnu20)

# Install Conan libraries
install(DIRECTORY
    ${CMAKE_BINARY_DIR}/conanlibs/
    DESTINATION bin)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(fmt REQUIRED)
find_package(Qt6 REQUIRED)

include_directories(SYSTEM ${fmt_INCLUDE_DIRS}
    SYSTEM ${Qt6_INCLUDE_DIRS})

add_subdirectory(src)

include(ProcessorCount)
ProcessorCount(CpuCount)

# This is the numbering of the RPM package itself, i.e. the version of the packaging and not the version of the content.
if($ENV{RPM_PACKAGE_RELEASE})
    set(CPACK_RPM_PACKAGE_RELEASE $ENV{RPM_PACKAGE_RELEASE})
else()
    set(CPACK_RPM_PACKAGE_RELEASE 1)
endif()

set(CPACK_THREADS ${CpuCount})
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME "BotTaws")
set(CPACK_PACKAGE_VENDOR "BotTaws")
set(CPACK_PACKAGE_CONTACT "bottaws@mathausmendel.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BotTaws Twitch Bot")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_COMPONENTS_ALL app)
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_RPM_PACKAGE_AUTOREQPROV "no ")
set(CPACK_STRIP_FILES TRUE)
set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/${PROJECT_NAME}")

include(CTest)
include(CPack)
