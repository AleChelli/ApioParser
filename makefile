# part of the makefile
APIO: APIO.l APIO.h APIO.y
	bison -d APIO.y
	flex APIO.l
	cc -o $@ APIO.tab.c lex.yy.c APIO.c