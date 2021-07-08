APP=TileWorld
#COMPILER=clang++
STD=-std=c++17
COMPILER=g++
CFLAGS=-O0 -Werror $(STD) -g -DDEBUGGING=1 -DDEBUG=1
LDFLAGS=$(STD) -g
#GUI=gtk

ifeq ($(GUI), gtk)
	CFLAGS+=$(shell pkg-config --cflags gtkmm-3.0) -DGTKGUI=1
	LDFLAGS+=$(shell pkg-config --libs gtkmm-3.0) 
else
	ifeq ($(GUI), wx)
		CFLAGS+=$(shell wx-config --cppflags) -DWXGUI=1
		LDFLAGS+=$(shell wx-config --libs)
	else
		CFLAGS+=-DNOGUI=1
	endif
endif

SRC=$(shell find . -name '*.cpp' | grep -v _test)
OBJS=$(subst .cpp,.o,$(SRC))
HDRS=$(shell find . -name '*.hpp')

all: $(APP)
	
$(APP): $(OBJS)
	$(COMPILER) $(LDFLAGS) -o TileWorld $(OBJS)

.cpp.o: %.c $(HDRS)
	$(COMPILER) $(CFLAGS) -c $<

clean: 
	rm -f $(OBJS) *.o TileWorld
