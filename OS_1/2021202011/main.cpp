#ifndef HEADERS_H
#define HEADERS_H
#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<time.h>
#include<string.h>
#include<termios.h>
#include<iostream>
#include<pwd.h>
#include<grp.h>
#include<set>
#include<bits/stdc++.h>
#include <sys/ioctl.h>
#include<sys/wait.h>
using namespace std;
#define ESC  27
#define ENTER 10
#define HOME 'h'
#define BACK 127
#define COLON 58
#define QUIT_big 81
#define QUIT_small 113
#define set_cursor_pos(x,y) printf("%c[%d;%dH",ESC,x,y)
#define eraseline printf("%c[%dK",ESC,2);
#define clearfrom printf("\e[0J");
#define eraseCharacter printf("\x1b[0K");
#define clearScreen printf("\33c");
#define cursor_up printf("\033[1A");
#define cursor_down printf("\033[1B");
stack<string> left_stack;
stack<string> right_stack;
set<string> search_result;
string current_directory;
char home[PATH_MAX];
// const char *home;
struct winsize w;
struct termios oldtio;
struct termios orig_termios;
int switch_normal_command_mode;
void enable_cursor_movement(vector<string>v, int w);
void listDirectory(const char *dirname, int w);
#endif


vector<string> split(const string str, char delim) {
    vector<string> res;
    size_t start=0, end=0;
    while ((start = str.find_first_not_of(delim, end)) != string::npos) {
        end = str.find(delim, start);
        res.push_back(str.substr(start, end - start));
    }
    return res;
}

string convertToString(double num) {
    ostringstream convert;
    convert << num;
    return convert.str();
}

double roundOff(double n) {
    double d = n * 100.0;
    int i = d + 0.5;
    d = (float)i / 100.0;
    return d;
}

string convertSize(size_t size) {              
    static const char *SIZES[] = { "B", "KB", "MB", "GB" };
    int div = 0;
    size_t rem = 0;

    while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
        rem = (size % 1024);
        div++;
        size /= 1024;
    }

    double size_d = (float)size + (float)rem / 1024.0;
    string result = convertToString(roundOff(size_d)) + " " + SIZES[div];
    return result;
}

void display_file_contents(string str) {
    vector<string> file_folder_name = split(str, '/');
    cout << setw(10) << file_folder_name[file_folder_name.size()-1] << "\t";
    struct stat st;
    string permission;
    if(stat(str.c_str(), &st) == 0){
        /* engraving the file/directory type and the permissions */
        mode_t perm = st.st_mode;
        permission += (S_ISDIR(st.st_mode) ? "d" : "-");
        permission += (perm & S_IRUSR) ? 'r' : '-';
        permission += (perm & S_IWUSR) ? 'w' : '-';
        permission += (perm & S_IXUSR) ? 'x' : '-';
        permission += (perm & S_IRGRP) ? 'r' : '-';
        permission += (perm & S_IWGRP) ? 'w' : '-';
        permission += (perm & S_IXGRP) ? 'x' : '-';
        permission += (perm & S_IROTH) ? 'r' : '-';
        permission += (perm & S_IWOTH) ? 'w' : '-';
        permission += (perm & S_IXOTH) ? 'x' : '-';
        cout << setw(10) << permission << " ";

        /* engraving the file size in human readable format */
        /* http://memoirsofacoder.blogspot.com/2014/08/c-get-file-size-in-kbmbgb-format.html */
        size_t file_size = st.st_size;
        cout << setw(10) << convertSize(file_size) << "\t";
        
        string uid = getpwuid(st.st_uid)->pw_name;   
        cout << uid << "\t";
        string gid = getgrgid(st.st_gid)->gr_name;
        cout << gid << "\t";
        // printf("%s\t",uid.c_str());
        // printf("%s\t",gid.c_str());

        /* Last modified time */
        /* https://gist.github.com/StrikeW/11116799 */
        auto mod_time = st.st_mtime;
        // cout << mod_time;
        char mtime[80];
        struct tm lt;
        localtime_r(&mod_time, &lt); /* convert to struct tm */
        // strftime(mtime, sizeof(mtime), "%a, %d %b %Y %T", &lt);
        strftime(mtime, sizeof(mtime), "%b %d %T", &lt);
        cout << setw(10) << mtime << "\t";
    }
}

void openFile(const char *filename) {
    if (fork() == 0) {
		execl("/usr/bin/xdg-open", "xdg-open", filename, (char *)0);
		exit(1);
	}
}

