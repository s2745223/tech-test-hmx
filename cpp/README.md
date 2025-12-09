# C++ Tech Test

This is a C++ version of the tech test, translated from the original C# version.

## Prerequisites

- CMake 3.10 or later
- C++17/20/23 compatible compiler (GCC, Clang, or MSVC)

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Running

### Console Application
```bash
cd build
./ConsoleApp
```

### Tests
```bash
cd build
./Tests
```

## Project Structure

- `Models/` - Core data models and interfaces
- `Loaders/` - Trade data loaders
- `Pricers/` - Pricing engines
- `RiskSystem/` - Risk system components
- `ConsoleApp/` - Main application
- `Tests/` - Unit tests

## Documentation

| Document | Description |
|----------|-------------|
| [candidate-instructions.md](./candidate-instructions.md) | **For candidates** - Exercise instructions and rules |

## Key Differences from C# Version

1. Uses C++17 standard library features
2. `std::optional` for nullable types
3. `std::chrono` for date/time handling
4. Simple test framework instead of NUnit
5. CMake build system instead of .NET project files
6. Factory pattern instead of reflection (Exercise 4)
7. Additional C++ specific challenges (memory management, pass-by-reference)
