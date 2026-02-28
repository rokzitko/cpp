# Makefile — static-link Lua into a C++ program (macOS/Linux)

# Toolchain
CC   := cc
CXX  := c++
AR   := ar
RANLIB := ranlib

# Build flags
CFLAGS   := -O2 -Wall -Wextra -fPIC
CXXFLAGS := -O2 -std=c++17 -Wall -Wextra

# Lua source directory
LUA_DIR := third_party/lua

# Lua core sources (PUC-Lua 5.4.x). Excludes lua.c and luac.c (executables).
LUA_CORE_SRC := \
  $(LUA_DIR)/lapi.c $(LUA_DIR)/lcode.c $(LUA_DIR)/lctype.c $(LUA_DIR)/ldebug.c \
  $(LUA_DIR)/ldo.c $(LUA_DIR)/ldump.c $(LUA_DIR)/lfunc.c $(LUA_DIR)/lgc.c \
  $(LUA_DIR)/llex.c $(LUA_DIR)/lmem.c $(LUA_DIR)/lobject.c $(LUA_DIR)/lopcodes.c \
  $(LUA_DIR)/lparser.c $(LUA_DIR)/lstate.c $(LUA_DIR)/lstring.c $(LUA_DIR)/ltable.c \
  $(LUA_DIR)/ltm.c $(LUA_DIR)/lundump.c $(LUA_DIR)/lvm.c $(LUA_DIR)/lzio.c \
  $(LUA_DIR)/lauxlib.c $(LUA_DIR)/lbaselib.c $(LUA_DIR)/lcorolib.c $(LUA_DIR)/ldblib.c \
  $(LUA_DIR)/liolib.c $(LUA_DIR)/lmathlib.c $(LUA_DIR)/loslib.c $(LUA_DIR)/lstrlib.c \
  $(LUA_DIR)/ltablib.c $(LUA_DIR)/lutf8lib.c $(LUA_DIR)/loadlib.c $(LUA_DIR)/linit.c

LUA_CORE_OBJ := $(LUA_CORE_SRC:.c=.o)
LUA_LIB      := $(LUA_DIR)/liblua.a

# Your program
TARGET := test1
APP_SRC := main.cpp
APP_OBJ := $(APP_SRC:.cpp=.o)

# Include path for lua headers
CPPFLAGS := -I$(LUA_DIR)

# System libs (platform dependent)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  SYSLIBS := -lm -ldl
else ifeq ($(UNAME_S),Darwin)
  # macOS: -lm usually not needed; -ldl not used.
  SYSLIBS :=
endif

.PHONY: all clean

all: $(TARGET)

# Build static Lua library
$(LUA_LIB): $(LUA_CORE_OBJ)
	$(AR) rcs $@ $^
	$(RANLIB) $@

$(LUA_DIR)/%.o: $(LUA_DIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Build your app and link with static liblua.a
$(TARGET): $(APP_OBJ) $(LUA_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $(APP_OBJ) $(LUA_LIB) $(SYSLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(APP_OBJ) $(LUA_CORE_OBJ) $(LUA_LIB)
