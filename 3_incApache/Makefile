#CC=clang
CFLAGS= -DHTML_404='"404_Not_Found.html"' \
        -DHTML_501='"501_Method_Not_Implemented.html"' \
        -DIMAGE='"uncadunca.jpg"' \
        -DSTYLE='"000_style.css"'
	# -DPRETEND_TO_BE_ROOT \
	# -DINCaPACHE_2_3 \
	# -DDEBUG \
	# -g3 -O0 -Wall -pedantic
OBJS=aux.o http.o main.o threads.o
BIN_DIR=bin
EXE=$(BIN_DIR)/incapache

.PHONY: clean tgz-students tgz-full

all: $(EXE)

$(EXE): $(OBJS) $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS) -lpthread
	sudo chown root $(EXE)
	sudo chmod u+s $(EXE)

aux.o: aux.c incApache.h
http.o: http.c incApache.h
main.o: main.c incApache.h
threads.o: threads.c incApache.h

clean:
	rm -f $(OBJS) $(EXE)

tgz-students: clean
	cd .. ; tar cvzf incapache-students.tgz --exclude=incapache-students/bin/Debug --exclude=incapache-students/.idea incapache-students

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

tgz-full: clean
	cd .. ; tar cvzf incapache-full.tgz --exclude=incapache-full/bin/Debug --exclude=incapache-full/.idea incapache-full


