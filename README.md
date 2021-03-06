AGROS - The New Limited Shell
=============================
AGROS is my implementation in C of the Limited Shell (lshell). Lshell is a
project developed by ghantoos (mouzannar@gmail.com), The project consists of a
UNIX shell coded in Python, that restricts users to a set of predefined
commands. You can find the original project here:

http://lshell.ghantoos.org.

AGROS aims at implementing lshell in C. The idea being to get rid of certain
limitations of Python, namely:

* The availability of the Python interpreter on a machine.
* Versions management. (Lshell does not yet support Python 3)
* Performance issues, most notably on embedded systems.

This is why AGROS is rightfully named AGROS Got Rid Of the Snake. I'm pretty
proud of that name :)

AGROS has a submodule called SMAGS, that recently replaced the use of GLib for
parsing the conf file. SMAGS is based on the 'iniparser' library written by
ndevilla. Here's a link to the original project:

http://ndevilla.free.fr/iniparser/


Latest Release
--------------
* Release Name: agros-0.4.0
* Release Date: 11/12/2011


Install
-------
You can compile the program by running `make`. If the dependencies are present,
it will create a binary executable named `agros` that can be directly runnable.

If make is called without any arguments, then agros will look for a file called
`agros.conf` in the current directory.

You can give two command line arguments to make:

* `SYSCONFDIR`: Determines the directory where `agros.conf` is located. make will
  also copy automatically the conf file to `SYSCONFDIR`
* `TARGETDIR`: Determines the directory where the executable will be moved.


Configuration
-------------
There are five variables defined in the agros.conf file.

* `allowed`: Gives a list of the allowed commands separated by a semicolon ";"
* `forbidden`: Defines a list of forbidden characters in the command line.
* `welcome` (optional): Displays a welcome message
* `loglevel` (optional): Defines the level of log sent to syslog
    0. No logging at all (default)
    1. Logging only prohibited commands
    2. Logging commands that returned an error
    3. Logging everything
* `warnings`: Sets a number of warnings that decreases every time the user
            enters a forbidden command. When warnings reach 0, AGROS exits.


GNU Readline
------------
Agros has an experimental version implementing the GNU Readline library for
handling user input. It comes with nice features like autocompletion or command
history.

So far the implementation is still buggy and not very stable. Which is why
development is maintained on a separate git branch. Use with caution as there is
a high probablity of random crashes.


Contact
-------
If you want to contribute to this project, please do not hesitate to send me
comments or code patchs at my email adress:

Joe "rahmu" HAKIM RAHME <joe.hakim.rahme@gmail.com>

You can also fork the repository on github.com, and send me pull
requests. Here's the repo's URL:

https://github.com/rahmu/Agros
