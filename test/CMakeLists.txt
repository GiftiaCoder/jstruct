set(TESTER test)
add_executable(${TESTER} "${TESTER}.cpp")
target_link_libraries(${TESTER} ${JSONCPPLIB})
