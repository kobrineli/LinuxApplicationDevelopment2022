cmake_minimum_required(VERSION 3.24)
project(guesser)

add_executable(guesser guesser.c)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/ru/LC_MESSAGES/guesser.mo
    COMMAND rm -rf ru && mkdir -p ru/LC_MESSAGES
    COMMAND xgettext --keyword=_ --language=C --add-comments --sort-output -o
            ${CMAKE_BINARY_DIR}/guesser.pot ${PROJECT_SOURCE_DIR}/guesser.c
    COMMAND msgmerge -o ${CMAKE_BINARY_DIR}/guesser.po
            ${PROJECT_SOURCE_DIR}/guesser.po ${CMAKE_BINARY_DIR}/guesser.pot
    COMMAND msgfmt -o ${CMAKE_BINARY_DIR}/ru/LC_MESSAGES/guesser.mo
            ${CMAKE_BINARY_DIR}/guesser.po
    DEPENDS ${PROJECT_SOURCE_DIR}/guesser.c
)

add_custom_target(translation ALL
    DEPENDS ${CMAKE_BINARY_DIR}/ru/LC_MESSAGES/guesser.mo)

add_dependencies(guesser translation)
