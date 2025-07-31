@echo off

:: Runs windowed game instance. Used for test online functionality localy


set CONFIG_FILE_NAME=\DevOps\CleanAndRegenerate\config.bat

set PROJECT_DIR=%cd%
echo Project dir: %PROJECT_DIR%

set CONFIG_FILE_PATH=%PROJECT_DIR%%CONFIG_FILE_NAME%
echo Config File: %PROJECT_DIR%%CONFIG_FILE_NAME%
call %CONFIG_FILE_PATH%

"%UE_ENGINE_PATH%\Binaries\Win64\UnrealEditor.exe" "%PROJECT_DIR%\%PROJECT_PURE_NAME%.uproject" -game -ResX=960 -ResY=540 -log -WINDOWED