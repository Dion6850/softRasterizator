# Soft-Resterizator（软光栅渲染器）
This project uses simple functions of FreeGLUT to draw a single point on the canvas, while performing other calculations on the CPU. It is aimed at learning how to create a software rasterizer.

[project repo](https://git.zhywyt.me/Rendering/softResterizator.git)
## Quick Start
```cmd
vcpkg install freeglut
mkdir build
cd build
cmake .. -DOOLCHAIN_FILE=<path to your vcpkg root>/scripts/buildsystems/vcpkg.cmake
```

## Project tree

```bash
|-- CMakeLists.txt
|-- README.md
|-- include
|   |-- test
|   |   |-- test1.h
|   |   `-- test2.h
|   `-- utils.h
|-- main.cpp
|-- thirdPart
|   `-- eigen-3.4.0
|       `-- Eigen

```

## test example

In your test file at `include/test/your_test_name.h`:
```cpp
#pragma once
#include <utils.h>
namespace your_test_name{
    void task(){
        // ... TODO ...
    }
}
```
Then you can call it at the main display function at file `main.cpp`:
```cpp
// something others ...
void disaplay(){
     /*
    ################## just can draw points ##################
    */
   your_test_name::task();
    /*
    ################## just can draw points ##################
    */
}
// something anothers ...
```

## code format
C++ format vscode setting file:
```json
"C_Cpp.vcFormat.newLine.beforeElse": false,
"C_Cpp.vcFormat.newLine.beforeCatch": false,
"C_Cpp.vcFormat.newLine.beforeOpenBrace.block": "sameLine",
"C_Cpp.vcFormat.newLine.beforeOpenBrace.function": "sameLine",
"C_Cpp.vcFormat.newLine.beforeOpenBrace.lambda": "sameLine",
"C_Cpp.vcFormat.newLine.beforeOpenBrace.namespace": "newLine",
"C_Cpp.vcFormat.newLine.beforeOpenBrace.type": "sameLine",
"C_Cpp.formatting": "vcFormat"
```