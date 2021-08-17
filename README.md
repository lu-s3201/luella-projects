# Similar Lines Detection Program

The program determines matching word groups that exist within a number
of files, and prints the matching words within the group, as well as 
the filename & line number in which it's found in.

Readaline was implemented such that:
- It reads a single line of output of any length from an input file pointer
- It terminates gracefully if the input file is null
- It stores the contents of the line into the datapp pointer
- It returns the number of bytes contained in the read line
- It does not crash with non-ASCII characters
- It does not leak memory

Simlines was implemented such that:
- It opens any number of input files and reads the contents within
- It successfully utilizes readaline to assist in reading lines from a file
- The program does not count “non-word” strings in its calculations for match 
  groups
- The program gracefully exits if it encounters:
   - Invalid files
   - Errors during memory allocation
