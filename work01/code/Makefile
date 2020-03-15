#Version2
CC=g++
CFLAGS=-c 
OBJS=main.o f1.o f2.o 

hello:$(OBJS)
	$(CC) $^ -o $@
.PHONY:clean
clean:
	rm -rf *.o 

#Version1
# hello:main.o f1.o f2.o 
# 	$(CC) main.o f1.o f2.o -o hello
# main.o:main.cpp
# 	$(CC) $(CFLAGS) main.cpp
# f1.o:f1.cpp
# 	$(CC) $(CFLAGS) f1.cpp
# f2.o:f2.cpp
# 	$(CC) $(CFLAGS) f2.cpp
# .PHONY:clean
# clean:
# 	rm -rf *.o 