# Locate Ice home (from orca robotics project)

# This module defines the following variables:
# ICE_FOUND : 1 if Ice is found, 0 otherwise
# ICE_HOME  : path where to find include, lib, bin, etc.
# ICE_INCLUDE_DIR
# ICE_LIBRARY_DIR
# ICE_SLICE_DIR
#
#
# ICEJ_FOUND : 1 if Ice for Java is found, 0 otherwise
# ICEJ_HOME  : path where to find Ice for Java archive

#
# Ice for C++
#

# Assumption: we look for Ice.h and assume that the rest is there.
# i.e. slice2cpp, libIce.so, etc.
# to be more robust we can look for all of those things individually.

# start with 'not found'
set(ICE_FOUND 0 CACHE BOOL "Do we have Ice?")

message(STATUS "ICE_HOME ${ICE_HOME}")
find_path( ICE_HOME_INCLUDE_ICE Ice.h
  # rational for this search order:
  #    source install w/env.var -> source install
  #    package -> package
  #    package + source install w/env.var -> source install
  #    package + source install w/out env.var -> package
  #
  # installation selected by user
  ${ICE_HOME}/include/Ice
  $ENV{ICE_HOME}/include/Ice
  /org/share/archive/SFB588_RefDist/Ice/include/Ice
  # debian package installs Ice here
  /usr/include/Ice
  # Test standard installation points: generic symlinks first, then standard dirs, newer first
  /opt/Ice/include/Ice
  /opt/Ice-4/include/Ice
  /opt/Ice-4.0/include/Ice
  /opt/Ice-3/include/Ice
  /opt/Ice-3.5/include/Ice
  /opt/Ice-3.4/include/Ice
  /opt/Ice-3.3/include/Ice
  # some people may manually choose to install Ice here
  /usr/local/include/Ice
  # windows
  C:/Ice-3.4.0-VC80/include/Ice
  C:/Ice-3.4.0/include/Ice
  C:/Ice-3.3.0-VC80/include/Ice
  C:/Ice-3.3.0/include/Ice
  )
# message( STATUS "DEBUG: Ice.h is apparently found in : ${ICE_HOME_INCLUDE_ICE}" )

# NOTE: if ICE_HOME_INCLUDE_ICE is set to *-NOTFOUND it will evaluate to FALSE
if( ICE_HOME_INCLUDE_ICE )

    set( ICE_FOUND 1 CACHE BOOL "Do we have Ice?" FORCE )

    # strip 'file' twice to get rid off 'include/Ice'
#     message( STATUS "DEBUG: ICE_HOME_INCLUDE_ICE=" ${ICE_HOME_INCLUDE_ICE} )
    get_filename_component( ICE_HOME_INCLUDE ${ICE_HOME_INCLUDE_ICE} PATH )
    #message( STATUS "DEBUG: ICE_HOME_INCLUDE=" ${ICE_HOME_INCLUDE} )
    get_filename_component( ICE_HOME ${ICE_HOME_INCLUDE} PATH CACHE )

    message( STATUS "Setting ICE_HOME to ${ICE_HOME}" )

    # include and lib dirs are easy
    set( ICE_INCLUDE_DIR ${ICE_HOME}/include )
    set( ICE_LIBRARY_DIR ${ICE_HOME}/lib )

	message( STATUS "Setting ICE_LIBRARY_DIR to ${ICE_LIBRARY_DIR}" )

    # Slice files are often installed elsewhere
    find_path( ICE_SLICE_ICE Communicator.ice
        # installation selected by user
        ${ICE_SLICE_DIR}/Ice
        $ENV{ICE_SLICE_DIR}/Ice
        # standard Ice installation puts here
        ${ICE_HOME}/slice/Ice
        # SFB588 location
        /org/share/archive/SFB588_RefDist/Ice/slice/Ice
        # debian package installs it here
        /usr/share/slice/Ice
        # slackware package installs it here
        /usr/share/Ice/slice/Ice
    )

    # NOTE: if ICE_SLICE_ICE is set to *-NOTFOUND it will evaluate to FALSE
    if( NOT ICE_SLICE_ICE )
        message( FATAL_ERROR "Ice was found but Slice file installation was not. Check Ice installation or specify ICE_SLICE_DIR manually." )
    endif()

    # strip 'file' once to get rid off 'Ice'
#     message( STATUS "DEBUG: ICE_SLICE_ICE=" ${ICE_SLICE_ICE} )
    get_filename_component( ICE_SLICE_DIR ${ICE_SLICE_ICE} PATH )
    message( STATUS "Slice files were found in ${ICE_SLICE_DIR}" )

    # some libs only care about IceUtil, we tell them to find IceUtil in the same place as Ice.
    set( ICEUTIL_HOME ${ICE_HOME} )
    message( STATUS "Setting ICEUTIL_HOME to ${ICEUTIL_HOME}" )

endif( ICE_HOME_INCLUDE_ICE )

#
# Ice for Java
#

# start with 'not found'
set( ICEJ_FOUND 0 CACHE BOOL "Do we have Ice for Java?" )

# don't bother if Ice for C++ was not found
if( ICE_FOUND )

    find_path( ICEJ_HOME Ice.jar
    # installation selected by user
    $ENV{ICEJ_HOME}
    # debian package installs Ice here
    /usr/share/java
    # maybe user put it into the same place as Ice
    ${ICE_HOME}/lib
    )

    if( ICEJ_HOME )

        set( ICEJ_FOUND 1 CACHE BOOL "Do we have Ice?" FORCE )
        message( STATUS "Setting ICEJ_HOME to ${ICEJ_HOME}" )

    endif( ICEJ_HOME )

endif( ICE_FOUND )
