CFLAGS=-Wall -Wextra
CC?=gcc

GLFW=$(shell pkg-config --cflags --libs glfw3)

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCES=$(call rwildcard,src,*.c) 
OBJECTS=$(addprefix build/,$(notdir $(SOURCES:%.c=%.o)))

TARGET=build/bin/rouge_souls

all: $(TARGET) build/bin/assets

include extern/Makefile

define GEN_RECIPE
$(1) build
	@echo "  CC	$$@"
	@$$(CC) -c $$< $$(CFLAGS) -o $$@ -Isrc/glad/include $$(GLFW) $$(CGLM_CFLAGS)
endef

$(foreach context,$(join $(addsuffix :,$(OBJECTS)),$(SOURCES)),$(eval $(call GEN_RECIPE,$(context))))

$(TARGET): $(OBJECTS) $(CGLM_LIB) build/bin
	@echo "  LD	$@"
	@$(CC) $(OBJECTS) $(CFLAGS) $(GLFW) $(CGLM_LDFLAGS) -o $@

build/bin/assets: build/bin
	@echo "  CP	$@"
	@cp -r assets build/bin/

build/bin build:
	mkdir -p $@
