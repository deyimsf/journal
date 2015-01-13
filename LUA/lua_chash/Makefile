CC				:=		gcc
LD				:= 		gcc
CFLAGS			:=		-O3 -Wall -g -std=c99 -pedantic

CHASH_CFLAGS 	:=		-fPIC
CHASH_LDFLAGS	:=		-shared
LUA_INCLUDE_DIR	:=		/usr/local/include
PREFIX			:=		/export/servers/nginx
CHASH_DEST_DIR	:=		$(PREFIX)/conf/lualib

TARGET	:=	chash.so
OBJS	:=	lua_chash.o

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFAGS) $(CHASH_LDFLAGS) -o $@ $^

.c.o:
	$(CC) -c $(CFLAGS) $(CHASH_CFLAGS) -I$(LUA_INCLUDE_DIR) -o $@ $<	

install:
	mkdir -p $(CHASH_DEST_DIR)
	cp $(TARGET) $(CHASH_DEST_DIR)

clean:
	rm -f *.o $(TARGET)

