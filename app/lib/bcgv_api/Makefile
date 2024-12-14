#==============================================================================

# Define executable name
EXECUTABLE_NAME := bcgv_api

# Define build mode (debug or release)
BUILD_MODE := debug

# Define source files to compile
SOURCES := bcgv_api.c

#==============================================================================
# DIRECTORIES AND FILES
#==============================================================================

### Predefined directories
DIR_BUILD := build/
DIR_SRC := src/
DIR_LIB := lib/

### Target
TARGET := $(DIR_LIB)$(EXECUTABLE_NAME).a

### Source files
SOURCE_FILES := $(strip $(filter-out \, $(addprefix $(DIR_SRC), $(SOURCES))))

### Object file 
OBJECT_FILES := $(strip $(subst $(DIR_SRC), $(DIR_BUILD), $(addsuffix .o, $(basename $(SOURCE_FILES)))))

#==============================================================================
# COMPILER AND LINKER
#==============================================================================

### C Compiler
CC := gcc

### C standard
CSTD := -std=c99

### Extra flags to give to the C compiler
CFLAGS := $(CSTD) -W -Wall -pedantic

### Build mode specific flags
DEBUG_FLAGS   := -O0 -g3
RELEASE_FLAGS := -O2 -g0

### Library static
AR := ar

### Library compiler function
RCS := rcs

#==============================================================================
# SHELL
#==============================================================================

### Commands
MKDIR := mkdir -p
RM    := rm -rf

#==============================================================================
# RULES
#==============================================================================

default: build

###Verbosity
VERBOSE := $(or $(v), $(verbose))
ifeq ($(VERBOSE),)
	Q := @
else
	Q := 
endif

#-------------------------------------------------
# (Internal rule) Check directories
#-------------------------------------------------
.PHONY: __checkdirs
__checkdirs:
	@if [ ! -d "$(DIR_BUILD)" ]; then \
		$(MKDIR) $(DIR_BUILD); \
	fi
	@if [ ! -d "$(DIR_LIB)" ]; then \
		$(MKDIR) $(DIR_LIB); \
	fi

#-------------------------------------------------
# (Internal rule) Pre build operations
#-------------------------------------------------
.PHONY: __prebuild
__prebuild: __checkdirs
ifeq ($(EXECUTABLE_NAME),)
	$(error EXECUTABLE_NAME is required. Must provide an executable name)
endif
ifeq ($(filter $(BUILD_MODE), debug release),)
	$(error BUILD_MODE is invalid. Must provide a valid mode (debug or release))
endif
ifeq ($(SOURCES),)
	$(error SOURCES is required. Must provide sources files to compile)
endif

ifeq ($(BUILD_MODE),debug)
	$(eval CFLAGS += $(DEBUG_FLAGS))
else ifeq ($(BUILD_MODE),release)
	$(eval CFLAGS += $(RELEASE_FLAGS))
endif

	@echo 'Build $(TARGET) ($(BUILD_MODE))'


#-------------------------------------------------
# Build operations
#-------------------------------------------------
.PHONY: build
build: __prebuild $(TARGET)
	@echo 'Build done'


#-------------------------------------------------
# Rebuild operations
#-------------------------------------------------
.PHONY: rebuild
rebuild: clean build


#-------------------------------------------------
# Compile C source files
#-------------------------------------------------
$(DIR_BUILD)%.o: $(DIR_SRC)%.c
	@echo '-- Compiling $<'
	@$(MKDIR) $(dir $@)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@


#----------------------------------------------------
# Compile O source files to build the static library
#----------------------------------------------------
$(TARGET): $(OBJECT_FILES)
	@echo '-- Creating static library $@'
	$(Q)$(AR) $(RCS) $@ $^


#-------------------------------------------------
# Clean generated files
#-------------------------------------------------
.PHONY: clean
clean:
	@echo 'Clean generated files'
	@echo '-- Deleting target $(TARGET)'
	@if [ -n "$(TARGET)" ]; then \
		$(RM) $(TARGET); \
	else \
		echo "Skipping: TARGET is not set"; \
	fi
	@echo '-- Deleting objects $(OBJECT_FILES)'
	@if [ -n "$(OBJECT_FILES)" ]; then \
		$(RM) $(OBJECT_FILES); \
	else \
		echo "Skipping: OBJECT_FILES is not set"; \
	fi
	@if [ -n "$(DIR_BUILD)" ]; then \
		$(RM) $(DIR_BUILD); \
	else \
		echo "Skipping: DIR_BUILD is not set"; \
	fi
	@if [ -n "$(DIR_LIB)" ]; then \
		$(RM) $(DIR_LIB); \
	else \
		echo "Skipping: DIR_LIB is not set"; \
	fi
	@echo 'Clean done'


#-------------------------------------------------
# Project informations
#-------------------------------------------------
.PHONY: info
info:
	@echo 'Build configurations'
	@echo '-- CC: $(CC)'
	@echo '-- CFLAGS: $(CFLAGS)'
	@echo 'Files'
	@echo '-- TARGET: $(TARGET)'
	@echo "SOURCES: $(SOURCES)"
	@echo '-- SOURCE_FILES: $(SOURCE_FILES)'
	@echo '-- OBJECT_FILES: $(OBJECT_FILES)'
