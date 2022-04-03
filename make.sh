#You should copy this file (`make.sh`) to the `build` directory.
cmake .. && make
rm ../scripts/*.so
cp *.so ../scripts