TARGET=libial.so
OBJS=keyboard_pc.o keyboard_bd.o keyboard_ial.o mouse_ps2.o mouse_ts.o mouse_ial.o
CFLAGS=-fPIC -g -fgnu89-inline
LDFLAGS=-lc -lm
%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
$(TARGET): $(OBJS)
	$(CC) -g -shared -Wl,-soname,$@ -o $@ $^ $(LDFLAGS)
clean:
	rm -rf *.o
	rm -rf libial.so
