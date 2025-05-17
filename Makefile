 CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/container.c src/data_structures.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(OBJ)

# Pattern rule to build each .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run a test like: make test_vector
test_%: tests/test_%.c $(OBJ)
	$(CC) $(CFLAGS) $^ -o run_$@
	./run_$@

# Run container (main program)
run_container: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o run_container
	./run_container

# Clean build artifacts
clean:
	rm -f $(OBJ) run_* run_container