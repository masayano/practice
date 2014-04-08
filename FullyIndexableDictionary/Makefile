# マクロ定義部
CC       = g++
CPPFLAGS = -std=c++11 -Wall
OBJS     = FullyIndexableDictionary.o main.o

# 生成規則部
FullyIndexableDictionary: $(OBJS)
	$(CC) -o $@ $(OBJS)

.c.o:
	$(CC) $(CPPFLAGS) -c $<

main.o: main.cpp
FullyIndexableDictionary.o: FullyIndexableDictionary.cpp

clean:
	rm *.o FullyIndexableDictionary *~
