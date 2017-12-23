SUB_DIRS := khttpd/ \
	remoteproxy/ \
	simpleclient/
OUT_DIR := out

$(info dirs including ${SUB_DIRS})
.PHONY: all clean install

all:
	@for d in ${SUB_DIRS} ; do \
	make -C $$d $@; done;
clean:
	rm -rf ${OUT_DIR}
	@for d in ${SUB_DIRS} ; do \
	make -C $$d $@; done;
install: all
	mkdir -p ${OUT_DIR}
	@for d in ${SUB_DIRS} ; do \
	OUT_DIR=${OUT_DIR} make -C $$d $@; done;