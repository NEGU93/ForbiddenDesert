# - Find allegro 5
# Find the native ALLEGRO 5 includes and library
#
#  ALLEGRO_INCLUDE_DIR - where to find allegro.h, etc.
#  ALLEGRO_LIBRARIES   - List of libraries when using allegro.
#  ALLEGRO_FOUND       - True if allegro found.


find_package(PkgConfig)

# Find main allegro and assume the rest is there too
pkg_check_modules(Allegro5_PKGCONF allegro-5)

# Perhaps use this later?
# pkg_search_module(ALLEGRO5 REQUIRED allegro-5 allegro-5.0)

# MESSAGE("lib: ${Allegro5_PKGCONF_LIBRARY_DIRS}")
# MESSAGE("include: ${Allegro5_PKGCONF_INCLUDE_DIRS}")

# Include dir
find_path(Allegro5_INCLUDE_DIR
        NAMES allegro5/allegro5.h
        PATHS ${Allegro5_PKGCONF_INCLUDE_DIRS}
        )

# message("include dir: ${Allegro5_INCLUDE_DIR}")

# Names of all libraries in Allegro, without versions
set(ALLEGRO5_ALL_LIBRARIES
        allegro allegro_image allegro_font
        allegro_primitives allegro_ttf allegro_audio
        allegro_dialog allegro_memfile allegro_acodec
        allegro_color allegro_main allegro_physfs
        )

# set(ALLEGRO5_LIBRARIES "")

# Find all libraries to link
foreach(ALLEGRO5_ONE_LIBRARY ${ALLEGRO5_ALL_LIBRARIES})
    # message("${ALLEGRO5_ONE_LIBRARY}")
    find_library("${ALLEGRO5_ONE_LIBRARY}_AID" "${ALLEGRO5_ONE_LIBRARY}"
            ${Allegro5_PKGCONF_LIBRARY_DIRS}
            )
    set(Allegro5_LIBRARIES
            "${Allegro5_LIBRARIES}" "${${ALLEGRO5_ONE_LIBRARY}_AID}")
    # MESSAGE("${Allegro5_LIBRARIES}")
endforeach(ALLEGRO5_ONE_LIBRARY)
# Remove first empty erroneous "library"
list(REMOVE_AT Allegro5_LIBRARIES 0)
# MESSAGE("${Allegro5_LIBRARIES}")
# message("${ALLEGRO5_LIBRARIES}")
# allegro_image allegro_font allegro_primitives
#   allegro_ttf allegro_audio allegro_dialog allegro_acodec
#   allegro_color allegro_main allegro_physfs



# Set the include dir variables and the libraries and let libfind_process do the rest.
set(ALLEGRO_INCLUDE_DIR "${Allegro5_INCLUDE_DIR}")
set(ALLEGRO_LIBRARIES "${Allegro5_LIBRARIES}")
# libfind_process(Allegro5)

# MESSAGE("all libs: ${ALLEGRO_LIBRARIES}")
# MESSAGE("all include: ${ALLEGRO_INCLUDE_DIR}")

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Allegro5 DEFAULT_MSG ALLEGRO_LIBRARIES ALLEGRO_INCLUDE_DIR)

MARK_AS_ADVANCED(ALLEGRO_INCLUDE_DIR ALLEGRO_LIBRARIES)
