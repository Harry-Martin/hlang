
.PHONY = all clean

all: hi
	
hi: hlang.c
	gcc $< -o $@ -g

clean:
	rm hi

