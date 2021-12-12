# A basic installer
set(MONIQUE_PRODUCT_DIR ${CMAKE_BINARY_DIR}/monique_products)
file(MAKE_DIRECTORY ${MONIQUE_PRODUCT_DIR})


add_custom_target(monique-installer)

function(monique_package format)
    get_target_property(output_dir MoniqueMonosynth RUNTIME_OUTPUT_DIRECTORY)

    if( TARGET MoniqueMonosynth_${format})
        add_dependencies(monique-installer MoniqueMonosynth_${format})
        add_custom_command(
                TARGET monique-installer
                POST_BUILD
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMAND echo "Installing ${output_dir}/${format} to ${MONIQUE_PRODUCT_DIR}"
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${output_dir}/${format}/ ${MONIQUE_PRODUCT_DIR}/
        )
    endif()
endfunction()

monique_package(VST3)
monique_package(VST)
monique_package(LV2)
monique_package(AU)
monique_package(Standalone)

add_custom_command(
        TARGET monique-installer
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND echo "Installing ${output_dir}/${format} to ${MONIQUE_PRODUCT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/LICENSE-gpl3 ${MONIQUE_PRODUCT_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/Installer/ZipReadme.txt ${MONIQUE_PRODUCT_DIR}/Readme.txt
)


find_package(Git)

if (Git_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE VERSION_CHUNK
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(VERSION_CHUNK "unknownhash")
endif ()

set(MONIQUE_ZIP MoniqueMonosynth-${VERSION_CHUNK}-${CMAKE_SYSTEM_NAME}.zip)
message( STATUS "Basic Installer: Target is installer/${MONIQUE_ZIP}")

add_custom_command(
        TARGET monique-installer
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory installer
        COMMAND ${CMAKE_COMMAND} -E tar cvf installer/${MONIQUE_ZIP} --format=zip ${MONIQUE_PRODUCT_DIR}/
)