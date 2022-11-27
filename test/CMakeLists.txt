set(TESTER test_load_normal)
add_executable(${TESTER} "${TESTER}.cpp")
target_link_libraries(${TESTER} ${JSONCPPLIB})
add_test(NAME ${TESTER} COMMAND ${TESTER})

set(TESTER test_load_ignore_missing)
add_executable(${TESTER} "${TESTER}.cpp")
target_link_libraries(${TESTER} ${JSONCPPLIB})
add_test(NAME ${TESTER} COMMAND ${TESTER})

set(TESTER test_load_missing_field)
add_executable(${TESTER} "${TESTER}.cpp")
target_link_libraries(${TESTER} ${JSONCPPLIB})
add_test(NAME ${TESTER} COMMAND ${TESTER})

set(TESTER test_load_ignore_error)
add_executable(${TESTER} "${TESTER}.cpp")
target_link_libraries(${TESTER} ${JSONCPPLIB})
add_test(NAME ${TESTER} COMMAND ${TESTER})

set(TESTER test_load_error_field)
add_executable(${TESTER} "${TESTER}.cpp")
target_link_libraries(${TESTER} ${JSONCPPLIB})
add_test(NAME ${TESTER} COMMAND ${TESTER})
