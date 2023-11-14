set(THIS TextSpotterCBind)

set(SOURCE_FILES src/c_bind.cpp)

include_directories("include/")

add_library(${THIS} SHARED ${SOURCE_FILES})

target_link_libraries(${THIS} PRIVATE ${OpenCV_LIBS} ${Tesseract_LINK_LIBRARIES} leptonica fmt::fmt libtextspotter)
target_include_directories(${THIS} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")