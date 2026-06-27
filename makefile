# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
CC := gcc
OPT := -O2
TARGET_EXEC := sh

BUILD_DIR_DEBUG := ./build/debug
BUILD_DIR_RELEASE := ./build/release
SRC_DIRS := ./src
INC_DIRS := ./include

# Default release build flags.
CFLAGS_RELEASE := -Wall -Wextra -pedantic -Wstrict-aliasing $(OPT)
LDFLAGS_RELEASE :=

# Debug build.
CFLAGS_DEBUG := -Wall -Wextra -pedantic -g -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS_DEBUG := -fsanitize=address,undefined

# Default to release build.
MODE ?= release

ifeq ($(MODE),debug)
    CFLAGS := $(CFLAGS_DEBUG)
    LDFLAGS := $(LDFLAGS_DEBUG)
	BUILD_DIR := $(BUILD_DIR_DEBUG)
else
    CFLAGS := $(CFLAGS_RELEASE)
    LDFLAGS := $(LDFLAGS_RELEASE)
	BUILD_DIR := $(BUILD_DIR_RELEASE)
endif

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.s')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# Final build.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean debug release

clean:
	rm -rf $(BUILD_DIR_DEBUG) $(BUILD_DIR_RELEASE)
debug:
	$(MAKE) MODE=debug
release:
	$(MAKE) MODE=release

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
