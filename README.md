# CShell
A C program that emulate a simple Unix shell.

Users can customize the shell prompt by running ./shell -p followed by prompt name.
  
The shell supports both built in and regular Unix program commands.
  
Built in commands:
---
  pid - the shell should print its process ID:
  
  ppid - the shell should print the process ID of its parent

  cd dir - change the working directory of the shell process. With no arguments, change to the user’s home directory (which is stored in the environment variable HOME)

  pwd - print the current working directory

  exit – the shell should terminate and accept no further input from the user It can also programs PID.
