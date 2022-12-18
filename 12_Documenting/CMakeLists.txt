cmake_minimum_required(VERSION 3.24)
project(guesser)

add_executable(guesser guesser.c)

include(FindGettext)
include(FindDoxygen)

if (GETTEXT_FOUND)

    find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext)
    find_program(GETTEXT_MSMERGE_EXECUTABLE msgmerge)
    find_program(GETTEXT_MSGFMT_EXECUTABLE msgfmt)

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/ru/LC_MESSAGES/guesser.mo
        COMMAND rm -rf ru && mkdir -p ru/LC_MESSAGES
        COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} --keyword=_ --language=C --add-comments --sort-output -o
                ${CMAKE_BINARY_DIR}/guesser.pot ${PROJECT_SOURCE_DIR}/guesser.c
        COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} -o ${CMAKE_BINARY_DIR}/guesser.po
                ${PROJECT_SOURCE_DIR}/guesser.po ${CMAKE_BINARY_DIR}/guesser.pot
        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${CMAKE_BINARY_DIR}/ru/LC_MESSAGES/guesser.mo
                ${CMAKE_BINARY_DIR}/guesser.po
        DEPENDS ${PROJECT_SOURCE_DIR}/guesser.c
    )

    add_custom_target(translation ALL
        DEPENDS ${CMAKE_BINARY_DIR}/ru/LC_MESSAGES/guesser.mo)

    add_dependencies(guesser translation)
endif()

if (DOXYGEN_FOUND)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)

    doxygen_add_docs(
        docs
        ${PROJECT_SOURCE_DIR}
    )

    add_dependencies(guesser docs)
endif()
