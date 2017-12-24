CC := gcc
CXX := g++

LOCAL_PATH := .

LOCAL_MODULE := simpleclient
LOCAL_DIRS := ${LOCAL_PATH}/src

LOCAL_SRCEXTS = .c .C .cc .cpp .CPP .c++ .cxx .cp

LOCAL_SRCS := $(foreach d,$(LOCAL_DIRS), $(wildcard $(addprefix $(d)/*,$(LOCAL_SRCEXTS))))

LOCAL_OBJS := $(addsuffix .o, $(basename $(LOCAL_SRCS)))  

LOCAL_INCLUDES = -I./include \

LD_FLAGS := -lpthread

CCFLAGS := -g -Wall -O0
CXXFLAGS := -g -Wall -O0

.PHONY: clean all

all:$(LOCAL_MODULE)

$(LOCAL_MODULE) : $(LOCAL_OBJS)
	$(CC) $^ -o $@ $(LOCAL_INCLUDES) $(LD_FLAGS)

%.o : %.c
	$(CC) -c $< $(CCFLAGS) ${LOCAL_INCLUDES} -o $@
	
%.o : %.cpp
	$(CXX) -c $< $(CXXFLAGS) ${LOCAL_INCLUDES} -o $@

install:
	cp ${LOCAL_MODULE} ${OUT_DIR}/

clean:
	rm -rf $(LOCAL_MODULE) $(LOCAL_OBJS)