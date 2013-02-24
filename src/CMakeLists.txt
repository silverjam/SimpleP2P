project (SimpleP2P)

find_package ( Boost COMPONENTS system REQUIRED )
find_package ( Threads REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(simplep2p simplep2p.cxx client.cxx)

TARGET_LINK_LIBRARIES(
  simplep2p
  ${Boost_LIBRARIES} 
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)
