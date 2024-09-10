cmake_minimum_required(VERSION 3.5.0)

project(Tower_of_Hanoi VERSION 0.1.0.0)

add_executable(${PROJECT_NAME} main.cpp)

# Specifies C++ Standard (cxx_std_20 = C++20)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# -g adds allows you to debug the program.
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
        ${PROJECT_NAME}_DEBUG=1
        >
    )
# -02 is a common flag for optimizations.
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -O2")
endif()


target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})
