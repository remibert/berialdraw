# Doxygen documentation target
find_package(Doxygen REQUIRED doxygen)

# Define paths
set(DOXYGEN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/delivery/doc")
set(DOXYGEN_INPUT_DIR "${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/resources/colors ${CMAKE_SOURCE_DIR}/resources/styles/pearl")

# Create output directory
file(MAKE_DIRECTORY "${DOXYGEN_OUTPUT_DIR}")

# Configure doxygen.dox template
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/doxygen.dox.in"
    "${CMAKE_CURRENT_BINARY_DIR}/doxygen.dox"
    @ONLY
)

# Create custom target for documentation - include in default build (ALL)
add_custom_target(
    docs ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Generating Doxygen documentation..."
    COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/doxygen.dox"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating API documentation with Doxygen -> ${DOXYGEN_OUTPUT_DIR}"
    VERBATIM
)
