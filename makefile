
.PHONY = all

all: hi
	
hi: hlang.c
	gcc $< -o $@

