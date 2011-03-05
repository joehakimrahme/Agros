OBJS= main.o agros.o
CC=gcc
CFLAGS=-Wall
ifdef SYSCONFDIR
    SYSCONF=\"$(SYSCONFDIR)/agros.conf\"
endif

#first_rule:
#	echo $(SYSCONF)

# Default Rule. It all starts here
agros: $(OBJS)
	$(CC) $(CFLAGS) `pkg-config --libs glib-2.0` -o agros $(OBJS)
	rm -f $(OBJS)
ifdef TARGETDIR
	mv $@ $(TARGETDIR)/agros
endif
ifdef SYSCONFDIR
	cp agros.conf $(SYSCONFDIR)
endif


main.o: agros.o include/agros.h
	$(CC) $(CFLAGS) -c -I include/ `pkg-config --cflags glib-2.0` -DCONFIG_FILE=$(SYSCONF) src/main.c

agros.o: src/agros.c include/agros.h
	$(CC) $(CFLAGS) -c -I include/ src/agros.c

.PHONY : clean

clean:
	-rm -f agros $(OBJS)
