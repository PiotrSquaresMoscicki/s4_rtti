# compile, build, test

mkdir build
cd build
cmake ..
cmake --build .
if [ $? = 0 ]
then
    ctest --output-on-failure
fi