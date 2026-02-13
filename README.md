# kbox Library

A lightweight, modular C++ multimedia / game utility library built around GLFW, OpenGL and OpenAL. This repository contains a small engine core and utility libraries for rendering, audio, components and basic scene management.

## Repository layout

- **alUtils/** — OpenAL helpers (devices, buffers, sound manager).
- **glUtils/** — OpenGL helpers (shaders, meshes, text rendering, utilities).
- **engine/** — Core engine services: windowing, scene lifecycle, run-loop, events.
- **Components/** — Reusable component code (e.g. `Transform`).

See the project files (`*.vcxproj`) for build configurations and platform targets.

## Design & Architecture

- **Modular library layers:** audio (`alUtils`), graphics (`glUtils`) and engine core (`engine`) are separated into independently-built static libraries, making the codebase easy to reuse and link into other projects.
- **Scene abstraction (Template Method):** `kbox::Scene` defines a lifecycle (`load`, `process`, `render`, `unload`) that concrete scenes implement. This enforces a clean separation of initialization, per-frame processing and teardown. See [engine/Scene.h](engine/Scene.h).
- **Static manager classes (lightweight singletons):** classes such as `SoundManager` provide convenient, centralized resource access via static methods, for example [alUtils/SoundManager.h](alUtils/SoundManager.h).
- **Context management:** `kbox::Engine` centralizes OpenGL and OpenAL context setup and the primary run loop (window creation, event polling, and coordinated updates). See [engine/Engine.h](engine/Engine.h).
- **Resource encapsulation & inspection:** `glUtils::Shader` wraps shader compilation, program creation and reflection of attributes/uniforms to give a small, safe API around raw GL handles. See [glUtils/Shader.h](glUtils/Shader.h).

## Patterns & Practices

- **RAII-friendly design:** resource lifetime is handled at object boundaries (contexts, buffers, shader programs), reducing risk of leaks and making cleanup deterministic.
- **Separation of concerns:** rendering, audio and engine logic are clearly separated into distinct modules.
- **Use of standard containers and idiomatic C++:** `std::vector`, `std::map`, `std::string` are used throughout for simple, well-tested data handling.
- **Minimal global state:** state is mostly localized to manager classes and engine singletons, avoiding pervasive globals.
- **Explicit error propagation:** the engine exposes error reporting via `kbox::Error` and `kbox::Engine::notify()` to centralize diagnostics.

## Supported features

- Multi-window support via GLFW and a central run loop.
- OpenGL shader management and introspection (`glUtils/Shader`).
- Text rendering, meshes and shader utilities in `glUtils`.
- OpenAL device and buffer helpers plus a centralized sound registry in `alUtils`.
- Simple component and transform utilities in `Components`.
- A small scene lifecycle and window management API in `engine` to run interactive applications.

## Build & Run

This project uses Visual Studio project files.

Notes:
- Windows/Visual Studio is the primary tested platform (see `*.vcxproj`).
- The code expects OpenGL loader (glad), GLFW and OpenAL headers/libraries available to the build environment.

## Extending the library

- Add new engine subsystems by creating a new static library project and exposing a minimal API to `kbox::Engine`.
- To add a new resource type, follow `glUtils::Shader` and `alUtils::Buffer` patterns: encapsulate native handles, provide load/inspect APIs and centralized cleanup.
- For gameplay features, inherit from `kbox::Scene` and implement the lifecycle methods (`load`, `process`, `render`, `unload`).

## Notable files

- [engine/Engine.h](engine/Engine.h) — engine bootstrap, run loop and context management.
- [engine/Scene.h](engine/Scene.h) — scene lifecycle abstraction.
- [glUtils/Shader.h](glUtils/Shader.h) — shader compilation and reflection helper.
- [alUtils/Device.h](alUtils/Device.h) — OpenAL device handling.
- [alUtils/SoundManager.h](alUtils/SoundManager.h) — centralized sound registry.
