# Pathtracer
![Image of output](https://github.com/UsedHandle/Pathtracer/blob/main/500spp.png?raw=true)
## build (CMake which fetches assimp and glm)
For mac or linux:
```
mkdir build
cd build
cmake .. -D CMAKE_POLICY_VERSION_MINIMUM=3.5
make
```
On windows, ```make``` can be replaced with compilation through Visual Studio after setting better as the startup project usage (make sure assets is in the current directory)
for n samples per pixel, m bounces, AxB image (default: 8 spp and 5 bounces at 1024x768)
```
./better n m A B
```
or just
```
./better
```
once it finishes rendering it outputs to ```outimage.png```
