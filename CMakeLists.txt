cmake_minimum_required(VERSION 3.13)
project(eeprog)

set(CMAKE_CXX_STANDARD 14)

add_executable(eeprog src/main.cpp include/gpio.h src/gpio.cpp src/eeprom.cpp)

add_library(bcm_host SHARED IMPORTED include/eeprom.h)
set_target_properties(bcm_host PROPERTIES IMPORTED_LOCATION /opt/vc/lib/libbcm_host.so)

target_include_directories(eeprog PRIVATE include /opt/vc/include)
target_link_libraries(eeprog bcm_host)
