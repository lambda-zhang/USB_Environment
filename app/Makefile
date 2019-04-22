#
# Makefile
#

APP = main

all: $(APP)

clean:
	rm -f $(APP)

$(APP): $(APP).c
	$(CC) -Wall -o $@ $(APP).c -lusb

run:
	./${APP}
