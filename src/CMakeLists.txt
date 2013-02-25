project (SimpleP2P)

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-std=c++0x HAS_GNU_CXX11)

if (HAS_GNU_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

find_package ( Boost COMPONENTS system REQUIRED )
find_package ( Threads REQUIRED )

add_definitions ( -DDEBUG_LOGGING )

include_directories ( ${Boost_INCLUDE_DIR} )

file ( GLOB simplep2p_srcs
    simplep2p.cxx 
    client.cxx
    sender.cxx
    receiver.cxx
    )

add_executable(
    simplep2p-cli
    main.cxx 
    )

add_library(
    simplep2p 
    ${simplep2p_srcs}
    )

TARGET_LINK_LIBRARIES(
  simplep2p-cli
  simplep2p
  ${Boost_LIBRARIES} 
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)
