#==============================================================================

#Define file name
FILE_NAME := Makefile

#==============================================================================
# DIRECTORIES AND FILES
#==============================================================================

### Predefined directories
DIR_APP := app/
DIR_LIB := app/lib/bcgv_api/

#==============================================================================

default : all

###Verbosity
VERBOSE := $(or $(v), $(verbose))
ifeq ($(VERBOSE),)
	Q := @
else
	Q := 
endif

#-------------------------------------------------
# Makefile's clean rule
#-------------------------------------------------
.PHONY: clean
clean:
	@echo 'Clean generated files and static library'
	$(Q)$(MAKE) -C $(DIR_APP) clean
	$(Q)$(MAKE) -C $(DIR_LIB) clean

#-------------------------------------------------
# App Makefile
#-------------------------------------------------
.PHONY: app
app:
	@echo 'App Makefile'
	$(Q)$(MAKE) -C $(DIR_APP)

#-------------------------------------------------
# Lib Makefile
#-------------------------------------------------
.PHONY: lib
lib:
	@echo 'Static Lib Makefile'
	$(Q)$(MAKE) -C $(DIR_LIB)

#-------------------------------------------------
# App and Lib Makefile
#-------------------------------------------------

.PHONY: all
all: lib app

#-------------------------------------------------
# Project informations
#-------------------------------------------------
.PHONY: info
info:
	@echo 'Files'
	@echo '-- FILE_NAME: $(FILE_NAME)'
	@echo 'SOURCES: $(SOURCES)'
	@echo '-- APP_FILE: $(APP_FILE)'
	@echo '-- LIB_FILE: $(LIB_FILE)'