// static void list_dir(const char *path) {
//     struct dirent *entry;
//     DIR *dir = opendir(path);
//     if (dir == NULL) {
//         return;
//     }

//     while ((entry = readdir(dir)) != NULL) {
//         printf("%s\n",entry->d_name);
//     }

//     closedir(dir);
// }

/*** terminal ***/
void die(const char *s) {
    perror(s);
    exit(1);
}

void disableRawMode() {
//   tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
        die("tcsetattr");
    }
}

void enableRawMode() {
    //   tcgetattr(STDIN_FILENO, &orig_termios);
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        die("tcgetattr");
    }
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    //   raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    //   raw.c_oflag &= ~(OPOST);
    //   raw.c_cflag |= (CS8);
    //   raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_lflag &= ~(ECHO | ICANON);

    //   raw.c_cc[VMIN] = 0;
    //   raw.c_cc[VTIME] = 1;

    //   tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
    }
}

// void switch_to_non_cannonical_mode() {
//     if(tcsetattr(0, TCSANOW, &oldtio))    //switch to cananical mode
//     {
//         fprintf(stderr,"Set attributes failed");
//         exit(1);
//     }
// }

// void switch_to_cannonical_mode() {
//     // struct termios oldtio;
//     struct termios newtio;
//     tcgetattr(0, &oldtio); //0 is file discriptor for standard input
    
//     newtio = oldtio; 
//     newtio.c_lflag &= ~(ICANON | ECHO);   //switch to noncanonical and nonecho mode 
//     tcsetattr(0, TCSANOW, &newtio);
// }

// void displayParent_dir(string list,int w) {
//     cout << list << endl;
//     vector<string> path=split(list, '/');
//     string dir='/' + path[0];
//     for(int i=1;i<path.size()-2;i++)
//         dir+='/'+path[i];
//     listDirectory(dir.c_str(),w);    
// }

string getAbsolutePathwithFileName(string path, string fileName)
{
    string homeDirPath = home;
    string currDirPath = current_directory;
    string absolutePath = "";
    if(path[0] == '~')
    {
        absolutePath = absolutePath + homeDirPath + path.substr(1,path.length()-1);
    }
    if(path[0] == '.')
    {
        if(path.length() == 1)
        {
            absolutePath = currDirPath;
        }
        else
        {
            absolutePath = currDirPath + path.substr(1, path.length()-1);
        }
    }
    else if(path[0] == '/')
    {
        absolutePath = absolutePath + homeDirPath + path;
    }
    else
    {
        absolutePath = currDirPath + '/' + path;
    }
    absolutePath = absolutePath + '/' + fileName;

    return absolutePath;
}

string getCompletePathForDirectory(string path)
{
    string completePath = home;
    if(path[0] == '~')
    {
        completePath += path.substr(1, path.length()-1);
    }
    else if(path[0] == '/')
    {
        completePath += path;
    }
    else if(path[0] == '.')
    {
        if(path.length() == 1)
        {
            completePath = current_directory;
        }
        else
        {
            completePath = current_directory + path.substr(1, path.length()-1);
        }
    }
    else
    {
        completePath = current_directory;
        completePath = completePath + '/' + path;
    }
    return completePath;
}

void create_file(vector<string> v)
{
    if(v.size() != 3)
    {
        // cout << "too few arguements" << endl;
        return;
    }
    else
    {
        string path = v[2];
        string fileName = v[1];
        string filePath = getAbsolutePathwithFileName(path, fileName);
        fstream file;
        file.open(filePath, ios::out);
        if(!file)
        {
            // cout<<"Error in creating file!!!";
        }
        // cout<<"File created successfully.";
        file.close();
    }
}

void create_directory(vector<string> v)
{
    if(v.size() != 3)
    {
        // cout << "too few arguements" << endl;
        return;
    }
    else
    {
        string path = v[2];
        string dirName = v[1];
        string dirPath = getAbsolutePathwithFileName(path, dirName);
        int check;
        check = mkdir(dirPath.c_str(), 0777);
  
        // check if directory is created or not
        if (!check) {
            // printf("Directory created\n");
        }
        else {
            // printf("Unable to create directory\n");
        }
    }
}

