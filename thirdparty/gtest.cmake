if(NOT TARGET gtest)
    set(BUILD_GMOCK OFF)
    add_subdirectory(./googletest)
endif()