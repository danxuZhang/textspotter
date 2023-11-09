set(SOURCE_FILES src/textspotter.cpp)

include_directories("include/")

add_library(textspotter SHARED ${SOURCE_FILES})

target_link_libraries(textspotter PRIVATE ${OpenCV_LIBS} ${Tesseract_LIBRARIES} leptonica fmt::fmt)
target_include_directories(textspotter INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
