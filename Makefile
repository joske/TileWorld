APP=TileWorld

CFLAGS=$(shell pkg-config --cflags gtkmm-3.0) $(shell wx-config --cppflags)
LDFLAGS=$(shell pkg-config --libs gtkmm-3.0) $(shell wx-config --libs)

SRC=$(shell find . -name '*.cpp')
OBJS=$(subst .cpp,.o,$(SRC))

all: $(APP)
	
$(APP): $(OBJS)
	g++ -o TileWorld $(OBJS) $(LDFLAGS)

.cpp.o: %.c
	clang++ -g -std=c++17 -Wall -DNOGUI=1 -DWXGUI=1 $(CFLAGS) -c $<

clean: 
	rm -f $(OBJS) TileWorld
