cmake_minimum_required(VERSION 3.5.0)

project(Rock_Paper_Scissors VERSION 0.1.0.0)

add_executable(${PROJECT_NAME} main.cpp term.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
        ${PROJECT_NAME}_DEBUG=1
        >
    )
# -03 is an extreme set of optimizations and requires testing.
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -O3")
endif()


target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})
