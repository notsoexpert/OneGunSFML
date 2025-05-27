cmake_minimum_required(VERSION 3.20) # setting this is required
project(OneGunSFMLProject)            # this sets the project name

###############################################################################
## file globbing ##############################################################
###############################################################################

# These instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources` and `data`.
file(GLOB_RECURSE sources      src/main/*.cpp src/main/*.hpp src/main/actors/*.hpp src/main/actors/*.cpp src/main/system/*.hpp src/main/system/*.cpp)
file(GLOB_RECURSE sources_test src/test/*.cpp)
file(GLOB_RECURSE data assets/*)
# You can use set(sources src/main.cpp) etc if you don't want to
# use globbing to find files automatically.

###############################################################################
## target definitions #########################################################
###############################################################################

# The data is just added to the executable, because in some IDEs (QtCreator) 
# files are invisible when they are not explicitly part of the project.
add_executable(OneGunSFML ${sources} ${data})

target_precompile_headers(OneGunSFML PRIVATE src/main/pch.hpp)

# Just for example add some compiler flags.
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(OneGunSFML PUBLIC -std=c++23 -stdlib=libc++ -Wall -Wextra -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(OneGunSFML PUBLIC -std=c++23 -Wall -Wextra -Werror)
elseif(MSVC)
  target_compile_options(OneGunSFML PUBLIC /std:c++23 /W4 /WX /permissive- /Zc:__cplusplus)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  target_compile_options(OneGunSFML PUBLIC -std=c++23 -Wall -Wextra -Werror)
endif()

# This allows to include files relative to the root of the src directory with a <> pair
target_include_directories(OneGunSFML PUBLIC src/main)
target_include_directories(OneGunSFML PUBLIC vendor/SFML/include)
target_include_directories(OneGunSFML PUBLIC vendor/spdlog/include)
target_include_directories(OneGunSFML PUBLIC vendor/glm/glm)
target_include_directories(OneGunSFML PUBLIC vendor/entt/single_include)


# This copies all resource files in the build directory.
# We need this, because we want to work with paths relative to the executable.
file(COPY ${data} DESTINATION assets)

###############################################################################
## dependencies ###############################################################
###############################################################################

# This defines the variables that containts all library names
# that we need to link into the program.
add_subdirectory(vendor/SFML)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/entt)
#find_package(SFML COMPONENTS window graphics audio REQUIRED)
#find_package(spdlog REQUIRED)
#find_package(glm REQUIRED)

target_link_libraries(OneGunSFML PUBLIC
  sfml-window
  sfml-graphics
  sfml-audio
  sfml-system
  spdlog
  glm
  EnTT
)



###############################################################################
## testing ####################################################################
###############################################################################

# This is for our testing framework,
# we don't add REQUIRED because it's just for testing.
# People who might want to build the project to use it should not be required
# to install testing dependencies.
#find_package(GTest)

if(GTEST_FOUND)
  add_executable(unit_tests ${sources_test} ${sources})

  # This define is added to prevent collision with the main.
  # It might be better solved by not adding the source with the main to the
  # testing target.
  target_compile_definitions(unit_tests PUBLIC UNIT_TESTS)

  # This allows us to use the executable as a link library, and inherit all 
  # linker options and library dependencies from it, by simply adding it as dependency.
  set_target_properties(OneGunSFML PROPERTIES ENABLE_EXPORTS on)

  target_link_libraries(unit_tests PUBLIC
    ${GTEST_BOTH_LIBRARIES}
    OneGunSFML
  )

  target_include_directories(unit_tests PUBLIC
    ${GTEST_INCLUDE_DIRS} # doesn't do anything on linux
  )
  
endif()

###############################################################################
## packaging ##################################################################
###############################################################################

# All install commands get the same destination. this allows us to use paths
# relative to the executable.
install(TARGETS OneGunSFML DESTINATION C:/Dev/OneGunSFML)

# This is basically a repeat of the file copy instruction that copies the
# resources in the build directory, but here we tell cmake that we want it
# in the package.
install(DIRECTORY assets DESTINATION C:/Dev/OneGunSFML)

# Now comes everything we need, to create a package
# there are a lot more variables you can set, and some
# you need to set for some package types, but we want to
# be minimal here.
set(CPACK_PACKAGE_NAME "OneGunSFML")
set(CPACK_PACKAGE_VERSION "1.0.0")

# We don't want to split our program up into several incomplete pieces.
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)