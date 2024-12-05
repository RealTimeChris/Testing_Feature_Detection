# CMakeList.txt : CMake project for Testing_Feature_Detection, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Testing_Feature_Detection "Testing_Feature_Detection.cpp" "Testing_Feature_Detection.h")

set_property(TARGET Testing_Feature_Detection PROPERTY CXX_STANDARD 20)

# TODO: Add tests and install targets if needed.
