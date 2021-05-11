# Shell-Program
The objective of this project is to create a small shell program with custom commands and learn how to use some of the C system command libraries.  A list of what commands are currently
fully implemented is defined below.

1. fnew "fileName" - Creates a new file with the name "fileName"
2. whereami - Displays the user's current directory location
3. movetodir "DirectoryName" - Checks if "DirectoryName" exists and then reassigns a string variable to that directory.
4. exist "name" - Determines if "name" is either a directory or file and then displays whether either one exists in the system
5. copy "sourceFileName" "destinationFileName" - Copies the contents of "sourceFileName" to "destinationFileName"
6. history - Displays a numbered list of the entire history of commands used in the current terminal session
    - When "clr" is appended to the history command, this clears the entire history data structure.
7. replay "commandNum" - Re-executes the selected command defined by "commandNum". This variable corresponds to a command's value in the history data structure, thus it is
  advisable to check the history prior to using this command as the wrong command may be used.

Due to a lack of effective support for process handling on Windows OS, this shell does not currently incorporate a program execution command.  However, once testing on Linux 
becomes possible I will continue to expand the available commands to include this and others.
