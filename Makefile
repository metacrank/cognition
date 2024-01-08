##
# Project Title
#
# @file
# @version 0.1

all:
	cc *.c -o stem -lm

clean:
	rm stem

install:
	cp stem /usr/local/bin/
# end
