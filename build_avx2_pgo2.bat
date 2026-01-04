@echo off
call devcmd

set "BUILD_DIR=build_avx2_pgo"
cd "%BUILD_DIR%"

cmake -G Ninja -S ../src/ -B . -DCMAKE_C_FLAGS="/Qpar /GL /arch:AVX2 /O2 /Ob2 /Gw /Gy /fp:fast" -DCMAKE_CXX_FLAGS="/Qpar /GL /arch:AVX2 /O2 /Ob2 /Gw /Gy /fp:fast" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/dev/deps/" -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON -DCMAKE_EXE_LINKER_FLAGS="/LTCG:PGOPTIMIZE"

ninja -j4
cd ..