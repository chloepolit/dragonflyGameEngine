# Dragonfly Game Engine
## Setup, Build & Testing Guide (macOS)
### Chloe Polit

---

## Prerequisites

### 1. Install Xcode Command Line Tools
```bash
xcode-select --install
```

### 2. Install Homebrew (if not already installed)
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 3. Install SFML via Homebrew
```bash
brew install sfml
```
Works on both Apple Silicon (M1/M2/M3) and Intel Macs.

---

## Project Setup

Place all `.cpp`, `.h`, `Makefile`, and `df-font.ttf` files in the same directory.
The font **must** be in the same folder as the executable at runtime.

---

## Build & Run

```bash
cd df_fixed
make
make run
```

Expected output:
```
======================================
 Dragonfly Engine - Test Suite
======================================
All managers started OK.

--- Vector Tests ---
  PASS: Default constructor x=0
  ...

======================================
 Results: 47 passed, 0 failed
======================================
(See dragonfly.log for detailed engine log)
```

## Files

```
df-font.ttf
Makefile
Vector.h / .cpp          Color.h
Clock.h / .cpp           Event.h / .cpp
EventStep.h / .cpp       EventOut.h / .cpp
EventCollision.h / .cpp  EventKeyboard.h / .cpp
EventMouse.h / .cpp      Manager.h / .cpp
LogManager.h / .cpp      Object.h / .cpp
ObjectList.h / .cpp      WorldManager.h / .cpp
DisplayManager.h / .cpp  InputManager.h / .cpp
GameManager.h / .cpp     main_test.cpp
README.md
```
