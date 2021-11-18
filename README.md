## Important:
Most of it is in https://docs.conan.io/en/latest/getting_started.html
But here's TL;DR
- You need conan and cmake
1. Create build/ dir
2. Move to build/
3. Run ```conan install .. --build=missing```
4. Run ```cmake ..```
5. Run ```cmake --build .```