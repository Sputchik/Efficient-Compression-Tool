@echo off
call devcmd
if exist build ( rmdir /s /q build )
mkdir build
cd build

cmake -G Ninja -S ../src/ -B . -DCMAKE_C_FLAGS="/Qpar /GL /arch:AVX2 /O2 /Ob2 /Gw /Gy /fp:fast" -DCMAKE_CXX_FLAGS="/arch:AVX2 /O2 /Ob2 /Gw /Gy /fp:fast" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/dev/deps/" -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON -DZLIB_LIBRARY="C:/dev/deps/lib/zlib-ng.lib"
ninja -j4