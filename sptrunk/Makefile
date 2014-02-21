CC=g++

all: UtilityCommon.o  Singleton.o nmath.o nnode.o nlist.o Narray.o
	
UtilityCommon.o:UtilityCommon.h
	$(CC) -o $@ -c $^

Singleton.o:Singleton.h
	$(CC) -o $@ -c $^ 

nmath.o:nmath.h
	$(CC) -o $@ -c $^
nnode.o:nnode.h
	$(CC) -o $@ -c $^
nlist.o:nnode.h
	$(CC) -o $@ -c $^

Narray.o:Narray.h
	$(CC) -o $@ -c $^


clean:
	@echo "清空工程build文件"
	rm UtilityCommon.o  Singleton.o nmath.o nnode.o nlist.o Nstring.o
