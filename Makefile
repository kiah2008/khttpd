BUILDDIRS := \
    external/mxml-2.11 \
	khttpd/ \
	remoteproxy/ \
	simpleclient/

#	external/libcups-2.2.6/ \

OUT_DIR	:=	out
export LDFLAGS:= -Lout

export OPTIM =	-Wall -Wno-format-y2k -Wunused -fPIC -Os -g -fstack-protector -Wno-unused-result -Wsign-conversion -Wno-tautological-compare -Wno-format-truncation

$(info dirs including ${BUILDDIRS})

.PHONY: all clean install

all:
	@mkdir -p ${OUT_DIR}
	@for d in ${BUILDDIRS} ; do \
	make -C $$d $@ && OUT_DIR=$${PWD}/${OUT_DIR} make -C $$d install ||exit 1; \
	done;
clean:
	@-rm -rf ${OUT_DIR}
	@for d in ${BUILDDIRS} ; do \
	make -C $$d $@  ||exit 1; \
	done;
install: all
	@mkdir -p ${OUT_DIR}
	@for d in ${BUILDDIRS} ; do \
	OUT_DIR=${OUT_DIR} make -C $$d $@  ||exit 1; \
	done;
