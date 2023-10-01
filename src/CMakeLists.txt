add_library(pilar pilar.cpp)

add_executable(pilarc pilarc.cpp)

target_link_libraries(pilarc
  PRIVATE
  pilar
  )
