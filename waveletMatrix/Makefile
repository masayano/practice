# マクロ定義部
CC      = g++
OBJS    = main.o WaveletMatrix.o common.o

# 生成規則部
waveletMatrix: $(OBJS)
	$(CC) -o $@ $(OBJS)

.c.o:
	$(CC) -c $<

main.o: main.cpp
WaveletMatrix.o: WaveletMatrix.cpp
common.o: common.cpp

clean:
	rm *.o waveletMatrix
