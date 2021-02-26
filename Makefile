APP=TileWorld
#COMPILER=clang++
COMPILER=g++
CFLAGS=-O0 -Werror -std=c++17 -g -DDEBUGGING=1
LDFLAGS=-std=c++17 -g

ifeq ($(GUI), gtk)
	CFLAGS+=$(shell pkg-config --cflags gtkmm-3.0) -DGTKGUI=1
	LDFLAGS+=$(shell pkg-config --libs gtkmm-3.0) 
else
	ifeq ($(GUI), wx)
		CFLAGS+=$(shell wx-config --static=yes --cppflags) -DWXGUI=1
		LDFLAGS+=$(shell wx-config --static=yes --libs)
	else
		CFLAGS+=-DNOGUI=1
	endif
endif

SRC=$(shell find . -name '*.cpp')
OBJS=$(subst .cpp,.o,$(SRC))
HDRS=$(shell find . -name '*.hpp')

all: $(APP)
	
$(APP): $(OBJS)
	$(COMPILER) $(LDFLAGS) -o TileWorld $(OBJS)

.cpp.o: %.c $(HDRS)
	$(COMPILER) $(CFLAGS) -c $<

clean: 
	rm -f $(OBJS) TileWorld
