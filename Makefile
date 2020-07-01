APP=TileWorld

ifeq ($(GUI), gtk)
CFLAGS=$(shell pkg-config --cflags gtkmm-3.0) $(shell wx-config --cppflags) -DGTKGUI=1
LDFLAGS=$(shell pkg-config --libs gtkmm-3.0) $(shell wx-config --libs)
else
	ifeq ($(GUI), wx)
		CFLAGS=$(shell wx-config --cppflags) -DWXGUI=1
		LDFLAGS=$(shell wx-config --libs)
	else
		CFLAGS=-DNOGUI=1
	endif
endif

SRC=$(shell find . -name '*.cpp')
OBJS=$(subst .cpp,.o,$(SRC))

all: $(APP)
	
$(APP): $(OBJS)
	clang++ -fPIE -o TileWorld $(OBJS) $(LDFLAGS)

.cpp.o: %.c
	clang++ -g -std=c++17 -fPIE -Werror $(CFLAGS) -c $<

clean: 
	rm -f $(OBJS) TileWorld
