
CC = cc
CFLAGS = -g -Wall -Wextra
LDFLAGS = 


NGX_HOME = /My/workspace/git/nginx-1.9.4

NGX_INCS = -I$(NGX_HOME)/src/core \
	   -I$(NGX_HOME)/src/os/unix \
	   -I$(NGX_HOME)/objs \
	   -I/My/Tool/pcre-8.35

NGX_CORE_O = $(wildcard $(NGX_HOME)/objs/src/core/*.o)
NGX_EVENT_O = $(wildcard $(NGX_HOME)/objs/src/event/*.o)
NGX_OS_O = $(wildcard $(NGX_HOME)/objs/src/os/**/*.o)
NGX_OTHER_O = $(NGX_HOME)/objs/src/event/modules/*.o \
	      $(NGX_HOME)/objs/ngx_modules.o
NGX_O = $(NGX_CORE_O) $(NGX_EVENT_O) $(NGX_OS_O) $(NGX_OTHER_O)


TARGET = array

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(NGX_INCS) $(NGX_O) \
	/My/Tool/pcre-8.35/.libs/libpcre.a -lcrypto -lz $^ -o $@

clean:
	rm -r $(TARGET)
