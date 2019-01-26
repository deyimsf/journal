CC		:=	gcc
LD		:=  gcc
CFLAGS	:= -g -W -std=c99
LDFLAGS := -lcurses
#VPATH	:= src include

all: MyC

MyC: main.o part.o
	gcc -o demo $^
	@echo "111"	


main.o: main.c
part.o: part.c part.h


clean:
	rm -rf ./*.o
