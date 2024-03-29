set(S4_RTTI_INCLUDE ${CMAKE_CURRENT_LIST_DIR})

# gather source files list
file(GLOB_RECURSE S4_RTTI_SRCS RELATIVE ${CMAKE_CURRENT_LIST_DIR}
    ${S4_RTTI_INCLUDE}/*.cpp 
    ${S4_RTTI_INCLUDE}/*.hpp)
GenerateSourceGoups("${S4_RTTI_SRCS}")

file(GLOB_RECURSE S4_RTTI_HEADERS RELATIVE ${CMAKE_CURRENT_LIST_DIR} 
    ${S4_RTTI_INCLUDE}/*.hpp)

include(GenerateExportHeader)
add_library(${S4_RTTI_TARGET} SHARED ${S4_RTTI_SRCS})
generate_export_header(${S4_RTTI_TARGET})
target_compile_features(${S4_RTTI_TARGET} PUBLIC cxx_std_20)
target_link_libraries(${S4_RTTI_TARGET} PUBLIC s4_core)
#target_include_directories(${S4_RTTI_TARGET} PUBLIC ${S4_RTTI_INCLUDE} ${PROJECT_BINARY_DIR})

target_include_directories(${S4_RTTI_TARGET} PUBLIC
    $<BUILD_INTERFACE:${S4_RTTI_INCLUDE}>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
    $<INSTALL_INTERFACE:include>)

#install(TARGETS ${S4_RTTI_TARGET} EXPORT ${S4_RTTI_TARGET} DESTINATION lib)
#install(FILES ${S4_RTTI_HEADERS} DESTINATION include)