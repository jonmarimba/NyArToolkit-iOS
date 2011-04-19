NAME_EXE=rawtest
INCLUDE=-I../../inc
LIB=../libNyARToolkit/lib/libnyartoolkit.a


all:rawtest

rawtest:main.o
	g++ $^ $(LIB) -o $@


clean:
	rm -f $(NAME_EXE) $(NAME_EXE).exe main.o

.cpp.o:
	$(CC) -O2 -c $(INCLUDE) $< -o $@