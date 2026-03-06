CC = gcc
TARGET_EXEC := main

BUILD_DIR := ./build
SOURCE := ./src/main.c
INC_DIRS := ./include

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Include files
INC_FILES := $(foreach D, $(INC_DIRS), $(wildcard $(D)/*.h))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(SOURCE) $(INC_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(SOURCE) -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