void rename(vector<string> v)
{
    if(v.size() != 3)
    {
        // cout << "too few arguements" << endl;
        return;
    }
    else
    {
        string path1 = v[1];
        string path2 = v[2];
        string dirPath1 = getCompletePathForDirectory(path1);
        string dirPath2 = getCompletePathForDirectory(path2);
        int value;
        value = rename(dirPath1.c_str(), dirPath2.c_str());
  
        // Print the result
        if(!value)
        {
            // cout << "File name changed successfully" << endl;
        }
        else
        {
            perror("Error");
        }
    }
}

void goto_directory(vector<string> v, int w)
{
    if(v.size() != 2)
    {
        // cout << "too few args" << endl;
        return;
    }
    else
    {
        string path = v[1];
        string dirPath = getCompletePathForDirectory(path);
        cout << dirPath << endl;
        right_stack.push(dirPath);
        switch_normal_command_mode = INT_MAX;
        listDirectory(dirPath.c_str(), w);
    }
}

void delete_file(vector<string> v)
{
    if(v.size() != 2)
    {
        // cout << "too few args" << endl;
        return;
    }
    else
    {
        string filePath = v[1];
        string completefilePath = getCompletePathForDirectory(filePath);
        cout << completefilePath << endl;
        if (unlink(completefilePath.c_str()) == 0)
        {
            // cout << "Delete Successful" << endl;
        }
        else
        {
            // cout << "Unsuccessful!!" << endl;
        }
    }
    
}

int isFileOrDirectory(string path)
{
    struct stat s;
    if(stat(path.c_str(), &s) == 0)
    {
        /* Directory */
        if(s.st_mode & S_IFDIR)
        {
            return 1;
        }
        else if(s.st_mode & S_IFREG)
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

void delete_directory_helper(string completeDirPath)
{
    DIR *dir;
    dir = opendir(completeDirPath.c_str());
    if(!dir)
    {
        // cout << "Directory not found" << endl;
        return;
    }
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
        {
            continue;
        }
        string newDirPath = completeDirPath + '/' + entry->d_name;
        /* directory */
        if(isFileOrDirectory(newDirPath) == 1)
        {
            delete_directory_helper(newDirPath);
        }
        /* file */
        else if(isFileOrDirectory(newDirPath) == 2)
        {
            unlink(newDirPath.c_str());
        }
    }
    rmdir(completeDirPath.c_str());
    closedir(dir);   
}

void delete_directory(vector<string> v)
{
    if(v.size() != 2)
    {
        // cout << "too few args" << endl;
        return;
    }
    else
    {
        string dirPath = v[1];
        string dirFullPath = getCompletePathForDirectory(dirPath);
        cout << dirFullPath << endl;
        delete_directory_helper(dirFullPath);
    }
}

void search_file_directory(vector<string> v, string currPath)
{
    if(v.size() != 2)
    {
        // cout << "Incorrect no. of arguements" << endl;
        return;
    }
    
    DIR *dir;
    string currFullPath = getCompletePathForDirectory(currPath);
    dir = opendir(currFullPath.c_str());
    if(!dir)
    {
        // cout << "Directory not found" << endl;
        return;
    }

    struct dirent *entry;
    while((entry = readdir(dir))!=NULL)
    { 
        if(!strcmp(entry->d_name,"..")||!strcmp(entry->d_name,"."))
        {
            continue;
        }
        string newFileOrDirName = entry->d_name; 
        string newFileOrDirPath = getAbsolutePathwithFileName(currFullPath, newFileOrDirName);
        int r = strcmp(currFullPath.c_str(), newFileOrDirPath.c_str());
        if(r == 0)
        {
            search_result.insert(newFileOrDirPath);
        }
        if(isFileOrDirectory(newFileOrDirPath) == 1)
        {
            search_file_directory(v, newFileOrDirPath);
        }
    }    
}

void copy_file(string fullSourcePath, string fullDestPath)
{
    vector<string> tokens = split(fullSourcePath, '/');
    string newFullDestPath = fullDestPath + '/' + tokens.back();

    std::ifstream src(fullSourcePath, std::ios::binary);
    std::ofstream dest(newFullDestPath, std::ios::binary);
    dest << src.rdbuf();
    // return src && dest;
}

void copy_directory(string fullSourcePath, string fullDestPath)
{
    vector<string> tokens = split(fullSourcePath, '/');
    string newFullDestPath = fullDestPath + '/' + tokens.back();
    if(mkdir(newFullDestPath.c_str(), 0777) == -1)
    {
        // cout << "Directory could not be created" << endl;
        return;
    }
    DIR *dir;
    dir = opendir(fullSourcePath.c_str());
    if(dir == NULL)
    {
        // cout << "error in opening directory" << endl;
        return;
    }
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        if((entry->d_name == ".") || (entry->d_name == ".."))
        {
            continue;
        }
        string newSrcFullPath = fullSourcePath + '/' + entry->d_name;
        /* directory */
        if(isFileOrDirectory(newSrcFullPath) == 1)
        {
            copy_directory(newSrcFullPath, newFullDestPath);
        }
        else
        {
            copy_file(newSrcFullPath, newFullDestPath);
        }
    }
}

