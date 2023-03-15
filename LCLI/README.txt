Project 1:
1. Run "g++ -o CL CL.c" to compile
2. Run "./CL.c" to execute
3. Enter any commands ("quit" to exit)
4. "chmod +x CLBatch.sh" to make batch executable
5. "./CLBatch" to execute batch

Project 2 Additions:
Use the "new" command to create a new process that will be split into pages in the page table.
Use the "table" command to display the contents of the table and total memory usage.
Use the "run" command to run selected processes.

Running processes will change the priority of the processes, causes process that have run more to be more likely
to stay in the page table when adding new pages. Processes with lower priority are more likely to be replaced. Use
the "table" command to see how many runs (USES) a project has undergone as well as the memory each page contains.

The PROCESS ID allows you to see which pages are associated with what processes. This is also displayed in the table.

Project 3 Additions: 
The "run" command can now run multiple processes by sending them to the process scheduling algorithm (firstComeFirstServe).
The processes will be run in the order that they were chosen by the user. The information about the processes can still be
viewed in the page table by entering the "table" command.

Project 4 Additions: 
Can take in commands related to File Management. File management command list (example format in parentheses):

touch- creates an empty file ("touch <new file>")
vi- allows the user to edit a file ("vi <file to edit>")
rm- removes a file ("rm <file to delete>")
mkdir- makes a directory ("mkdir <new directory>")
rmdir- removes an empty directory ("rmdir <directory to delete>"|"rm -r <directory to delete>" to remove non-empty directory)
mv- moves or renames a file or directory ("mv <current name> <new name>")
cp- duplicate a file or directory (cp <original> <copy of original>)
find- find a file ("find <file>")
ls- shows basic info for all files and directories in current directory ("ls"| type "ls -l" for more detailed information)