# マクロ定義部
CC       = g++
OBJS     = KahanSummationAlgorithm.o
LIBS     = -lboost_timer -lboost_system
CPPFLAGS = -Wall -std=c++11

# 生成規則部
KahanSummationAlgorithm: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) -c $<

KahanSummationAlgorithm.o: KahanSummationAlgorithm.cpp

clean:
	rm *.o KahanSummationAlgorithm *~