void copy(vector<string> v)
{
    if(v.size() < 3)
    {
        // cout << "too few arguements" << endl;
        return;
    }
    string destPath = v.back();
    string fullDestPath = getCompletePathForDirectory(destPath);
    cout << fullDestPath << endl;
    /* checking directory or not */
    if(isFileOrDirectory(fullDestPath) == 1)
    {
        // cout << "directory" << endl;
        for(int i=1; i<v.size()-1; i++)
        {
            string sourcePath = v[i];
            cout << sourcePath << endl;
            string fullSourcePath = getCompletePathForDirectory(sourcePath);
            cout << fullSourcePath << endl;
            /* if source is a file */
            if(isFileOrDirectory(fullSourcePath) == 2)
            {
                copy_file(fullSourcePath, fullDestPath);
            }
            /* if source path is a directory */
            else if(isFileOrDirectory(fullSourcePath) == 1)
            {
                copy_directory(fullSourcePath, fullDestPath);
            }
            else
            {
                // do nothing
            }
        }
    }
    else
    {
        // do nothing
    }
}

void move(vector<string> v)
{
    if(v.size() < 3)
    {
        // cout << "too few args" << endl;
        return;
    }
    copy(v);
    for(int i=1; i<v.size()-1; i++)
    {
        string src = v[i];
        string srcFullPath = getCompletePathForDirectory(src);
        /* directory, so remove it */
        if(isFileOrDirectory(srcFullPath) == 1)
        {
            delete_directory_helper(srcFullPath);
        }
        /* file, remove it */
        else if(isFileOrDirectory(srcFullPath) == 2)
        {
            unlink(srcFullPath.c_str());
        }
    }
}

void commandMode(vector<string> v, int w, string msg)
{
    int effective_window_size;
    int len = v.size();
    if(w < v.size())
    {
        effective_window_size = w;
    }
    else
    {
        effective_window_size = len+4;
    }
    /* placing the cursor at y=0 */
    int cursor_pos_y = 0;
    set_cursor_pos(effective_window_size, cursor_pos_y);
    clearfrom;
    cout << "Command Mode: ";
    cout << msg;  
    cursor_pos_y = 15 + msg.length();
    set_cursor_pos(effective_window_size, cursor_pos_y);
   
    vector<string> tokens;
    char c;
    vector<string> kll;
    string command = msg;
    string absolute_command;

    while(true)
    {
        enableRawMode();
        switch((c=getchar()))
        {   
                case ESC: 
                        set_cursor_pos(effective_window_size+4, 0);
                        clearfrom;
                        switch_normal_command_mode = INT_MIN;
                        disableRawMode();
                        enable_cursor_movement(v, w);
                        break;
                case BACK:
                        if(cursor_pos_y <= 15)
                        {
                            // don't allow to move;
                        }
                        else
                        {
                            if(command.size() > 0)
                            {
                                command.pop_back();
                                cursor_pos_y = cursor_pos_y - 1;
                                set_cursor_pos(effective_window_size, cursor_pos_y);
                                eraseCharacter;
                            }
                        }
                        break;
                case QUIT_big:
                        set_cursor_pos(effective_window_size+4, 0);
                        /* clear the word NORMAL MODE */
                        clearfrom;
                        disableRawMode();
                        exit(0);
                case QUIT_small:
                        set_cursor_pos(effective_window_size+4, 0);
                        /* clear the word NORMAL MODE */
                        clearfrom;
                        disableRawMode();
                        exit(0);
                case ENTER: 
                        // y=0;
                        // setCommand(n,&y);
                        tokens = split(command, ' ');
                        absolute_command = tokens[0];
                        if(absolute_command.compare("copy")==0)
                        {
                            // cout << "copy" << endl;
                            copy(tokens);
                            commandMode(v, w, "");
                        }  
                        else if(absolute_command.compare("move")==0)
                        {
                            // cout << "move" << endl;
                            move(tokens);
                            commandMode(v, w, "");
                        }   
                        else if(absolute_command.compare("rename")==0)
                        {
                            // cout << "rename" << endl;
                            rename(tokens);
                            commandMode(v, w, "");
                        }
                        else if(absolute_command.compare("create_file")==0)
                        {
                            // cout << "create file" << endl;
                            create_file(tokens);
                            commandMode(v, w, "");
                        }   
                        else if(absolute_command.compare("create_dir")==0)
                        {
                            // cout << "create directory" << endl;
                            create_directory(tokens);
                            commandMode(v, w, "");
                        }
                        else if(absolute_command.compare("delete_file")==0)
                        {
                            // cout << "delete file" << endl;
                            delete_file(tokens);
                            commandMode(v, w, "");
                        }
                            
                        else if(absolute_command.compare("delete_dir")==0)
                        {
                            // cout << "delete dirctory" << endl;
                            delete_directory(tokens);
                            commandMode(v, w, "");
                        }
                        else if(absolute_command.compare("goto")==0)
                        {
                            // cout << "goto" << endl;
                            goto_directory(tokens, w);
                            commandMode(v, w, "");
                        }
                        else if(absolute_command.compare("search")==0)
                        {
                            // cout << "search" << endl;
                            search_file_directory(tokens, current_directory);
                            if(search_result.size() == 0)
                            {
                                // cout << "true" << endl;
                                msg = "true";
                            }
                            else
                            {
                                // cout << "false" << endl;
                                msg = "false";
                            }
                            commandMode(v, w, msg);
                        }
                        else
                        {
                            // cout << "Invalid Commnad" << endl;
                            commandMode(v, w, "Invalid Commnad");
                        }
                        command.clear();
                        break;
                default: 
                        cout << c; 
                        command.push_back(c);
                        cursor_pos_y++;
                        set_cursor_pos(effective_window_size, cursor_pos_y); 
                        break;
        }
    }
}

