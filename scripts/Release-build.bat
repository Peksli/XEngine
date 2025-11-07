@echo off

cd ../build-artifacts
cmake ..
cmake --build . --config Release