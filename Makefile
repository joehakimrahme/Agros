# VARIABLE DECLARATION
######################

OBJS= main.o agros.o
CC=gcc
CFLAGS=-Wall -Wextra -Werror

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
	$(CC) $(CFLAGS) -c -I include/ src/main.c
	
agros.o: src/agros.c include/agros.h
ifdef SYSCONF
	$(CC) $(CFLAGS) -c -I include/ `pkg-config --cflags glib-2.0` -DCONFIG_FILE=$(SYSCONF) src/agros.c
else
	$(CC) $(CFLAGS) -c -I include/ `pkg-config --cflags glib-2.0` src/agros.c
endif

smags: src/smag_main.c
	$(CC) $(CFLAGS) -I include/ src/smag_main.c -o smags -L lib/iniparser -liniparser -I lib/iniparser/src
	
# PHONY RULES
#############

.PHONY : clean
	
clean:
	-rm -f agros $(OBJS)
