src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)

CC  = gcc
CPP = g++

LDFLAGS = -L/usr/local/lib
LDLIBS  = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt

INCLUDE = -I/usr/local/include
DEBUG   = -g -O0
CFLAGS  = $(DEBUG) -Wall $(INCLUDE)

lc_example: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

-include $(dep)
%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) $(dep) lc_example
