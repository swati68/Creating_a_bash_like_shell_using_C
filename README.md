# Creating_a_bash_like_shell_using_C
C code to implement various functions of a bash shell

Functions implemented:-

1. Cat command: One of the most frequently used command in Linux. It allows to create
single or multiple files, view content of a file and append text to the file. It is also used to create
a file and add text.

2. Add command: This command adds the content of the first file to the second file.

3. Grep command: The grep command is used to search a specific text in the given file. It
outputs the entire line where the word has been found.

4. Find command: The entire Linux filesystem is in the form of a tree structure. Using a
recursive function, one can implement the command to find a file in the given directory. It
outputs the relative path of the file from the given directory as well.

5. Mv command: This command moves the contents of one file to another one and deletes
the first file. This is also used to rename a file.

6. Ps command: On parsing the /proc directory, the command can be used to display
information about all the processes on the system such an Pid, Name, Status, etc.

7. Touch command: This command creates a new file without adding any content. The file
is created for later purposes.

8. Mtime command: The command is used to modify the timestamp of a particular file.

9. Log command: This command maintains history of all the commands entered in the
shell along with the timestamp and displays it to the user. Before exiting the shell, it stores the
buffer content to a log file.

10. Cd command: It is used to change the working directory to the given directory.

11. Help command: It displays information about all the commands available in the shell.

12. Echo command: The echo command displays any text that was entered with the command
back to the user.

13. Ls command: This command lists out all the files present in the working directory.

14. Wc command: The command is used for printing line, word,character and byte counts for
file. It can return the number of lines in a file, the number of characters in a file and the number
of words in a file. It can also return the above information for two specified files and print the
total count. The longest line of the file can also be printed by this command.

15. Sort command: The sort command is used to sort the contents of the given file in ascending
or descending order and accordingly it will either display the result on the shell or write it into
the specified file.

16. Rm command: This command is used to remove a file from the working directory.

17. Clear command: The clear commands clears the entire shell console.

18. Cmp command: This command is used to compare the contents of two files. It also displays
the difference in the characters and byte count.

19. Clclog command: This command clears the log buffer and the log file content. It can also
clear the log content of a particular date.

20. Viewlog Command: This command allows the user to view the entire content of the log file
or the contents of a specific date.

21. Uniq command: It is used to report or filter repeated lines in a file. It also shows the count
of occurrences of the lines in the file.

22. Head command: The head command displays the specified number of lines from the start
of the file.

23. Tail command: The head command displays the specified number of lines from the end of
the file.

24. Diff command: This command is used to display the differences in the files by comparing
the files line by line. Unlike cmp, it tells us which lines in one file has to be changed to make the
two files identical.

25. Expt command: This command is used to solve a simple numerical expression. This is
evaluated by converting the expression to postfix expression.

26. Fact command: This command outputs all the prime factors of the given number.

27. Tac command: The tac command outputs the contents of a file in the reverse order.

28. Exit command: This command is used to exit the created shell.
