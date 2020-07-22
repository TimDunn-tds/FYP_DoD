@echo off
path = %path%;%CD%\toolchain\gcc-arm-none-eabi\bin;%CD%\toolchain\OpenOCD\0.10.0-7-20180123-1217\bin;%CD%\toolchain\mingw\bin;%CD%\toolchain\gnuwin32\bin;%CD%\util\PortableGit\bin;%CD%\util\WinMerge;%CD%\util\npp;%CD%\toolchain\mingw\opt\bin;
title MCHA3500 STM32 development shell
echo.
%SystemRoot%\system32\cmd.exe  /K echo MCHA3500 STM32 development shell is ready