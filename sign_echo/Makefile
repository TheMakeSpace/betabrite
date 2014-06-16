CC=gcc
CC_FLAGS=-Wall

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
BIN=sign_echo

all : $(BIN) clean_objs

debug : $(BIN)

$(BIN) : $(OBJS)
	$(CC) $(CC_FLAGS) $(OBJS) -o $(BIN) 

%.o : %.c
	$(CC) -c $(CC_FLAGS) $< -o $@

clean :
	rm -f $(OBJS) $(BIN) *~

clean_objs :
	rm -f $(OBJS)
