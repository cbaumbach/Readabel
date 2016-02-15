# ==== GENERAL COMPILE AND LINK TIME VARIABLES =======================

CC := g++
CFLAGS += -g -ansi -Wall -Wextra -fpic
CPPFLAGS += -I include

# ==== MACROS ========================================================

SED := sed
TEE := tee
MV := mv -f
RM := rm -f
MKDIR := mkdir

build-shared-library-command = $(CC) -shared -fPIC -o $@ $^ >/dev/null
build-static-library-command = $(AR) $(ARFLAGS) $@ $^ >/dev/null

define make-object-file
@$(MKDIR) -p $(dir $@)
@$(call make-dependency-file,$<,$@,$(subst .o,.d,$@))
$(COMPILE.c) -o $@ $<
endef

# $(call exclude-files, list-of-files-to-exclude, list-of-files)
define exclude-files
$(strip \
  $(foreach f, $2, \
    $(if $(findstring $f, $1),, $f)))
endef

# $(call make-dependency-file, source-file, object-file, dependency-file)
dependency_flag = $(if $(filter gcc g++, $(CC)), -MM, -M)
define make-dependency-file
$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(dependency_flag) $1 | \
$(SED) -e 's,$(notdir $2) *:,$2 $3:,' > $3.tmp && \
$(SED) -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *$$//' -e '/^$$/d' \
       -e 's/\([^\\]\)$$/\1:/' $3.tmp >> $3.tmp && \
$(MV) $3.tmp $3
endef

# $(call subst-dir, files, directory)
subst-dir = $(strip $(foreach f, $1, $2/$(notdir $f)))

# ==== FILES =========================================================

source_directory := src
build_directory := build
test_directory := test
lib_directory := lib

the_library := $(lib_directory)/libreadabel.a
test_library := $(build_directory)/libreadabeltest.so

source_extension := cpp
the_sources := $(wildcard $(source_directory)/*.$(source_extension))
test_sources := $(wildcard $(test_directory)/*.$(source_extension))
sources := $(the_sources) $(test_sources)
the_objects := $(patsubst $(source_directory)/%,$(build_directory)/%,$(subst .$(source_extension),.o,$(the_sources)))
test_objects := $(patsubst $(test_directory)/%,$(build_directory)/%,$(subst .$(source_extension),.o,$(test_sources)))
objects := $(the_objects) $(test_objects)
dependencies := $(subst .o,.d,$(objects))

test_runner := $(build_directory)/runner
test_logfile := $(test_runner).log
test_objects_without_runner_object := $(call exclude-files,$(build_directory)/runner.o,$(test_objects))

# ==== RULES =========================================================

# Empty the list of known suffix rules.  We'll roll our own.
.SUFFIXES:

.PHONY: all test clean

all: test $(the_library)

$(the_library): $(the_objects)
	$(MKDIR) -p $(lib_directory)
	$(build-static-library-command)

$(build_directory)/%.o: $(test_directory)/%.$(source_extension)
	$(call make-object-file)

$(build_directory)/%.o: $(source_directory)/%.$(source_extension)
	$(call make-object-file)

test: $(test_logfile)

$(test_logfile): $(test_runner)
	-$< | $(TEE) $@

$(test_runner): $(build_directory)/runner.o $(test_library) $(the_library)
	$(LINK.o) $^ -o $@

$(test_library): $(test_objects_without_runner_object)
	$(build-shared-library-command)

ifneq "$(MAKECMDGOALS)" "clean"
-include $(dependencies)
endif

clean:
	$(RM) -r $(build_directory) $(lib_directory)
