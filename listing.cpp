# include <iostream>
# include <dirent.h>
using namespace std;

void directory_contents ( char * directory_path )
{  
  DIR *dh;
  struct dirent * contents; 
  
  dh = opendir ( directory_path );
  
  if ( !dh )
  {
    cout << "The given directory is not found";
    return;
  }

  while ( ( contents = readdir ( dh ) ) != NULL )
  {
    string name = contents->d_name;
    cout << name << endl;
  }

  closedir ( dh );
}

int main ()
{
  directory_contents ( (char*) "/home/sudipta/Desktop/OS" );
  return 0;
}
