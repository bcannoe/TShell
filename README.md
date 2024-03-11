This is a simple shell project I made to help me understand the Linux filesystem better and make myself familiar with C++ libraries, This project is currently incomplete and some parts of it could be potentially unsafe to use.
This project was made to run on a Linux machine and uses Unix libraries which will make this program not work on Windows systems. This program was developed on Ubuntu 22.04.4 and I am uncertain if it will work on distrobutions that are not
based on Debian.

Currently the commands supported by TShell are
"cpath" This will move the users current directory to a new directory, currently only absolute file paths are supported.
"mk filename.extension :text in file" this will make a new file anything after the ":" symbol will be placed inside the file.
"rm filename.extension" this will remove a file from your system, although it should not be allowed to remove critical files I did not test that so don't try it.
"pl" this will list out all the process ID's and the name of some identifiable processes this may not work on non-Debian systems.
"pk [PID number]" this will allow the user to terminate a process from the command line, this can crash your computer, so it is best to see which process is being terminated before executing the command.
"lpath" lists out all directories and files in current directory.
"history" lists out previous commands that the user gave.

This project still has a lot of room for improvement and more features I would try to add, such as features to copy files, move files, concatinate multiple files together, append text to a file, making new directories, and moving directories.
along with cleaning up some of the output related to file paths and the PIDs.
