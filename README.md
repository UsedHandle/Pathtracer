# Pathtracer
![Image of output](https://github.com/UsedHandle/Pathtracer/blob/main/500spp.png?raw=true)
## build (CMake)
For mac or linux:
```
mkdir build
cd build
cmake .. -D CMAKE_POLICY_VERSION_MINIMUM=3.5
make
```
On windows, ```make``` can be replaced with compilation through Visual Studio as cmake generates the sln file
## usage
for n samples per pixel, m bounces, AxB image (default: 8 spp and 5 bounces at 1024x768)
```
./better n m A B
```
once it finishes rendering it outputs to ```outimage.png```
