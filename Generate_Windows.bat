SET HG_PLATFORM_NAME=Windows
SET PREMAKE_PATH=%~dp0Tools\Premake\Windows\premake5.exe
%PREMAKE_PATH% --file=%~dp0Makefiles\premake5.lua --os=windows vs2022
pause