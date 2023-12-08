
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lSDL2

SRC = main.c
OBJ = $(SRC:.c=.o)

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

clean:
	rm -f $(OBJ) $(SRC:.c=.d) $(TARGET)
