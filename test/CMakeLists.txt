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

add_executable(test_003 test_003.cpp)

target_link_libraries(test_003
  PRIVATE
  pilar
  )

add_executable(test_004 test_004.cpp)

target_link_libraries(test_004
  PRIVATE
  pilar
  )

add_executable(dag_002 dag_002.cpp)

target_link_libraries(dag_002
  PRIVATE
  dag
  )
