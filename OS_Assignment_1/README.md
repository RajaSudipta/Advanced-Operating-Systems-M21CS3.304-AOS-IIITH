# Terminal based file explorer 
## Overview
- File explorer is work in two modes :
    - Normal mode   
    - Command mode 
- The application is start in normal mode, which is the default mode and used to explore the current directory and navigate around in the filesystem.  
- The root of the application is the directory where the application was started.  
- The last line of the display screen is used as status bar -  used in normal and command-line modes. 
## Execution
>`$ g++ main.cpp`
>
>`$ ./a.out `
>
**Platform**: Linux

## Working
##### 1.Normal mode 
-  Display a list of directories and files in the current folde. 
- Every file in the directory is displayed on a new line with the following attributes for each file - 
    - File Name 
    - File Size 
    - Ownership (user and group) and Permissions 
    - Last modified 
- The file explorer handles scrolling in the case of vertical overflow using keys k & l. 
- User will be able to navigate up and down in the file list using the corresponding up and down arrow keys 
- Open directories and files 
- When enter key is pressed on the - 
    - Directory – Screen will be cleared and navigated into the directory and show the directory contents as specified in point 1 
    - File - Opens the file in any default editor. 
- Traversal 
  - Go back - Left arrow key takes the user to the previously visited directory   
  - Go forward - Right arrow key takes the user to the next directory 
  - Up one level - Backspace key takes the user up one level 
  - Home - h key takes the user to the home folder (the folder where the application was started) 

##### 2.Command Mode 
The application enters the Command button whenever “:” (colon) key is pressed. In the command mode, the user will be able to enter different commands. All commands appears in the status bar at the bottom. 
- Copy – copy files/ directorires ‘copy <source_file(s)> <destination_directory>’  
  `eg- copy foo.txt bar.txt baz.mp4 ~/foobar` 
- Move – move files/ directories ‘move <source_file(s)> <destination_directory>’ 
`eg- move foo.txt bar.txt baz.mp4 ~/foobar` 
- Rename - ‘rename <old_filename> <new_filename>’   
`eg- rename foo.txt bar.txt` 
- Create File - ‘create_file <file_name> <destination_path>’  
`eg - create_file foo.txt ~/foobar` 
- Create Directory - ‘create_dir <dir_name> <destination_path>’ 
`eg- create_dir foo ~/foobar` 
- Delete File - ‘delete_file <file_path>’ 
- Delete Directory - ‘delete_dir <dir_path>’ 
The file/dir path is be relative to the root from where the application is run. 
- Goto - ‘goto <location>’ 
`eg - goto <directory_path>` 
- Search - ‘search <file_name>’ or ‘search <directory_name>’ 
- On pressing ESC key, the application goes back to Normal Mode  

## Assumptions
- 'q' can be typed in normal mode to quit the program. 
- Files or folders with space in their name are not supported. eg to create folder named "my folder" in Home directory, type : 
   >create_dir my_folder ~  (valid) 
   >
   > create_dir my folder ~  (invalid) 
- Preferable to open in full size terminal. If one record spans over more than one line then navigation won't work correctly which means if width of terminal is less and output wraps around to new line, the code will not be able to handle this condition. 
- In command mode, to edit the command, use backspace only. Arrow keys are not handled. 
- In the normal mode, if ENTER is pressed on some file and if there is no default application found then there will be no movement.
