# =============================================================================
# Dragonfly Game Engine - Makefile (macOS + Linux)
# Requires SFML installed via Homebrew: brew install sfml
# =============================================================================

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# ---- SFML detection ----
# Homebrew on Apple Silicon (M1/M2)
SFML_ARM = /opt/homebrew/opt/sfml
# Homebrew on Intel Mac
SFML_X86 = /usr/local/opt/sfml
# Linux pkg-config fallback
SFML_LINUX = /usr

ifeq ($(shell test -d $(SFML_ARM) && echo yes), yes)
    SFML_DIR = $(SFML_ARM)
else ifeq ($(shell test -d $(SFML_X86) && echo yes), yes)
    SFML_DIR = $(SFML_X86)
else
    SFML_DIR = $(SFML_LINUX)
endif

INCLUDES = -I$(SFML_DIR)/include
LDFLAGS  = -L$(SFML_DIR)/lib \
           -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# ---- Source files ----
SRCS = \
    Vector.cpp \
    Clock.cpp \
    Event.cpp \
    EventStep.cpp \
    EventOut.cpp \
    EventCollision.cpp \
    EventKeyboard.cpp \
    EventMouse.cpp \
    Manager.cpp \
    LogManager.cpp \
    Object.cpp \
    ObjectList.cpp \
    WorldManager.cpp \
    DisplayManager.cpp \
    InputManager.cpp \
    GameManager.cpp

TEST_SRC = main_test.cpp

OBJS     = $(SRCS:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

TARGET   = dragonfly_test

# ---- Targets ----
all: $(TARGET)

$(TARGET): $(OBJS) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TEST_OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
