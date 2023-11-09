set(THIS benchmark)

set(SOURCE_FILES main.cpp)

add_executable(${THIS} ${SOURCE_FILES})

target_link_libraries(${THIS} ${OpenCV_LIBS} argparse::argparse fmt::fmt textspotter)