void enable_cursor_movement(vector<string>v, int w) {
    int c, effective_window_size;
    int k=-1, l=-1;
    int cursor_pos=0;
    if(w < v.size()) {
        effective_window_size = w;
    } else {
        effective_window_size = v.size();
    }
    set_cursor_pos(effective_window_size+4, 0);
    cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<NORMAL MODE>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    set_cursor_pos(0,0);
    
    while(true) {
        enableRawMode();
     	int c = getchar();
		if(c == ESC)
		{
			char brac = getchar();
			if(brac=='[') {
				switch (getchar()) {
                    /* UP arrow key */
            		case 'A':
                        if(cursor_pos == 0) {
                            // do nothing
                        } else {
                            cursor_up;
                            // printf("\033[1A");
                            cursor_pos--;
                        }
                		break;
                    /* DOWN arrow key */
                    case 'B':
						if(cursor_pos == effective_window_size-1) {
                            // do nothing
                        } else {
                            cursor_down;
                            // printf("\033[1B");
                            cursor_pos++;
                        }
                		break;
                    /* LEFT arroew key */
                    case 'C':
                        if(left_stack.size() == 0) {
                            // do nothing
                        } else {
                            right_stack.push(current_directory);
                            string str = left_stack.top();
                            left_stack.pop();
                            listDirectory(str.c_str(),w);
                        }
                		break;
            		case 'D':
                        if(right_stack.size() == 0) {
                            // do nothing
                        } else {
                            left_stack.push(current_directory);
                            string str = right_stack.top();
                            right_stack.pop();
                            listDirectory(str.c_str(),w);
                        }
                		break;
                }
            }
        } else if(c == ENTER) { /* handling the ENTER key */
            cursor_pos += (k>0)?k:0;
            if(cursor_pos == 0) {
                // do nothing
                // . case
                // remain where you are
            }
            else if(cursor_pos!=0)
            {
                struct stat inode;
                stat((v[cursor_pos]).c_str(),&inode);
                /* if it is a directory, go inside the directory and show its contents */
                if(S_ISDIR(inode.st_mode))
                {
                    while(!left_stack.empty()) {
                        left_stack.pop();
                    }
                    right_stack.push(current_directory);
                    /* .. case, so we have to go to its parent directory */
                    if(cursor_pos == 1) {
                        /* /home/sudipta/OS/. */
                        string topmost_directory = v[0];
                        vector<string> vv = split(topmost_directory, '/');
                        string parent_directory = "/";
                        for(int i=0; i<vv.size()-2; i++) {
                            parent_directory = parent_directory + vv[i] + '/';
                        }
                        cout << parent_directory << endl;
                        listDirectory(parent_directory.c_str(), w);
                        // displayParent_dir(v[0],w);
                    }     
                    else {
                        listDirectory((v[cursor_pos]).c_str(),w);
                    }
                }
                /* if its a file, open using its default application */
                else 
                {
                    openFile((v[cursor_pos]).c_str());
                }
            }
        } else if(c == 107) { // k
            if(k==-1 && l==-1 && w+1<v.size()){  
                k=1;
                l=w+1;
            }
            if(l<v.size())
            {
                k++;l++;
                printf("\33c");
                for(int i=k;i<l;i++)
                {
                    display_file_contents(v[i]);
                }
                set_cursor_pos(effective_window_size+4,0);
                printf("*******************************NORMAL MODE*************************");    
                set_cursor_pos(w,0);
                cursor_pos=w;    
            }
        } else if(c == 108) { // l
            if (k>0 && l<=v.size()) {
                k--;l--;
                printf("\33c");
                for(int i=k;i<l;i++)
                {
                     display_file_contents(v[i]);
                }
                set_cursor_pos(effective_window_size+4,0);
                printf("*******************************NORMAL MODE*************************");      
                set_cursor_pos(0,0); 
                cursor_pos=0;
            }
        } else if(c == BACK) { /* handling the BACKSPACE key */
            right_stack.push(current_directory);
            /* clearing the left stack history */
            while(!left_stack.empty()) {
                left_stack.pop();
            }
            string parent_directory = v[1];
            listDirectory(parent_directory.c_str(),w);  
            
        } else if(c == HOME) { /* handling the HOME key */
            right_stack.push(current_directory);
            /* clearing the left stack history */
            while(!left_stack.empty()) {
                left_stack.pop();
            }
            listDirectory(home, w);  
            break;  
        } else if(c == QUIT_small || c == QUIT_big) {
            /* move the cursor to the position where we wrote NORMAL mode */
            set_cursor_pos(effective_window_size+4, 0);
            /* clear the word NORMAL MODE */
            eraseline;
            // disableRawMode();
            exit(0);
        } else if(c == COLON) { /* handling COLON case, go to command mode */
            disableRawMode();
            /* go to command mode */
            commandMode(v, w, "");
        }     
    }   	
}

