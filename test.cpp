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
struct winsize w;
string current_directory;

int getch(void)
{
 int ch;
 struct termios oldtio;
 struct termios newtio;
 tcgetattr(0, &oldtio); //0 is file discriptor for standard input
 
 newtio = oldtio; 
 newtio.c_lflag &= ~(ICANON | ECHO);   //switch to noncanonical and nonecho mode 
 tcsetattr(0, TCSANOW, &newtio);

 ch=cin.get();                         //Take the input
                   
 if(tcsetattr(0, TCSANOW, &oldtio))    //switch to cananical mode
    {
        fprintf(stderr,"Set attributes failed");
        exit(1);
    }
 
    return ch; 
}

int main() {
	int c;
	c =0;
	c = getch();
	cout << c << endl;

	return 0;
}
