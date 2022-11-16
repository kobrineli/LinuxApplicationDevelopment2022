cmake_minimum_required(VERSION 3.24)
project(rhasher)

add_executable(rhasher rhasher.c)

find_library(LIBRHASH rhash)
if (NOT LIBRHASH)
    message(FATAL_ERROR "Library ${LIBRHASH} not found.")
endif()

find_library(LIBREADLINE readline)
if (NOT LIBREADLINE)
    message("Library ${LIBREADLINE} not found.")
endif()

target_link_libraries(rhasher ${LIBRHASH})
if ("${READLINE}" STREQUAL "1" AND LIBREADLINE)
    target_link_libraries(rhasher ${LIBRHASH} ${LIBREADLINE})
    add_definitions(-DREADLINE)
endif()

file(COPY tests DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

enable_testing()
add_test(test_sha1 tests/test_sha1.sh)
add_test(test_md5 tests/test_md5.sh)
