TARGET			=		chash.so
PREFIX			=		/My/work/nginx

CFLAGS			=		-O3 -Wall -pedantic -DNDEBUG
CHASH_CFLAGS 	=		-fpic -g
CHASH_LDFLAGS	=		-shared
#MacOs
#CHASH_LDFLAGS   =       -bundle -undefined dynamic_lookup

LUA_INCLUDE_DIR	=		$(PREFIX)/luajit/include/luajit-2.1

#install to
LUA_CMODULE_DIR =		$(PREFIX)/nginx/conf/lualib
LUA_MODULE_DIR  =       $(LUA_CMODULE_DIR)

BUILD_CFLAGS	= 		-I$(LUA_INCLUDE_DIR) $(CHASH_CFLAGS)
OBJS			=		lua_chash.o

.PHONY: all clean install

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(BUILD_CFLAGS) -o $@ $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(CHASH_LDFLAGS) -o $@ $^

install:$(TARGET)
	mkdir -p $(DESTDIR)/$(LUA_CMODULE_DIR)
	install $(TARGET) $(DESTDIR)/$(LUA_CMODULE_DIR)
	@#chmod 755 $(DESTDIR)/$(LUA_CMODULE_DIR)/$(TARGET)
	@echo $(DESTDIR)
clean:
	rm -f *.o $(TARGET)

