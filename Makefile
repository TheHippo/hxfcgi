HXCPP_INCLUDE = $(shell sh -c 'haxelib path hxcpp')include/

CC = g++
CFLAGS = -Wall -shared -I$(HXCPP_INCLUDE)
LDFLAGS = -lfcgi
OUT = bin/hxfcgi.ndll
OBJ = src/hxfcgi.o src/request.o src/basic.o

hxfcgi: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT) $(LDFLAGS)
	
src/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm $(OUT)
	rm $(OBJ)

all: clean hxfcgi neko

neko:
	haxe neko.hxml
	nekotools boot bin/test.n
	mv bin/test bin/test.fcgi
	
cpp:
	haxe cpp.hxml
	mv bin/Test bin/test.fcgi
