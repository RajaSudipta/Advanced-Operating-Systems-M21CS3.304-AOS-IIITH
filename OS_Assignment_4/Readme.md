# OS Assignment 4
## Features
### 1. Disk Menu:
#### 1.1: create disk
#### On press 1: Take unique disk name as input on next line.
#### 1.2: mount disk
#### On press 2: Open the disk for mounting purposes(display options of section2 for that disk).
#### 1.3: exit
#### On press 3: Exit the application.
### 2. Mounting Menu:
#### 2.1: create file
#### On press 1: Take unique file name as input on next line.
#### 2.2: open file
#### On press 2: Take file name as input on next line.
#### Then take file mode as input as mentioned below on next line:
#### 0: read mode
#### 1: write mode
#### 2: append mode
#### ** After specifying the mode, display the file descriptor allocated to the opened file along with the mode in which the file is opened.
#### 2.3: read file
#### - On press 3: Take input file descriptor of the file which you want to read.
#### - File descriptor has been obtained in the open file command.
#### 2.4: write file
#### - On press 4: Take input file descriptor of the file which you want to write.
#### - File descriptor has been obtained in the open file command.
#### - Enter file content that you want to write in the file.
#### 2.5: append file
#### - On press 5: Take input file descriptor of the file to which you want to append further text.
#### - File descriptor has been obtained in the open file command.
#### - Enter the file content that you want to append to the file.
#### 2.6: close file
#### - On press 6: Take the input file descriptor of the file to which you want to close.
#### 2.7: delete file
#### - On press 7: Take the input file name which you want to delete.
#### 2.8: list of files
#### - On press 8: List all existing files on the disk.
#### 2.9: list of opened files
#### - On press 9: List all existing files which are currently open along with their file descriptors and mode in which they are open.
#### 2.10: unmount
#### - On press 10: Unmount/close the disk which is current mount(in which you are working currently) and return to the previous menu.