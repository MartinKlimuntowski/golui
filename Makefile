CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lSDL2

SRC = main.c
OBJ = $(SRC:.c=.o)
HDR = $(SRC:.c=.h)



TARGET = gol

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Rule to generate dependency files
%.d: %.c
	$(CC) $(CFLAGS) -MM $< -o $@

# Include generated dependency files
-include $(SRC:.c=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

commit: clean
	git add $(SRC) $(HDR) Makefile
	git commit -m "make commit"

clean:
	rm -f $(OBJ) $(SRC:.c=.d) $(TARGET)