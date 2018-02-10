all: pyxoroshiro.o
pyxoroshiro.o:
	$(CC) -g -fPIC `pkg-config --libs --cflags python3` -g -o pyxoroshiro.so --shared  -Iinclude lib/pyxoroshiro.c
