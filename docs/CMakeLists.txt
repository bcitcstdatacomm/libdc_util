set(DOXYGEN_EXTRACT_ALL YES)

doxygen_add_docs(docs dc_util/libdc_util "${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/include")
