# Machine Coding LLD

A C++20 workspace for practising low-level design and machine-coding problems. Each problem can
keep its implementation, runnable demo, tests, and design notes together under `problems/`.

## Tooling

- GCC and GDB for compiling and debugging
- CMake with Ninja for fast, portable builds
- CTest for dependency-free automated tests
- clang-format-compatible formatting rules
- VS Code tasks, CMake integration, and debugger configuration
- GitHub Actions checks on Windows and Linux

## Build and run

Open a **new terminal** after installing the toolchain so the updated `PATH` is loaded.

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
./build/debug/problems/hello_lld/hello_lld_app.exe
```

For an optimized build, replace `debug` with `release`.

In VS Code, open this folder, install the recommended extensions, select the `debug` CMake preset,
and use `Ctrl+Shift+B` to build. Select `hello_lld_app` as the launch target before pressing `F5`.

## Add an LLD problem

1. Create `problems/<problem_name>/` with `include/`, `src/`, `app/`, and `tests/` folders.
2. Add that problem's library, executable, and test in its local `CMakeLists.txt`.
3. Add `add_subdirectory(problems/<problem_name>)` to the root `CMakeLists.txt`.
4. Copy [the design template](docs/problem-template.md) into the problem folder and complete it.
5. Build and test before committing.

The `hello_lld` problem is a working example of this structure and can be copied or replaced.

## Publish to GitHub

After creating an empty repository on GitHub:

```bash
git add .
git commit -m "Set up C++ LLD practice workspace"
git remote add origin https://github.com/<your-username>/machine_coding_lld.git
git push -u origin main
```

Build output and machine-specific files are excluded through `.gitignore`.
