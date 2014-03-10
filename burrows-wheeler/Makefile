# マクロ定義部
CC      = g++
OBJS    = burrows-wheeler.o encoding.o decoding.o

# 生成規則部
burrows-wheeler: $(OBJS)
	$(CC) -o $@ $(OBJS)

.c.o:
	$(CC) -c $<

burrows-wheeler.o: burrows-wheeler.cpp
encoding.o: encoding.cpp
decoding.o: decoding.cpp

clean:
	rm *.o burrows-wheeler
