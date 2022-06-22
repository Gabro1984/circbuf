CC = g++
CFLAGS = -Wall -g -std=c++17 -pthread
INCLUDES = -Iinc
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = test

.PHONY: depend clean

all:    $(MAIN)
	@echo  $(MAIN) has been compiled

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS)

main.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c main.cpp

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^