# Pathtracer
## build (CMake)
For mac or linux:
```
mkdir build
cd build
cmake ..
make
```
On windows, ```make``` should be replaced with ```nmake```
## usage
for n samples per pixel and m bounces (default: 200 spp and 2 bounces)
```
./better n m
```
once it finishes rendering it outputs to ```outimage.png```
