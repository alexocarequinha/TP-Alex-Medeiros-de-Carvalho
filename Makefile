CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c team.c teamlist.c match.c matchlist.c
OBJS = $(SRCS:.c=.o)
TARGET = campeonato

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET) classificacao.csv

run: $(TARGET)
	./$(TARGET)
