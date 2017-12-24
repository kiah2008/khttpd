CC := gcc
CXX := g++

LOCAL_PATH := .

LOCAL_MODULE := khttpd
LOCAL_DIRS := ${LOCAL_PATH}/src ${LOCAL_PATH}/include ${LOCAL_PATH}/../include

LOCAL_SRCEXTS = .c .cpp

LOCAL_HEADERS := $(foreach d,$(LOCAL_DIRS), $(wildcard $(addprefix $(d)/*, .h)))

LOCAL_SRCS := $(foreach d,$(LOCAL_DIRS), $(wildcard $(addprefix $(d)/*,$(LOCAL_SRCEXTS))))

LOCAL_OBJS := $(addsuffix .o, $(basename $(LOCAL_SRCS)))  

LOCAL_INCLUDES = -I./include \
	-I../include \

LD_FLAGS := ${LDFLAGS}-lpthread

CCFLAGS := -g -Wall -O0
CXXFLAGS := -g -Wall -O0

.PHONY: clean all install

all:$(LOCAL_MODULE)

$(LOCAL_MODULE) : $(LOCAL_OBJS) $(LOCAL_HEADERS)
	$(CC) $^ -o $@ $(LOCAL_INCLUDES) $(LD_FLAGS)

%.o : %.c
	$(CC) -c $< $(CCFLAGS) ${LOCAL_INCLUDES} -o $@
	
%.o : %.cpp
	$(CXX) -c $< $(CXXFLAGS) ${LOCAL_INCLUDES} -o $@

install:
	cp ${LOCAL_MODULE} ${OUT_DIR}/
	cp -R htdocs ${OUT_DIR}/

clean:
	rm -rf $(LOCAL_MODULE) $(LOCAL_OBJS)