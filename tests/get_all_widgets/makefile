include prorab.mk


this_name := tests


this_srcs += $(call prorab-src-dir,.)


this_cxxflags += -Wall
this_cxxflags += -Wno-comment #no warnings on nested comments
this_cxxflags += -Wno-format #no warnings about format
this_cxxflags += -Wno-format-security #no warnings about format
this_cxxflags += -DDEBUG
this_cxxflags += -fstrict-aliasing #strict aliasing!!!
this_cxxflags += -g
this_cxxflags += -std=c++11



ifeq ($(debug), true)
    this_cxxflags += -DDEBUG
else
    this_cxxflags += -O3
endif

this_cxxflags += -I$(d)../../src
this_objcflags += -I$(d)../../src

ifeq ($(ogles2), true)
    this_cxxflags += -DM_RENDER_OPENGLES2

    ifeq ($(raspberrypi),true)
        this_cxxflags += -I/opt/vc/include
        this_ldflags += -L/opt/vc/lib
    endif
endif


ifeq ($(os),windows)
    this_ldlibs += -lmingw32 #these should go first, otherwise linker will complain about undefined reference to WinMain
    this_ldlibs +=  -lglew32 -lopengl32 -lpng -ljpeg -lz -lfreetype -mwindows
else ifeq ($(os),macosx)
    this_ldlibs += -lGLEW -framework OpenGL -framework Cocoa -lpng -ljpeg -lfreetype

    this_ldflags += -rdynamic

    this_mm_obj := $(d)$(prorab_obj_dir)objcpp/src/mordavokne/glue/macosx/glue.o

    define this_rules
        $(this_mm_obj): $(d)src/mordavokne/glue/macosx/glue.mm
		@echo Compiling $$<...
		$(prorab_echo)mkdir -p $$(dir $$@)
		$(prorab_echo)$(CC) -ObjC++ -std=c++11 -c -o "$$@" $(this_objcflags) $$<
    endef
    $(eval $(this_rules))
else ifeq ($(os),linux)
    this_cxxflags += -fPIC
    this_ldlibs += -pthread -lX11 -ldl
    this_ldflags += -rdynamic
endif

this_ldlibs += $(d)../../src/build/libmorda$(soext)


this_ldlibs += -lnitki -lopros -lpuu -lpapki -lstdc++ -lm

this_no_install := true

this_out_dir := build

$(eval $(prorab-build-app))

this_dirs := $(subst /, ,$(d))
this_test := $(word $(words $(this_dirs)),$(this_dirs))

define this_rules
test:: $(prorab_this_name)
	@myci-running-test.sh $(this_test)
	@(cd $(d); LD_LIBRARY_PATH=../../src/build $$^)
	@myci-passed.sh
endef
$(eval $(this_rules))


#add dependency on libmorda
ifeq ($(os),windows)
    $(d)libmorda$(soext): $(abspath $(d)../../src/build/libmorda$(soext))
	@cp $< $@

    $(prorab_this_name): $(d)libmorda$(soext)

    define this_rules
        clean::
		@rm -f $(d)libmorda$(soext)
    endef
    $(eval $(this_rules))
else
    $(prorab_this_name): $(abspath $(d)../../src/build/libmorda$(soext))
endif



$(eval $(call prorab-try-include, ../../src/makefile))

