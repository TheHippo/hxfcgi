HXCPP_INCLUDE = $(shell sh -c 'haxelib path hxcpp | head -1')include/

CC = g++
CFLAGS = -O2 -s -Wall -shared -I$(HXCPP_INCLUDE) -D hxcpp
ifdef HXCPP_M64
CFLAGS += -D HXCPP_M64 -fPIC
endif
LDFLAGS = -lfcgi
OUT = bin/hxfcgi.ndll
OBJ = src/hxfcgi.o src/request.o src/basic.o src/data.o

all: clean hxfcgi neko cpp

hxfcgi: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT) $(LDFLAGS)
	
src/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -f $(OUT)
	rm -f $(OBJ)


neko:
ifdef HXCPP_M64
	haxe -D HXCPP_M64 neko.hxml
else
	haxe neko.hxml
endif
	nekotools boot bin/test.n
	mv bin/test bin/test_neko.fcgi
	
cpp:
ifdef HXCPP_M64
	haxe -D HXCPP_M64 cpp.hxml
else
	haxe cpp.hxml
endif
	mv bin/Test bin/test_cpp.fcgi
