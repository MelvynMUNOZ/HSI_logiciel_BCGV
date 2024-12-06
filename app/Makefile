#==============================================================================

# Define executable name
EXECUTABLE_NAME := app

# Define build mode (debug or release)
BUILD_MODE := debug

# Define source files to compile
SOURCES := \
	app.c


#==============================================================================
# DIRECTORIES AND FILES
#==============================================================================

### Predefined directories
DIR_BIN   := bin/
DIR_BUILD := build/
DIR_SRC   := src/

### Target
TARGET := $(DIR_BIN)$(EXECUTABLE_NAME)

### Source files
SOURCE_FILES := $(filter-out \,$(addprefix $(DIR_SRC),$(SOURCES)))

### Object files
OBJECT_FILES := $(subst $(DIR_SRC),$(DIR_BUILD),$(addsuffix .o,$(basename $(SOURCE_FILES))))


#==============================================================================
# COMPILER AND LINKER
#==============================================================================

### C Compiler
CC := gcc

### C standard
CSTD := -std=c99

### Extra flags to give to the C compiler
CFLAGS := $(CSTD) -W -Wall -pedantic

### Extra flags to give to the C preprocessor (e.g. -I, -D, -U ...)
CPPFLAGS := -I../driver/include

### Extra flags to give to compiler when it invokes the linker (e.g. -L ...)
LDFLAGS := -L../driver/lib

### Library names given to compiler when it invokes the linker (e.g. -l ...)
LDLIBS := -l:drv_api.a

### Build mode specific flags
DEBUG_FLAGS   := -O0 -g3
RELEASE_FLAGS := -O2 -g0


#==============================================================================
# SHELL
#==============================================================================

### Commands
MKDIR := mkdir -p
RM    := rm -rf
CP    := cp -rf
MV    := mv


#==============================================================================
# RULES
#==============================================================================

default: build

### Verbosity
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
	@if [ ! -d "$(DIR_BIN)" ]; then \
		$(MKDIR) $(DIR_BIN); \
	fi
	@if [ ! -d "$(DIR_BUILD)" ]; then \
		$(MKDIR) $(DIR_BUILD); \
	fi


#-------------------------------------------------
# (Internal rule) Pre build operations
#-------------------------------------------------
.PHONY: __prebuild
__prebuild: __checkdirs
ifeq ($(EXECUTABLE_NAME),)
	$(error EXECUTABLE_NAME is required. Must provide an executable name)
endif
ifeq ($(filter $(BUILD_MODE),debug release),)
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
# Link object files into target target
#-------------------------------------------------
$(TARGET): $(OBJECT_FILES)
	@echo '-- Linking target $@'
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)


#-------------------------------------------------
# Compile C source files
#-------------------------------------------------
$(DIR_BUILD)%.o: $(DIR_SRC)%.c
	@echo '-- Compiling $<'
	@$(MKDIR) $(dir $@)
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ 


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
	@echo 'Clean done'


#-------------------------------------------------
# Project informations
#-------------------------------------------------
.PHONY: info
info:
	@echo 'Build configurations'
	@echo '-- CC: $(CC)'
	@echo '-- CFLAGS: $(CFLAGS)'
	@echo '-- CPPFLAGS: $(CPPFLAGS)'
	@echo '-- LDFLAGS: $(LDFLAGS)'
	@echo '-- LDLIBS: $(LDLIBS)'
	@echo 'Files'
	@echo '-- TARGET: $(TARGET)'
	@echo '-- SOURCE_FILES: $(SOURCE_FILES)'
	@echo '-- OBJECT_FILES: $(OBJECT_FILES)'