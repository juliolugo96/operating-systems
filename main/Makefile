CC= gcc
INCLUDE= -I../inc
FLAGS= -lm -lpthread
OBJSDIR= ../obj/
SRCDIR= ../src/
MAIN= ../bin/main.exe
WIDE= ../bin/wide.exe
NARROW= ../bin/narrow.exe
MAINOBJECTS= $(OBJSDIR)utils.o $(OBJSDIR)main.o
WIDEOBJECTS= $(OBJSDIR)utils.o $(OBJSDIR)wide.o
NARROWOBJECTS= $(OBJSDIR)utils.o $(OBJSDIR)narrow.o
SOURCES= $(SRCDIR)utils.c main.c


$(MAIN):	$(MAINOBJECTS)
	$(CC) $(INCLUDE) $^ $(FLAGS) -o $@

$(NARROW):	$(NARROWOBJECTS)
	$(CC) $(INCLUDE) $^ $(FLAGS) -o $@

$(WIDE):	$(WIDEOBJECTS)
	$(CC) $(INCLUDE) $^ $(FLAGS) -o $@

$(OBJSDIR)narrow.o: narrow.c
	$(CC) $(INCLUDE) -c $< $(FLAGS) -o $@

$(OBJSDIR)wide.o: wide.c
	$(CC) $(INCLUDE) -c $< $(FLAGS) -o $@

$(OBJSDIR)main.o: main.c
	$(CC) $(INCLUDE) -c $< $(FLAGS) -o $@

$(OBJSDIR)utils.o: $(SRCDIR)utils.c
	$(CC) $(INCLUDE) -c $< $(FLAGS) -o $@
	
clean:
	rm ../obj/*.o ../bin/*.exe