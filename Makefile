#==============================================================================
# DIRECTORIES
#==============================================================================

### Sub-Makefiles directories
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
# Makefile's clean rules
#-------------------------------------------------
.PHONY: clean
clean:
	@echo 'Clean application and static library'
	$(Q)$(MAKE) -C $(DIR_APP) clean
	$(Q)$(MAKE) -C $(DIR_LIB) clean

.PHONY: cleanlib
cleanlib:
	@echo 'Clean static library'
	$(Q)$(MAKE) -C $(DIR_LIB) clean

.PHONY: cleanapp
cleanapp:
	@echo 'Clean application'
	$(Q)$(MAKE) -C $(DIR_APP) clean

#-------------------------------------------------
# App Makefile
#-------------------------------------------------
.PHONY: app
app:
	$(Q)$(MAKE) -C $(DIR_APP)

#-------------------------------------------------
# Lib Makefile
#-------------------------------------------------
.PHONY: lib
lib:
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
	@echo 'Directories'
	@echo '-- APP: $(DIR_APP)'
	@echo '-- LIB: $(DIR_LIB)'
