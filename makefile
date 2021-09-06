
.PHONY = all clean

SRC := ${wildcard *.c}
OBJ := ${patsubst %.c, %.o, ${SRC}}

all: hi
	
	
hi: ${OBJ}
	echo ${OBJ}
	gcc $^ -o $@
	rm $^

${OBJ}: %.o: %.c
	gcc -c $< -o $@ -g

clean:
	rm hi ${OBJ}

