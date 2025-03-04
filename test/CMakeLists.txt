add_custom_target(test python3 ${CMAKE_CURRENT_SOURCE_DIR}/lit-runner.py -v -s --path=${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR} DEPENDS abicorn USES_TERMINAL)
