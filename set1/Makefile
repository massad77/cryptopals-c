EXE = convert
CFLAGS = -g -Wall
LDFLAGS = -g
SRC = main.c
OBJ = main.o

# default rule: compile everything
all: $(EXE)

# link
$(EXE): $(OBJ)
	gcc $(LDFLAGS) -o $@ $^

# compile
%.o : %.c
	gcc $(CFLAGS) -c -o $@ $< 

# clean
.PHONY : clean
clean:
	rm -rf $(OBJ)
