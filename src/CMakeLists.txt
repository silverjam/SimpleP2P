project (SimpleP2P)

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-std=c++0x HAS_GNU_CXX11)

if (HAS_GNU_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

find_package ( Boost COMPONENTS system REQUIRED )
find_package ( Threads REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(
    simplep2p 
    simplep2p.cxx 
    client.cxx
    sender.cxx
    receiver.cxx
    )

TARGET_LINK_LIBRARIES(
  simplep2p
  ${Boost_LIBRARIES} 
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)
