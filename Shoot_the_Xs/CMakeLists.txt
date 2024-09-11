# cmake -S . -B ./debug -D CMAKE_BUILD_TYPE=Debug -G"Unix Makefiles" --preset=AppleClangDebugFlags
# cmake -S . -B ./release -D CMAKE_BUILD_TYPE=Release -G"Unix Makefiles" --preset=AppleClangReleaseFlags

# Don't even think about release builds until Beta testing.
# Press F5 to launch a debug session, don't execute the program directly.

cmake_minimum_required(VERSION 3.5.0)

project(
    Shoot_the_Xs
    VERSION 0.1.2.0
)

# needed for make install
configure_file(version.config version_config.h)

add_subdirectory(src)

# Specifies C++ Standard (cxx_std_20 = C++20)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

# ~ % sudo make install, or drag and drop into usr/local/bin
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/version_config.h" DESTINATION include)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/UNLICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
include(CPack)
