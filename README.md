# 3D Renderer from Scratch

This is a custom 3D Software Renderer built completely from scratch in **C++**. 

It is based on the mathematical principles taught in Gabriel Gambetta's book, *"Computer Graphics from Scratch"*. Instead of relying on a hardware GPU API like OpenGL or Vulkan to do the math under the hood, this project calculates the geometry, lighting, reflections, and perspective manually on the CPU.

## 🛠️ Built With

*   **Language:** Standard `C++17`
*   **Build System:** `CMake`
*   **Windowing library:** `SDL2` (Simple DirectMedia Layer - used exclusively to create an empty canvas window and push pixels to the screen)

## 🚀 Getting Started

This project is built to run easily right out of the box using Visual Studio and CMake.

### Prerequisites

*   Microsoft Visual Studio 2022 (with the "Desktop development with C++" workload installed)
*   Alternatively: Any standard C++ compiler with CMake 3.16+ installed.

### Build Instructions in Visual Studio
1. Open Visual Studio 2022.
2. Select **"Open a local folder"** and choose this repository's folder.
3. CMake will automatically read the `CMakeLists.txt` and download/compile the required `SDL2` library.
4. Select `renderer.exe` as your launch target at the top of the IDE and press **Play** (the green arrow).

### The "PutPixel" Magic
The core of this renderer relies on translating 3D coordinates into a 2D screen coordinate. The custom `PutPixel(x, y, color)` function sits inside `main.cpp`. It adjusts standard computer screen coordinates (top-left origin) to standard mathematical graph coordinates (center origin) so the algorithms in the book function correctly.
