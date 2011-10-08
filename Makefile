# VARIABLE DECLARATION
######################

OBJS=  smag_main.o main.o agros.o
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
	$(CC) $(CFLAGS) -L lib/iniparser -liniparser -o agros $(OBJS)
	rm -f $(OBJS)
	
# Moves the executable to TARGETDIR if defined
ifdef TARGETDIR
	mv $@ $(TARGETDIR)/agros
endif
	
# Copies the conf file to SYSCONFDIR if defined
ifdef SYSCONFDIR
	cp agros.conf $(SYSCONFDIR)
endif
	
main.o: agros.o smag_main.o src/main.c include/agros.h include/smags.h
	$(CC) $(CFLAGS) -c -I include/ -L lib/iniparser -liniparser -I lib/iniparser/src src/main.c
	
agros.o: smag_main.o src/agros.c include/agros.h include/smags.h
ifdef SYSCONF
	$(CC) $(CFLAGS) -c -I include/ -L lib/iniparser -liniparser -I lib/iniparser/src -DCONFIG_FILE=$(SYSCONF) src/agros.c
else
	$(CC) $(CFLAGS) -c -I include/ -L lib/iniparser -liniparser -I lib/iniparser/src src/agros.c
endif

smag_main.o: src/smag_main.c include/smags.h
	$(CC) $(CFLAGS) -c -I include/ src/smag_main.c -L lib/iniparser -liniparser -I lib/iniparser/src
	
# PHONY RULES
#############

.PHONY : clean
	
clean:
	-rm -f agros $(OBJS)
