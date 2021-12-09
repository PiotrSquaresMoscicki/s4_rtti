# compile, build, test

mkdir build
cd build
cmake ..
make -j16
if [ $? = 0 ]
then
    ctest --output-on-failure
fi