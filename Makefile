# VARIABLE DECLARATION
######################

OBJS=  iniparser.o smag_main.o main.o agros.o
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
	$(CC) $(CFLAGS) -lreadline -o agros $(OBJS) dictionary.o
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
	$(CC) $(CFLAGS) -c -I include/ src/main.c
	
agros.o: smag_main.o src/agros.c include/agros.h include/smags.h
	$(CC) $(CFLAGS) -c -I include/ -l readline src/agros.c

smag_main.o: src/smag_main.c include/smags.h
ifdef SYSCONF
	$(CC) $(CFLAGS) -c -I include/ src/smag_main.c -DCONF_FILE=$(SYSCONF)
else
	$(CC) $(CFLAGS) -c -I include/ src/smag_main.c
endif

iniparser.o: src/dictionary.c src/iniparser.c include/dictionary.h include/iniparser.h
	$(CC) $(CFLAGS) -c -I include/ src/dictionary.c -o dictionary.o
	$(CC) $(CFLAGS) -c -I include/ src/iniparser.c

# PHONY RULES
#############

.PHONY : clean
	
clean:
	-rm -f agros $(OBJS)