void listDirectory(const char *dirname, int w)
{
    DIR *dir;
    
    /* open the directory */
    dir = opendir(dirname);
    if(dir==NULL) {
        cout << "The directory could not be opened due to some error" << endl;
        exit(1);
    }

    /* Loop through each file in the directory and adding in the vector */
    struct dirent *entry;
    vector<string> v;
    string ss2 = dirname;

    while ((entry = readdir(dir)) != NULL) {
        string ss = entry->d_name;
        ss = ss2 + "/" + ss;
        v.push_back(ss);
    }
    closedir(dir); 
    
    //sort the files in alphabetical order
    sort(v.begin(),v.end());
   
    /* clear the screen before printing File Details */
    clearScreen;

    for(int i=0; i<v.size(); i++) {
        display_file_contents(v[i]);
        cout << endl;
    }

    string temp = v[0];
    int len = temp.length();
    current_directory = temp.substr(0, len-2);

    if(switch_normal_command_mode == INT_MAX)
    {
        commandMode(v, w, "");
    }
    else
    {
        enable_cursor_movement(v, w);
    }

    // cout << "Current Directory: " << current_directory << endl;
    // return v;
}

void start_normal_mode(string str) {
    cout << str << endl;
    char* path;
    string str_obj(str);
    path = &str_obj[0];
    // list_dir(path);

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    // printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);

    int window_row_size = w.ws_row;
    int window_cols_size = w.ws_col;

    listDirectory(path, w.ws_row-4);

}

int main() {
    char tmp[256];
    getcwd(tmp, 256);
    // cout << "Current working directory: " << tmp << endl;
    stack<string> s;
    s.push(tmp);
    // cout << s.top() << endl;
    struct passwd *pw = getpwuid(getuid());
    // home = pw->pw_dir;
    getcwd(home, 256);
    start_normal_mode(s.top());
    return 0;
}