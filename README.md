# Pathtracer
## build (CMake)
For mac or linux:
```
mkdir build
cd build
cmake ..
make
```
On windows, ```make``` can be replaced with compilation through Visual Studio as cmake generates the sln file
## usage
for n samples per pixel and m bounces (default: 200 spp and 2 bounces)
```
./better n m
```
once it finishes rendering it outputs to ```outimage.png```
