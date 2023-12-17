set(SOURCE_FILES
        src/textspotter.cpp
        src/ocr.cpp
        src/utility.cpp
        src/east_detector.cpp
        src/text_matching.cpp
        src/detect_read.cpp
)

include_directories("include/")

add_library(libtextspotter ${SOURCE_FILES})

if (PkgConfig_FOUND)
    target_link_libraries(libtextspotter PRIVATE ${OpenCV_LIBS} ${Tesseract_LINK_LIBRARIES} leptonica fmt::fmt)
else ()
    target_link_libraries(libtextspotter PRIVATE ${OpenCV_LIBS} Tesseract::libtesseract leptonica fmt::fmt)
endif ()

if (OpenMP_CXX_FOUND)
    target_link_libraries(libtextspotter PUBLIC OpenMP::OpenMP_CXX)
endif ()

target_include_directories(libtextspotter INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
