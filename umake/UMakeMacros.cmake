# Copyright (c) 2015 Pavel Novy. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

function (add_module mid)
  # prepare target type
  set (target_type ${${mid}_TARGET_TYPE})
  if (NOT target_type)
    set (target_type STATIC) # static library is default
  endif ()

  # prepare sources
  set (sources ${${mid}_SOURCES})

  # handle custom deps generated sources
  if (${mid}_CUSTOM_DEPS)
    foreach (cust_dep ${${mid}_CUSTOM_DEPS})
      # append generated sources to target
      list (APPEND sources ${${cust_dep}_SOURCES})
      # XXX this causes files to be deleted upon clean!
      set_source_files_properties (${${cust_dep}_SOURCES} PROPERTIES
        GENERATED ON)
      if (OS_MAC)
        list (APPEND sources ${${cust_dep}_OUTPUTS})
        set_source_files_properties (${${cust_dep}_OUTPUTS} PROPERTIES
          MACOSX_PACKAGE_LOCATION Resources
          GENERATED ON)
      endif ()
    endforeach ()
  endif ()

  if (APPLE)
    # .app bundle icons
    if (target_type STREQUAL "app")
      set (icns_file ${CMAKE_SOURCE_DIR}/build/umake/resources/application.icns)
      if (${mid}_MACOSX_BUNDLE_ICON)
        set (icns_file ${${mid}_MACOSX_BUNDLE_ICON})
      endif ()
      list (APPEND sources ${icns_file})
      set_source_files_properties (${icns_file} PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources)
      if (${mid}_MAC_BUNDLE_RESOURCES)
        set_source_files_properties (${${mid}_MAC_BUNDLE_RESOURCES} PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources)
        list (APPEND sources ${${mid}_MAC_BUNDLE_RESOURCES})
      endif ()
    endif ()
  endif ()

  # create target
  if (target_type STREQUAL "STATIC"
      OR target_type STREQUAL "SHARED"
      OR target_type STREQUAL "OBJECT"
      OR target_type STREQUAL "MODULE")
    add_library (${mid} ${target_type} ${sources})
  else ()
    if (APPLE AND target_type STREQUAL "app")
      add_executable (${mid} MACOSX_BUNDLE ${sources} )
    else ()
      add_executable (${mid} ${sources})
    endif ()
  endif ()

  # prepare includes & defines
  set (dep_defines)
  set (dep_includes)
  foreach (dep ${${mid}_DEPS})
    list (APPEND dep_defines ${${dep}_PUBLIC_DEFINES})
    list (APPEND dep_includes ${${dep}_PUBLIC_INCLUDE_DIRS})
  endforeach ()
  #message ("${mid} - ${${mid}_DEPS}")

  # handle custom dependencies resources
  if ( (OS_WIN OR OS_LINUX) AND ${mid}_CUSTOM_DEPS)
    foreach (cust_dep ${${mid}_CUSTOM_DEPS})
        # create resource output directory
        if (${cust_dep}_OUTPUT_DIR)
          set (outdir "/${${cust_dep}_OUTPUT_DIR}")
        else ()
          unset (outdir)
        endif ()
        add_custom_command(TARGET ${mid} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/$<CONFIGURATION>/bin${outdir})

        # copy resources to place
        foreach (output ${${cust_dep}_OUTPUTS})
          add_custom_command(TARGET ${mid} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${output} ${CMAKE_BINARY_DIR}/$<CONFIGURATION>/bin${outdir})
        endforeach ()
    endforeach ()
  endif ()

  # include directories
  set (includes ${dep_includes} ${${mid}_PUBLIC_INCLUDE_DIRS} ${${mid}_INCLUDE_DIRS})
	if (includes)
		include_directories (${includes})
	endif ()
  #message ("${mid}\n${includes}\n")

  set (compile_flags
    "${${COMPILER}_COMPILE_FLAGS} ${${mid}_COMPILE_FLAGS_${COMPILER}}"
    )
  #message ("${mid} - ${compile_flags}")
	if (compile_flags)
		set_target_properties (${mid} PROPERTIES
      COMPILE_FLAGS "${compile_flags}")
	endif ()

	if (${mid}_LINK_FLAGS_${COMPILER})
    #message ("${mid} - ${${mid}_LINK_FLAGS_${COMPILER}}")
		set_target_properties (${mid} PROPERTIES
			COMPILE_FLAGS "${${mid}_LINK_FLAGS_${COMPILER}}")
	endif ()

  set (defines
    ${${OS}_DEFINES}
    ${dep_defines}
    ${${mid}_DEFINES}
    )

  #message("${mid} - ${defines}")
  if (defines)
    set_target_properties (${mid} PROPERTIES
      COMPILE_DEFINITIONS "${defines}"
      )
  endif ()

  # setup & link libraries and frameworks
  set (lib_deps ${${mid}_DEPS} ${${mid}_LIBS})
  set (lib_priv_deps ${${mid}_PRIV_DEPS})
	if (OS_MAC AND ${mid}_FRAMEWORKS)
		foreach (fmw ${${mid}_FRAMEWORKS})
      find_library (${fmw}_FMW ${fmw})
      list (APPEND lib_deps ${${fmw}_FMW})
		endforeach ()
	endif ()
  #message ("${mid} - ${lib_deps}")
  if ((lib_deps OR lib_priv_deps) AND NOT target_type STREQUAL "OBJECT"
       AND NOT target_type STREQUAL "MODULE")
    target_link_libraries (${mid}
      LINK_PUBLIC ${lib_deps}
      LINK_PRIVATE ${lib_priv_deps})
	endif ()

  set_module_output_directories (${mid} bin lib lib)

  if (APPLE AND target_type STREQUAL "app")
    set_target_properties (${mid} PROPERTIES
       MACOSX_BUNDLE_BUNDLE_NAME ${mid}
       MACOSX_BUNDLE_GUI_IDENTIFIER "${mid}"
       MACOSX_BUNDLE_ICON_FILE application.icns
       MACOSX_BUNDLE_INFO_STRING "${${mid}_MACOSX_BUNDLE_VERSION} ${mid}"
       MACOSX_BUNDLE_SHORT_VERSION_STRING "${${mid}_MACOSX_BUNDLE_VERSION}"
       MACOSX_BUNDLE_LONG_VERSION_STRING "${${mid}_MACOSX_BUNDLE_VERSION}"
       MACOSX_BUNDLE_BUNDLE_VERSION "${${mid}_MACOSX_BUNDLE_VERSION}"
       MACOSX_BUNDLE_COPYRIGHT "${PROJECT_COPYRIGHT}"
      )
    if (${mid}_MACOSX_PLIST)
      set_target_properties(${mid} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${${mid}_MACOSX_PLIST})
    endif ()
    if (${mid}_MACOSX_BUNDLE_NAME)
      set_target_properties(${mid} PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME ${${mid}_MACOSX_BUNDLE_NAME})
    endif ()
  endif ()
endfunction ()

function (set_module_output_directories mid bin arch lib)
  if (MSVC)
    set (cfgs ${CMAKE_CONFIGURATION_TYPES})
    foreach (cfg ${cfgs})
      string (TOUPPER "${cfg}" cfg_upper)
      set (cfg_build_dir ${CMAKE_BINARY_DIR}/${cfg})
      set_target_properties (${mid} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${cfg_upper} ${cfg_build_dir}/${bin}
        ARCHIVE_OUTPUT_DIRECTORY_${cfg_upper} ${cfg_build_dir}/${arch}
        LIBRARY_OUTPUT_DIRECTORY_${cfg_upper} ${cfg_build_dir}/${lib}
        )
    endforeach ()
  else ()
    set_target_properties (${mid} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${bin}"
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${arch}"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${lib}"
      )
  endif ()
endfunction ()
