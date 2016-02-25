# ==== GENERAL COMPILE AND LINK TIME VARIABLES =======================

CXX := g++
CXXFLAGS += -g -Wall -Wextra -fPIC
CPPFLAGS += -I include

# ==== MACROS ========================================================

SED := sed
TEE := tee
MV := mv -f
RM := rm -f
MKDIR := mkdir

define make-object-file
@$(MKDIR) -p $(dir $@)
@$(call make-dependency-file,$<,$@,$(subst .o,.d,$@))
$(COMPILE.cc) -ansi -o $@ $<
endef

# $(call make-dependency-file, source-file, object-file, dependency-file)
dependency_flag = $(if $(filter gcc g++, $(CXX)), -MM, -M)
define make-dependency-file
$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(dependency_flag) $1 | \
$(SED) -e 's,$(notdir $2) *:,$2 $3:,' > $3.tmp && \
$(SED) -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *$$//' -e '/^$$/d' \
       -e 's/\([^\\]\)$$/\1:/' $3.tmp >> $3.tmp && \
$(MV) $3.tmp $3
endef

# ==== FILES =========================================================

source_directory := src
build_directory := build
test_directory := test
lib_directory := lib

library := $(lib_directory)/libreadabel.a
sources := $(wildcard $(source_directory)/*.cpp)
test_sources := $(wildcard $(test_directory)/*.cpp)
objects := $(patsubst $(source_directory)/%,$(build_directory)/%,$(subst .cpp,.o,$(sources)))
test_objects := $(patsubst $(test_directory)/%,$(build_directory)/%,$(subst .cpp,.o,$(test_sources)))
dependencies := $(subst .o,.d,$(objects) $(test_objects))
test_runner := $(build_directory)/runner
test_logfile := $(test_runner).log

# ==== RULES =========================================================

# Empty the list of known suffix rules.  We'll roll our own.
.SUFFIXES:

.PHONY: all test clean

all: test $(library)

$(library): $(objects)
	$(MKDIR) -p $(lib_directory)
	$(AR) $(ARFLAGS) $@ $^ >/dev/null

$(build_directory)/%.o: $(test_directory)/%.cpp
	$(call make-object-file)

$(build_directory)/%.o: $(source_directory)/%.cpp
	$(call make-object-file)

test: $(test_logfile)

$(test_logfile): $(test_runner)
	-$< | $(TEE) $@

$(test_runner): $(test_objects) $(library)
	$(LINK.cc) -o $@ $^

ifneq "$(MAKECMDGOALS)" "clean"
-include $(dependencies)
endif

clean:
	$(RM) -r $(build_directory) $(lib_directory)
