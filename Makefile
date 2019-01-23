APP=TileWorld

CFLAGS=$(shell pkg-config --cflags gtkmm-3.0)
LDFLAGS=$(shell pkg-config --libs gtkmm-3.0)

SRC=$(shell find . -name '*.cpp')
OBJS=$(subst .cpp,.o,$(SRC))

all: $(APP)
	
$(APP): $(OBJS)
	g++ -o TileWorld $(OBJS) $(LDFLAGS)

.cpp.o: %.c
	g++ -g -Wall $(CFLAGS) -c $<

clean: 
	rm -f $(OBJS) TileWorld
