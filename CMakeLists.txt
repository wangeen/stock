project(test)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE on)

### cude
FIND_PACKAGE(CUDA REQUIRED)  
if(CUDA_FOUND)  
    message("CUDA FOUND")  
endif()  



include_directories(./common)

set(server "a_server")
set(client "a_client")

file(GLOB_RECURSE server_src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/*.cpp)
file(GLOB_RECURSE cuda_src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/server/*.cu)
file(GLOB_RECURSE client_src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/client/*.cpp)
file(GLOB_RECURSE common_src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp)

# add_executable(${server} ${server_src} ${common_src})
add_executable(${client} ${client_src} ${common_src})

CUDA_ADD_EXECUTABLE(${server} ${server_src} ${common_src} ${cuda_src})

# nvcc flags                                                                                                                                                                                               
#set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_20,code=sm_20)
#cuda_compile(HELPER_O helper.cu)
#cuda_compile(DRIVER_O driver.cu OPTIONS -G)
#cuda_add_executable(driver ${HELPER_O} ${DRIVER_O})


find_library(lib_pcap NAMES pcap)
message(${lib_pcap})
target_link_libraries(${server}
        ${lib_pcap}
    )


