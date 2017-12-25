CC := gcc
CXX := g++

LOCAL_PATH := .

LOCAL_MODULE := libmxml.so.1.6
LOCAL_DIRS := ${LOCAL_PATH}/

LOCAL_SRCEXTS = .c .C .cc .cpp .CPP .c++ .cxx .cp

LOCAL_SRCS := $(foreach d,$(LOCAL_DIRS), $(wildcard $(addprefix $(d)/*,$(LOCAL_SRCEXTS))))

LOCAL_OBJS := $(addsuffix .o, $(basename $(LOCAL_SRCS)))  

LOCAL_INCLUDES = -I./ \

LD_FLAGS := -lpthread -lz

CCFLAGS := -g -O0 -Wall -D_GNU_SOURCE   -D_THREAD_SAFE -D_REENTRANT
CXXFLAGS := -g -Wall -O0
DSOFLAGS	=	-shared -fPIC -Os -g

.PHONY: clean all

all:$(LOCAL_MODULE)

$(LOCAL_MODULE) : $(LOCAL_OBJS)
	$(CC) $(DSOFLAGS) $(CCFLAGS) $^ -o $@ $(LOCAL_INCLUDES) $(LD_FLAGS)

%.o : %.c
	$(CC) $(DSOFLAGS) -c $< $(CCFLAGS) ${LOCAL_INCLUDES} -o $@
	
%.o : %.cpp
	$(CXX) $(DSOFLAGS) -c $< $(CXXFLAGS) ${LOCAL_INCLUDES} -o $@

install:
	cp ${LOCAL_MODULE} ${OUT_DIR}/

clean:
	rm -rf $(LOCAL_MODULE) $(LOCAL_OBJS)
