set(SOURCE_FILES
        src/textspotter.cpp
        src/ocr.cpp
        src/utility.cpp
        src/text_detection.cpp
)

include_directories("include/")

add_library(libtextspotter ${SOURCE_FILES})

target_link_libraries(libtextspotter PRIVATE ${OpenCV_LIBS} ${Tesseract_LINK_LIBRARIES} leptonica fmt::fmt)
target_include_directories(libtextspotter INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
