if not exist build_device mkdir build_device
cd build_sim
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
cmake .. -G "NMake Makefiles" --toolchain=%PLAYDATE_SDK_PATH%/C_API/buildsupport/arm.cmake -DCMAKE_BUILD_TYPE=Release
nmake