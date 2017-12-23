SUB_DIRS:= khttpd/
.PHONY: all clean

all:
	@$(foreach d, ${SUB_DIRS}, make -C $d)

clean:
	@$(foreach d, ${SUB_DIRS}, make -C $d clean)