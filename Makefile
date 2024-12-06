CC = mpic++
SRCS = ./src/*.cpp
INC = ./src/
OPTS = -O3

EXEC = nbody

all: clean compile

compile:
	$(CC) $(SRCS) $(OPTS) -I$(INC) -o $(EXEC) -lm

clean:
	rm -f $(EXEC)
