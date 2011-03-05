# VARIABLE DECLARATION
######################

OBJS= main.o agros.o
CC=gcc
CFLAGS=-Wall

# Modify the format to suit gcc
ifdef SYSCONFDIR
    SYSCONF=\"$(SYSCONFDIR)/agros.conf\"
else 
    SYSCONF=\"$(CURDIR)/agros.conf\"
endif




# RULES
###########

# Default Rule. It all starts here
agros: $(OBJS)
	$(CC) $(CFLAGS) `pkg-config --libs glib-2.0` -o agros $(OBJS)
	rm -f $(OBJS)

# Moves the executable to TARGETDIR if defined
ifdef TARGETDIR
	mv $@ $(TARGETDIR)/agros
endif

# Copies the conf file to SYSCONFDIR if defined
ifdef SYSCONFDIR
	cp agros.conf $(SYSCONFDIR)
endif

main.o: agros.o include/agros.h
ifdef SYSCONFDIR
	$(CC) $(CFLAGS) -c -I include/ `pkg-config --cflags glib-2.0` -DCONFIG_FILE=$(SYSCONF) src/main.c
else
	$(CC) $(CFLAGS) -c -I include/ `pkg-config --cflags glib-2.0` src/main.c
endif

agros.o: src/agros.c include/agros.h
	$(CC) $(CFLAGS) -c -I include/ src/agros.c




# PHONY RULES
#############

.PHONY : clean

clean:
	-rm -f agros $(OBJS)
