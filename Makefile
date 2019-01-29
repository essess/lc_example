src = lc.c
obj = $(src:.c=.o)
dep = $(obj:.o=.d)

CC  = gcc
CPP = g++

LDFLAGS =
CFLAGS  = -Wall -g3

liblc.a: $(obj)
	ar rcs $@ $^

lc: $(obj)
	$(CC) -c -o $@ $^ $(LDFLAGS)

-include $(dep)
%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) $(dep) liblc.a