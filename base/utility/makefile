EXECUTABLE := main    # 可执行文件名
LIBDIR:=  /usr/local/google  /usr/local/Cellar/boost/1.58.0/lib# 静态库目录
LIBS :=  pthread # 静态库文件名
INCLUDES:=. /usr/local/include/ /usr/local/Cellar/boost/1.58.0/include/boost ../          # 头文件目录
SRCDIR:=              # 除了当前目录外，其他的源代码文件目录
CC:=g++
CFLAGS := -ggdb -Wall -o0 -std=c++11
CPPFLAGS := $(CFLAGS)
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += -MMD -DDEFINE_INT_TYPES
RM-F := rm -f
SRCS := $(wildcard *.cpp) $(wildcard $(addsuffix /*.cpp, $(SRCDIR)))
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
DEPS := $(patsubst %.o, %.d, $(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d, %.cpp,$(MISSING_DEPS)))
.PHONY : all deps objs clean veryclean rebuild info

all: $(EXECUTABLE)

deps : $(DEPS)

objs : $(OBJS)

clean :
	@$(RM-F) *.o *.d *~
veryclean: clean
	@$(RM-F) $(EXECUTABLE)
rebuild: veryclean all
ifneq ($(MISSING_DEPS),)
$(MISSING_DEPS) :
	@$(RM-F) $(patsubst %.d, %.o,$@)
endif
-include $(DEPS)
$(EXECUTABLE) : $(OBJS)
	$(CC) $(CPPFLAGS) -o $(EXECUTABLE) $(OBJS) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS))

info:
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(DEPS)
	@echo $(MISSING_DEPS)
	@echo $(MISSING_DEPS_SOURCES)
