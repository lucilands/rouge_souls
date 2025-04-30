CGLM_CFLAGS=-Iextern/cglm/include
CGLM_LIB=extern/cglm/libcglm.la
CGLM_LDFLAGS=-Lextern/cglm -lm -lcglm

.PHONY: cglm

$(CGLM_LIB):
	cd extern/cglm/ && sh autogen.sh
	cd extern/cglm/ && ./configure
	cd extern/cglm/ && make
