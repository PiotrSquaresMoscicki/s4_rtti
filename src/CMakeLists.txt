set(S4_RTTI_INCLUDE ${CMAKE_CURRENT_LIST_DIR})

# gather source files list
file(GLOB_RECURSE S4_RTTI_SRCS RELATIVE ${CMAKE_CURRENT_LIST_DIR}
    ${S4_RTTI_INCLUDE}/*.cpp 
    ${S4_RTTI_INCLUDE}/*.hpp 
    ${S4_RTTI_INCLUDE}/*.hpp)
GenerateSourceGoups("${S4_RTTI_SRCS}")

include(GenerateExportHeader)
add_library(${S4_RTTI_TARGET} SHARED ${S4_RTTI_SRCS})
generate_export_header(${S4_RTTI_TARGET})
target_compile_features(${S4_RTTI_TARGET} PUBLIC cxx_std_20)
target_include_directories(${S4_RTTI_TARGET} PUBLIC ${S4_RTTI_INCLUDE} ${PROJECT_BINARY_DIR})