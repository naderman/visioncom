# Locate IceUtil home (from orca robotics)
#
# This module defines the following variables:
# ICEUTIL_FOUND : 1 if Ice is found, 0 otherwise
# ICEUTIL_HOME  : path where to find include, lib, bin, etc.

# start with 'not found'
set( ICEUTIL_FOUND 0 CACHE BOOL "Do we have libIceUtil?" )

find_path( ICEUTIL_HOME_INCLUDE_ICEUTIL IceUtil.h
  # rational for this search order:
  #    source install w/env.var -> source install
  #    package -> package
  #    package + source install w/env.var -> source install
  #    package + source install w/out env.var -> package
  #
  # installation selected by user
  ${ICEUTIL_HOME}/include/IceUtil
  ${ICE_HOME}/include/IceUtil
  $ENV{ICEUTIL_HOME}/include/IceUtil
  $ENV{ICE_HOME}/include/IceUtil
  # SFB588 location
  /org/share/archive/SFB588_RefDist/Ice/include/IceUtil
  # debian package installs Ice here
  /usr/include/IceUtil
  # Test standard installation points: newer versions first
  /opt/Ice-4.0/include/IceUtil
  /opt/Ice-3.6/include/IceUtil
  /opt/Ice-3.5/include/IceUtil
  /opt/Ice-3.4/include/IceUtil
  /opt/Ice-3.3/include/IceUtil
  # some people may manually choose to install Ice here
  /usr/local/include/IceUtil
  # windows
  C:/Ice-3.2.0-VC80/include/IceUtil
  C:/Ice-3.2.0/include/IceUtil
  )
# message( STATUS "DEBUG: Ice.h is apparently found in : ${ICEUTIL_HOME_INCLUDE_ICE}" )

# NOTE: if ICEUTIL_HOME_INCLUDE_ICE is set to *-NOTFOUND it will evaluate to FALSE
if( ICEUTIL_HOME_INCLUDE_ICEUTIL )

    set( ICEUTIL_FOUND 1 CACHE BOOL "Do we have Ice?" FORCE )

    # strip 'file' twice to get rid off 'include/IceUtil'
#     message( STATUS "DEBUG: ICEUTIL_HOME_INCLUDE_ICE=" ${ICEUTIL_HOME_INCLUDE_ICE} )
    get_filename_component( ICEUTIL_HOME_INCLUDE ${ICEUTIL_HOME_INCLUDE_ICEUTIL} PATH )
#     message( STATUS "DEBUG: ICEUTIL_HOME_INCLUDE=" ${ICEUTIL_HOME_INCLUDE} )
    get_filename_component( ICEUTIL_HOME ${ICEUTIL_HOME_INCLUDE} PATH CACHE )
#     message( STATUS "Setting ICEUTIL_HOME to ${ICEUTIL_HOME}" )

endif( ICEUTIL_HOME_INCLUDE_ICEUTIL )

if( ICEUTIL_FOUND )
    message( STATUS "Looking for libIceUtil - found in ${ICEUTIL_HOME}")
else( ICEUTIL_FOUND )
    message( STATUS "Looking for libIceUtil - not found")
endif( ICEUTIL_FOUND )
