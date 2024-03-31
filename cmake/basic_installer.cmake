# A basic installer setup.
#
# This cmake file introduces two targets
#  monique-staged:      moves all the built assets to a well named directory
#  monique-installer:   depends on monique-staged, builds an installer
#
# Right now monique-installer builds just the crudest zip file but this is the target
# on which we will hang the proper installers later

set(MONIQUE_PRODUCT_DIR ${CMAKE_BINARY_DIR}/monique_products)
file(MAKE_DIRECTORY ${MONIQUE_PRODUCT_DIR})

add_custom_target(monique-staged)
add_custom_target(monique-installer)

function(monique_package format)
    get_target_property(output_dir MoniqueMonosynth RUNTIME_OUTPUT_DIRECTORY)

    if( TARGET MoniqueMonosynth_${format})
        add_dependencies(monique-staged MoniqueMonosynth_${format})
        add_custom_command(
                TARGET monique-staged
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
monique_package(CLAP)
monique_package(Standalone)

if (WIN32)
    message(STATUS "Including special windows cleanup installer stage")
    add_custom_command(TARGET monique-staged
            POST_BUILD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${CMAKE_COMMAND} -E echo "Cleaning up windows goobits"
            COMMAND ${CMAKE_COMMAND} -E rm -f "${MONIQUE_PRODUCT_DIR}/MoniqueMonosynth.exp"
            COMMAND ${CMAKE_COMMAND} -E rm -f "${MONIQUE_PRODUCT_DIR}/MoniqueMonosynth.ilk"
            COMMAND ${CMAKE_COMMAND} -E rm -f "${MONIQUE_PRODUCT_DIR}/MoniqueMonosynth.lib"
            COMMAND ${CMAKE_COMMAND} -E rm -f "${MONIQUE_PRODUCT_DIR}/MoniqueMonosynth.pdb"
            )
endif ()

add_dependencies(monique-installer monique-staged)

add_custom_command(
        TARGET monique-installer
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND echo "Installing ${output_dir}/${format} to ${MONIQUE_PRODUCT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/LICENSE-gpl3 ${MONIQUE_PRODUCT_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/resources/installer/ZipReadme.txt ${MONIQUE_PRODUCT_DIR}/Readme.txt
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

string(TIMESTAMP MONIQUE_DATE "%Y-%m-%d")
if (WIN32)
    set(MONIQUE_ZIP MoniqueMonosynth-${MONIQUE_DATE}-${VERSION_CHUNK}-${CMAKE_SYSTEM_NAME}-${BITS}bit.zip)
else ()
    set(MONIQUE_ZIP MoniqueMonosynth-${MONIQUE_DATE}-${VERSION_CHUNK}-${CMAKE_SYSTEM_NAME}.zip)
endif ()

if (APPLE)
    message(STATUS "Configuring for mac installer")
    add_custom_command(
            TARGET monique-installer
            POST_BUILD
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory installer
            COMMAND ${CMAKE_SOURCE_DIR}/libs/sst/sst-plugininfra/scripts/installer_mac/make_installer.sh "MoniqueMonosynth" ${MONIQUE_PRODUCT_DIR} ${CMAKE_SOURCE_DIR}/resources/installer_mac ${CMAKE_BINARY_DIR}/installer "${MONIQUE_DATE}-${VERSION_CHUNK}"
    )
elseif (WIN32)
    message(STATUS "Configuring for win installer")
    add_custom_command(
            TARGET monique-installer
            POST_BUILD
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory installer
            COMMAND 7z a -r installer/${MONIQUE_ZIP} ${MONIQUE_PRODUCT_DIR}/
            COMMAND ${CMAKE_COMMAND} -E echo "ZIP Installer in: installer/${MONIQUE_ZIP}")
    find_program(MONIQUE_NUGET_EXE nuget.exe PATHS ENV "PATH")
    if(MONIQUE_NUGET_EXE)
        message(STATUS "NuGet found at ${MONIQUE_NUGET_EXE}")
        add_custom_command(
            TARGET monique-installer
            POST_BUILD
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${MONIQUE_NUGET_EXE} install Tools.InnoSetup -version 6.2.1
            COMMAND Tools.InnoSetup.6.2.1/tools/iscc.exe /O"installer" /DMONIQUE_SRC="${CMAKE_SOURCE_DIR}" /DMONIQUE_BIN="${CMAKE_BINARY_DIR}" /DMyAppVersion="${MONIQUE_DATE}-${VERSION_CHUNK}" "${CMAKE_SOURCE_DIR}/resources/installer_win/monique${BITS}.iss")
    else()
        message(STATUS "NuGet not found")
    endif()
else ()
    message(STATUS "Basic Installer: Target is installer/${MONIQUE_ZIP}")
    add_custom_command(
            TARGET monique-installer
            POST_BUILD
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory installer
            COMMAND ${CMAKE_COMMAND} -E tar cvf installer/${MONIQUE_ZIP} --format=zip ${MONIQUE_PRODUCT_DIR}/
            COMMAND ${CMAKE_COMMAND} -E echo "Installer in: installer/${MONIQUE_ZIP}")
endif ()
