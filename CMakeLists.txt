

cmake_minimum_required(VERSION 3.22)

#General setup
set(PROJECT_NAME "Tessera")
set(CURRENT_VERSION "0.0.1")
set(PRODUCT_NAME "Tessera")
set(COMPANY_NAME "audioev")
set(BUNDLE_ID "com.audioev.Tessera")
set(FORMATS Standalone AU VST3)



project(Tessera VERSION 0.0.1)

add_subdirectory(modules/JUCE)

#project config
set(CMAKE_CXX_STANDARD 17)

#macos-specific settings
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment version" FORCE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Architectures" FORCE)
    set(CMAKE_XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET[arch=arm64] "11.0" CACHE STRING "arm 64 minimum deployment target" FORCE)
endif()

#linux-specific settings
#if(UNIX and CMAKE_SYSTEM_NAME STREQUAL "Linux")
#    find_package(PkgConfig REQUIRED)
#    pkg_check_modules(PkgConfig REQUIRED)
#
#    include_directories(${WEBKITZ_INCLUDE_DIRS})
#    link_directories(${WEBKITZ_LIBRARY_DIRS})
#    add_definitions(${WEBKITZ_CFLAGS_OTHER})
#endif()



#disable flags
add_definitions(-w)

#append module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules/cmake-include/sudara-cmake")

#windows-specific settings
#if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG;Debug>:Debug>" CACHE INTERNAL "")
#endif()

juce_add_plugin("${PROJECT_NAME}"
     COMPANY_NAME "${COMPANY_NAME}"                        # Specify the name of the plugin's author
     IS_SYNTH FALSE                       # Is this a synth or an effect?
     NEEDS_MIDI_INPUT FALSE               # Does the plugin need midi input?
     NEEDS_MIDI_OUTPUT FALSE              # Does the plugin need midi output?
     IS_MIDI_EFFECT FALSE                 # Is this plugin a MIDI effect?
#     EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
     COPY_PLUGIN_AFTER_BUILD FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Evnu              # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Tsra                            # A unique four-character plugin id with exactly one upper-case character
                                                   # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS "${FORMATS}"                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "${PRODUCT_NAME}"        # The name of the final executable, which can differ from the target name

)

add_compile_definitions(JUCE_PROJECT_NAME="${PROJECT_NAME}")

add_library(SharedCode INTERFACE)

target_compile_features(SharedCode INTERFACE cxx_std_20)

target_include_directories(SharedCode INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/modules/JUCE/modules"
        "${CMAKE_BINARY_DIR}/AudioPluginData"
)

target_compile_definitions(SharedCode INTERFACE
        JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
        JUCE_WEB_BROWSER=1
        JUCE_USE_CURl=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
        VERSION="${CURRENT_VERSION}"
        JUCE_DISPLAY_SPLASH_SCREEN=0
        PRODUCT_NAME_WITHOUT_VERSION="Tessera"
)



file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

target_sources("${PROJECT_NAME}" PRIVATE ${SourceFiles})

file(GLOB_RECURSE Assets "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.*")

juce_add_binary_data(AudioPluginData SOURCES ${Assets})

target_link_libraries(SharedCode INTERFACE AudioPluginData)

add_dependencies(${PROJECT_NAME} AudioPluginData)




# juce_add_binary_data(AudioPluginData SOURCES ...)


target_link_libraries("${PROJECT_NAME}" PRIVATE
        SharedCode
        AudioPluginData
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_product_unlocking
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_include_directories("${PROJECT_NAME}" PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)