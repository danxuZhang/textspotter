set(THIS InteractiveMatch)

set(SOURCE_FILES main.cpp)

add_executable(${THIS} ${SOURCE_FILES})

target_link_libraries(${THIS} argparse::argparse fmt::fmt libtextspotter)