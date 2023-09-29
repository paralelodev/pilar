add_executable(test_001 test_001.cpp)

target_link_libraries(test_001
  PRIVATE
  pilar
  )

add_executable(test_002 test_002.cpp)

target_link_libraries(test_002
  PRIVATE
  pilar
  )
