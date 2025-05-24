@echo off
setlocal

:: Navigate to the test directory
cd /d "%~dp0\build\tests" || (
    echo Failed to change directory to build/tests. Ensure the path exists.
    exit /b 1
)

:: Run ctest
ctest --output-on-failure

:: Return to the original directory
cd /d "%~dp0"
endlocal
@echo on
