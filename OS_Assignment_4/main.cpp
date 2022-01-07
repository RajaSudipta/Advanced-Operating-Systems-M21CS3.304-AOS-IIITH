#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;

/* Block Size 4KB */
#define BLOCK_SIZE 4096 
/* Disk Size 500 MB, number of disk blocks = 500MB/4KB =  128000 */
#define DISK_BLOCKS 131072
/* 60% of DISK_BLOCKS are inodes */
#define NO_OF_INODES 78644   
#define MOD 1000000007
#define NO_OF_FILE_DESCRIPTORS 32
char disk_name[100];
FILE *diskptr;
static bool isDiskOpen = false;

const string PATH = "/home/sudipta/Desktop/fileIo/";
vector<string> file_list;
vector<pair<string, char>> file_open_desc(32);
int memry = 0;


struct inode 
{
    int file_size;
    // int index;
    /* 10 direct pointers, 1 single indirect, 1 double indirect pointer */
    int file_pointer[12]; 
    // bool flag;
};
struct inode inode_arr[NO_OF_INODES];

struct index_map
{
    int indexing_size;
    bool schema_flag;
};

struct file_to_inode_mapping 
{
    char file_name[30]; 
    // bool flag;
    int inode_no;   
};
struct file_to_inode_mapping file_inode_mapping_arr[NO_OF_INODES];

struct super_block
{
    bool inode_freelist[NO_OF_INODES];   

    int no_of_blocks_used_by_superblock = ceil(((float)sizeof(super_block)) / BLOCK_SIZE);

    bool datablock_freelist[DISK_BLOCKS];

    int no_of_blocks_used_by_file_inode_mapping = ceil(((float)sizeof(struct file_to_inode_mapping) * NO_OF_INODES) / BLOCK_SIZE);

    int starting_index_of_inodes = (no_of_blocks_used_by_superblock + no_of_blocks_used_by_file_inode_mapping);

    int no_of_blocks_used_to_store_inodes = ceil(((float)(NO_OF_INODES * sizeof(struct inode))) / BLOCK_SIZE);

    int starting_index_of_data_blocks = ((no_of_blocks_used_to_store_inodes + no_of_blocks_used_by_superblock) + no_of_blocks_used_by_file_inode_mapping);

    int total_no_of_available_blocks = (DISK_BLOCKS - starting_index_of_data_blocks);
    
};
struct super_block sb;

vector<int> data_block_vector_free;  
stack<string> sta1;  
map<string, int> file_to_inode_map;  
map<string, string> dmaping;
map<int, string> inode_to_file_map;  
unordered_map<int, int> umdamping;
vector<int> inode_vector_free;   
map<int, int> file_descriptor_mode_map;       
vector<bool> filevapp;
vector<int> filedescriptor_vector_free;      
stack<int> sta2;  
map<int, pair<int, int>> file_descriptor_map; 
unordered_map<int, int> umdamp;
int openfile_count = 0;                       


bool create_disk(char *disk_name)
{
    char buffer[BLOCK_SIZE];
    memset(buffer, 0, BLOCK_SIZE);
    int actual_size = BLOCK_SIZE % MOD;
    bool flag = true;
    if(access( disk_name, F_OK ) != -1)
    {
        cout << "Virtual Disk already exists !!!" << endl;
        flag = false;
        return false;
    }

    diskptr = fopen(disk_name, "wb");
    
    /* Using buffer to initialize whole disk as NULL  */
    for (int i = 0; i < DISK_BLOCKS; i++)
    {
        fwrite(buffer, 1, BLOCK_SIZE, diskptr);
    }
    
    stack<int> fcw;
    struct super_block sb;

    int i = 0;
    while(i < sb.starting_index_of_data_blocks)
    {
        sb.datablock_freelist[i] = true;
        i++;
    }

    i = sb.starting_index_of_data_blocks;
    while(i < DISK_BLOCKS)
    {
        sb.datablock_freelist[i] = false; 
        i++;
    }

    i=0;
    for (; i < NO_OF_INODES; i++)
    {
        sb.inode_freelist[i] = false;
    }

    i=0;
    for (; i < NO_OF_INODES; i++)
    {
        int jj = 0;
        while(jj < 12)
        {
            int num = -1;
            inode_arr[i].file_pointer[jj] = num;
            jj++;
        }
    }

    int len = sizeof(struct super_block);
    // cout << "len: " << len << endl;
    char superblock_buffer[len];
    int kl[10];
    memset(superblock_buffer, 0, len);
    memset(kl, 0, 10);
    memcpy(superblock_buffer, &sb, sizeof(sb));
    fseek(diskptr, 0, SEEK_SET);
    fwrite(superblock_buffer, sizeof(char), sizeof(sb), diskptr);

    len = sizeof(file_inode_mapping_arr);
    // cout << "len: " << len << endl;
    char directory_buffer[len];
    int posk[10];
    memset(directory_buffer, 0, len);
    memset(posk, 0, 10);
    memcpy(directory_buffer, file_inode_mapping_arr, len);
    fseek(diskptr, (sb.no_of_blocks_used_by_superblock) * BLOCK_SIZE, SEEK_SET);
    sta2.push(sb.no_of_blocks_used_by_superblock);
    fwrite(directory_buffer, sizeof(char), len, diskptr);

    len = sizeof(inode_arr);
    // cout << "len: " << len << endl;
    char inode_buffer[len];
    int post[10];
    memset(inode_buffer, 0, len);
    memset(post, 0, 10);
    memcpy(inode_buffer, inode_arr, len);
    fseek(diskptr, (sb.starting_index_of_inodes) * BLOCK_SIZE, SEEK_SET);
    sta2.push(sb.starting_index_of_inodes);
    fwrite(inode_buffer, sizeof(char), len, diskptr);

    fclose(diskptr);
    cout << "Virtual Disk Created Successfully!!!"  << endl;

    return true;
}

bool mount_disk(char *disk_name)
{
    diskptr = fopen(disk_name, "rb+");
    int super_block_size = sizeof(sb);
    char superblock_buffer[super_block_size];
    if (diskptr == NULL)
    {
        cout << "Disk does not exist" << endl;
        return false;
    }
    
    char posx[10];
    memset(superblock_buffer, 0, super_block_size);
    memset(posx, 0, 10);
    fread(superblock_buffer, sizeof(char), sizeof(sb), diskptr);
    memset(posx, 1, 10);
    memcpy(&sb, superblock_buffer, super_block_size);

    int tmpp = sb.no_of_blocks_used_by_superblock;
    int fsek_temp = BLOCK_SIZE * tmpp;
    fseek(diskptr, fsek_temp, SEEK_SET);
    int len = sizeof(file_inode_mapping_arr);
    char directory_buffer[len];
    int fdc[15];
    memset(directory_buffer, 0, len);
    memset(fdc, 0, 15);
    fread(directory_buffer, sizeof(char), len, diskptr);
    memset(fdc, 1, 15);
    memcpy(file_inode_mapping_arr, directory_buffer, len);

    int tmpp2 = sb.starting_index_of_inodes;
    int fsek_temp2 = BLOCK_SIZE * tmpp2;
    fseek(diskptr, fsek_temp2, SEEK_SET);
    len = sizeof(inode_arr);
    int fdd[15];
    char inode_buff[len];
    memset(fdd, 0, 15);
    memset(inode_buff, 0, len);
    fread(inode_buff, sizeof(char), len, diskptr);
    memset(fdc, 1, 15);
    memcpy(inode_arr, inode_buff, len);

    int i = NO_OF_INODES-1;
    for (; i >= 0; i--)
    {
        if(sb.inode_freelist[i] == false)
        {
            inode_vector_free.emplace_back(i);
        }
        else
        {
            string temp = (string)(file_inode_mapping_arr[i].file_name);
            sta1.push(temp);
            int inno = file_inode_mapping_arr[i].inode_no;
            file_to_inode_map[temp] = inno;
            inno = file_inode_mapping_arr[i].inode_no;
            temp = (string)(file_inode_mapping_arr[i].file_name);
            sta2.push(inno);
            inode_to_file_map[inno] = string(temp);
        }
    }

    i = DISK_BLOCKS-1;
    int endpoint = sb.starting_index_of_data_blocks;
    while(i >= sb.starting_index_of_data_blocks)
    {
        if(sb.datablock_freelist[i] == true)
        {
            sta2.push(i+1);
        }
        if (sb.datablock_freelist[i] == false)
        {
            sta1.push(to_string(i));
            data_block_vector_free.push_back(i);
        } 
        i--;
    }
    
    i = NO_OF_FILE_DESCRIPTORS - 1;

    while(i >= 0)
    {
        filevapp.push_back(true);
        filedescriptor_vector_free.push_back(i);
        i--;
    }
    isDiskOpen = true;
    cout << "Disk is Mounted Successfully!!!" << endl;
    return true;
}

bool unmount_disk()
{
    if(isDiskOpen == false)
    {
        cout << "No disk is currently opened!!" << endl;
        return false;
    }

    long long int i = 0;
    while(i < data_block_vector_free.size())
    {
        long long int temp = data_block_vector_free[i];
        sta2.push(temp);
        sb.datablock_freelist[temp] = false;
        i++;
    }

    i = DISK_BLOCKS - 1;
    for (; i >= sb.starting_index_of_data_blocks; i--)
    {
        sta2.push(i);
        sb.datablock_freelist[i] = true;
    }
    
    data_block_vector_free.clear();

    i=0;
    while (i < NO_OF_INODES)
    {
        sta2.push(i);
        sb.inode_freelist[i] = true;
        i++;
    }

    i=0;
    while (i < inode_vector_free.size())
    {
        sta1.push(to_string(i));
        int temp = inode_vector_free[i];
        sb.inode_freelist[temp] = false;
        i++;
    }

    int len = sizeof(super_block);
    char superblock_buffer[len];
    char posc[10];
    memset(superblock_buffer, 0, len);
    memset(posc, 0, 10);
    memcpy(superblock_buffer, &sb, sizeof(sb));
    fseek(diskptr, 0, SEEK_SET);
    fwrite(superblock_buffer, sizeof(char), sizeof(sb), diskptr);
    
    len = sizeof(file_inode_mapping_arr);
    char directory_buffer[len];
    char posd[15];
    memset(directory_buffer, 0, len);
    memset(posd, 0, 15);
    memcpy(directory_buffer, file_inode_mapping_arr, len);
    int tmpp = sb.no_of_blocks_used_by_superblock;
    int tmpp2 = BLOCK_SIZE * tmpp;
    fseek(diskptr, tmpp2, SEEK_SET);
    fwrite(directory_buffer, sizeof(char), len, diskptr);

    len = sizeof(inode_arr);
    char inode_buffer[len];
    char pose[10];
    memset(inode_buffer, 0, len);
    memset(pose, 0, 10);
    memcpy(inode_buffer, inode_arr, len);
    tmpp = sb.starting_index_of_inodes;
    tmpp2 = BLOCK_SIZE * tmpp;
    fseek(diskptr, tmpp2, SEEK_SET);
    fwrite(inode_buffer, sizeof(char), len, diskptr);

    isDiskOpen = false;

    cout << "Disk Unmounted successfully !!" << endl;

    return true;
}


bool _create_file_by_fileIO()
{
    string file_name;
    cout << "Enter File Name: ";
    cin >> file_name;
    string effective_file_path = PATH + file_name;
    // cout << effective_file_path << endl;
    fstream file;
    file.open(effective_file_path.c_str(), ios::out);
    if(!file)
    {
        cout<<"Error in creating file!!!";
        return false;
    }
    file_list.push_back(file_name);
    cout<<"File created successfully.";
    file.close();
    return true;
}

bool fileExists(string fileName)
{
    if (file_to_inode_map.find(fileName) == file_to_inode_map.end())
    {
        return false;
    }   
    else
    {
        return true;
    }
}

bool create_file()
{
    string fileName;
    cout << "Enter File Name: ";
    cin >> fileName;
    bool flag;

    if(fileExists(fileName) == true)
    {
        cout << "File already present in DISK !!!" << endl;
        flag = false;
        return false;
    }
    else
    {
        int jk = inode_vector_free.size();
        // cout << "size: " << jk << endl;
        jk += 1;
        jk -= 1;
        // cout << "size: " << jk << endl;
        if (jk == 0)
        {
            cout << "Inodes unavialable" << endl;
            flag = false;
            return false;
        }

        jk = data_block_vector_free.size();
        // cout << "size: " << jk << endl;
        jk += 1;
        jk -= 1;
        // cout << "size: " << jk << endl;
        if (jk == 0)
        {
            cout << "Datablocks unavailable" << endl;
            flag = false;
            return false;
        }

        int next_inode = inode_vector_free[inode_vector_free.size() - 1];
        int next_datablock = data_block_vector_free[data_block_vector_free.size() - 1];
        int high_index = next_inode + 1;
        // cout << next_inode << " " << next_datablock << endl;
        inode_vector_free.pop_back();
        int low_index = next_datablock - 1;
        data_block_vector_free.pop_back();
        
        inode_arr[next_inode].file_pointer[0] = next_datablock;
        inode_arr[next_inode].file_pointer[0] = low_index + 1;
        inode_arr[next_datablock].file_size = 0;


        file_to_inode_map[fileName] = next_inode;
        file_to_inode_map[fileName] = high_index - 1;
        inode_to_file_map[next_inode] = fileName;
        low_index = next_inode;

        file_inode_mapping_arr[next_inode].inode_no = high_index-1;
        file_inode_mapping_arr[next_inode].inode_no = next_inode;
        strcpy(file_inode_mapping_arr[next_inode].file_name, fileName.c_str());

        cout << "DONE!!" << endl;
        cout << "File Created Successfully !!" << endl;
        flag = true;
        return true;
    }

}

int check_vec_size(vector<int> filedescriptor_vector_free)
{
    if(filedescriptor_vector_free.size() == 0)
    {
        return 0;
    }
    else
    {
        return filedescriptor_vector_free.size();
    }
}

bool fdes_map_find(map<int, std::pair<int, int>> file_descriptor_map, int index)
{
    if (file_descriptor_map.find(index) == file_descriptor_map.end())
    {
        return false;
    }
    return true;
}

bool open_file()
{
    string fileName;
    cout << "Enter File Name to open: ";
    cin >> fileName;
    bool flag;

    if (fileExists(fileName) == false)
    {
        cout << "File Not found in the DISK" << endl;
        flag = false;
        return false;
    }
    else
    {
        if (check_vec_size(filedescriptor_vector_free) == 0)
        {
            cout << "File descriptor not available !!!" << endl;
            flag = false;
            return false;
        }

        cout << "0 : READ MODE" << endl;
        cout << "1 : WRITE MODE" << endl;
        cout << "2 : APPEND MODE" << endl;
        int decide_file_mode = -1;
        cin >> decide_file_mode;
        if(!(decide_file_mode == 0 || decide_file_mode == 1 || decide_file_mode == 2))
        {
            cout << "Choose Valid option !!" << endl;
        }
        else
        {
            int curr_inode = -1;
            curr_inode = file_to_inode_map[fileName];
            if(decide_file_mode == 1)
            {
                int start_index = 0;
                int end_index = NO_OF_FILE_DESCRIPTORS;
                int i = start_index;
                for (; i < NO_OF_FILE_DESCRIPTORS; i++)
                {
                    int index = i;
                    if (fdes_map_find(file_descriptor_map, index) == true && file_descriptor_map[i].first == curr_inode)
                    {
                        if(file_descriptor_mode_map[i] == 1)
                        {
                            int fdes = i;
                            cout << "File is already in use with file descriptor : " << fdes << endl;
                            flag = false;
                            return flag;
                        }
                        
                    }
                }
            }
            if(decide_file_mode == 2)
            {
                int start_index = 0;
                int end_index = NO_OF_FILE_DESCRIPTORS;
                int i = start_index;
                for (; i < NO_OF_FILE_DESCRIPTORS; i++)
                {
                    int index = i;
                    if (fdes_map_find(file_descriptor_map, index) == true && file_descriptor_map[i].first == curr_inode)
                    {
                        if(file_descriptor_mode_map[i] == 2)
                        {
                            int fdes = i;
                            cout << "File is already in use with file descriptor : " << fdes << endl;
                            flag = false;
                            return flag;
                        }
                        
                    }
                }
            }   
            
            int last_index = filedescriptor_vector_free.size()-1;
            int fdes = filedescriptor_vector_free[last_index];
            stack<int> sta1;
            sta1.push(fdes);
            filedescriptor_vector_free.pop_back();

            int tmp = 0;
            file_descriptor_map[fdes].second = tmp;
            file_descriptor_map[fdes].first = curr_inode;
            sta1.push(decide_file_mode);
            file_descriptor_mode_map[fdes] = decide_file_mode;
            memry = openfile_count;
            openfile_count += 1;

            cout << "DONE!!" << endl;
            cout << fileName << " opened with file descriptor  : " << fdes << endl;
            
            flag = true;
            return flag;
        }
    
    }
    return true;

}


bool _open_file_by_fileIO()
{
    string file_name;
    cout << "Enter File Name: ";
    cin >> file_name;
    if(find(file_list.begin(), file_list.end(), file_name) == file_list.end())
    {
        cout << "ERROR!! File not found";
        return false;
    }

    cout << "0 : Read Mode" << endl;
    cout << "1 : Write Mode" << endl;
    cout << "2 : Append Mode" << endl;

    int mode;
    cin >> mode;

    if(mode == 0)
    {
        char ch = 'r';
        int pos = -1;
        for(int i=0; i<31; i++)
        {
            if(file_open_desc[i].first == "" && file_open_desc[i].second == '\0')
            {
                file_open_desc[i].first = file_name;
                file_open_desc[i].second = ch;
                pos = i;
                break;
            }
        }
        if(pos == -1)
        {
            cout << "File Descriptor full!!" << endl;
            return false;
        }
        else
        {
            cout << "File " << file_name << " opened in read mode with file descriptor : " << pos << endl;
        }
    }
    else if(mode == 1)
    {
        char ch = 'w';
        int pos = -1;
        for(int i=0; i<31; i++)
        {
            if(file_open_desc[i].first == "" && file_open_desc[i].second == '\0')
            {
                file_open_desc[i].first = file_name;
                file_open_desc[i].second = ch;
                pos = i;
                break;
            }
        }
        if(pos == -1)
        {
            cout << "File Descriptor full!!" << endl;
            return false;
        }
        else
        {
            cout << "File " << file_name << " opened in write mode with file descriptor : " << pos << endl;
        }
    }
    else if(mode == 2)
    {
        char ch = 'a';
        int pos = -1;
        for(int i=0; i<31; i++)
        {
            if(file_open_desc[i].first == "" && file_open_desc[i].second == '\0')
            {
                file_open_desc[i].first = file_name;
                file_open_desc[i].second = ch;
                pos = i;
                break;
            }
        }
        if(pos == -1)
        {
            cout << "File Descriptor full!!" << endl;
            return false;
        }
        else
        {
            cout << "File " << file_name << " opened in append mode with file descriptor : " << pos << endl;
        }
    }
    else
    {
        cout << "Choose correct mode" << endl;
        return false;
    }
    return true;
}

void _list_of_files_by_fileIO()
{
    for(auto it: file_list)
    {
        cout << it << endl;
    }
}

void _list_of_open_files_by_fileIO()
{
    /* huge.txt is opened with descriptor [ 0 ] in READ mode */
    for(int i=0; i<31; i++)
    {
        if(file_open_desc[i].first != "" && file_open_desc[i].second != '\0')
        {
            cout << file_open_desc[i].first << " is opened with file descriptor [" << i << "] in";
            if(file_open_desc[i].second == 'r')
            {
                cout << " READ mode" << endl;
            }
            else if(file_open_desc[i].second == 'w')
            {
                cout << " WRITE mode" << endl;
            }
            else if(file_open_desc[i].second == 'a')
            {
                cout << " APPEND mode" << endl;
            }
        }
    }
}

void _close_file_by_fileIO()
{
    // string file_name;
    // cout << "Enter File Name: ";
    // cin >> file_name;
    // if(find(file_list.begin(), file_list.end(), file_name) == file_list.end())
    // {
    //     cout << "ERROR!! File not found";
    //     return;
    // }
    int fdes;
    cout << "Enter File Descriptor to close: ";
    cin >> fdes;
    if(fdes<0 || fdes>31)
    {
        cout << "File Descriptor does not exist" << endl;
        return;
    }
    if(file_open_desc[fdes].first == "" && file_open_desc[fdes].second == '\0')
    {
        cout << "File not opened yet!!" << endl;
        return;
    }

    file_open_desc[fdes].first = "";
    file_open_desc[fdes].second = '\0';
    cout << "File descriptor closed successfully!!" << endl;
    // for(int i=0; i<31; i++)
    // {
    //     if(file_open_desc[i].first != "" && file_open_desc[i].second != '\0')
    //     {
    //         if(file_open_desc[i].first == file_name)
    //         {
    //             file_open_desc[i].first = "";
    //             file_open_desc[i].second = '\0';
    //         }
    //     }
    // }
}

void do_map_cleaning(int fdes)
{
    file_descriptor_mode_map.erase(fdes);
    memry = fdes+1;
    file_descriptor_map.erase(fdes);
    memry = openfile_count;
    openfile_count -= 1;
}

bool close_file_descriptor()
{
    int fdes;
    cout << "Enter the file descriptor: ";
    cin >> fdes;
    bool flag;
    if(fdes_map_find(file_descriptor_map, fdes) == false)
    {
        cout << "File is not opened yet !!!" << endl;
        flag = false;
        return flag;
    }

    do_map_cleaning(fdes);
    filedescriptor_vector_free.push_back(fdes);
    cout << "File descriptor closed successfully" << endl;
    flag = true;
    return flag;
}


void _delete_file_by_fileIO()
{
    string file_name;
    cout << "Enter File Name: ";
    cin >> file_name;
    if(find(file_list.begin(), file_list.end(), file_name) == file_list.end())
    {
        cout << "ERROR!! File not found";
        return;
    }
    for(int i=0; i<31; i++)
    {
        if(file_open_desc[i].first != "" && file_open_desc[i].second != '\0')
        {
            if(file_open_desc[i].first == file_name)
            {
                cout << "File is still still opened !! Close it before delete" << endl;
                return;
            }
        }
    }
    auto it = find(file_list.begin(), file_list.end(), file_name);
    file_list.erase(it);
    string file_effective_path = PATH + file_name;
    if (remove(file_effective_path.c_str()) == 0)
    {
        cout << file_name << " deleted successfully" << endl;
    }
    else
    {
        cout << " Unable to delete " << file_name << endl;
    }
    
}

bool file_search(string fileName)
{
    if (file_to_inode_map.find(fileName) == file_to_inode_map.end())
    {
        return false;
    }
    return true;
}

bool delete_file()
{
    cout << "Enter File Name: ";
    string fileName;
    cin >> fileName;
    bool flag;

    //check if file exist or not
    if (file_search(fileName) == false)
    {
        cout << "File doesn't exist !!!" << endl;
        return false;
    }


    int curr_inode = file_to_inode_map[fileName];
    int high_index = curr_inode + 1;
    int i = 0;
    int end_index = NO_OF_FILE_DESCRIPTORS;
    while(i < end_index)
    {
        int index = i;
        if (fdes_map_find(file_descriptor_map, index) == true)
        {
            if(file_descriptor_map[i].first == curr_inode)
            {
                cout << "File is opened, Can not delete an opened file !!!" << endl;
                flag = false;
                return flag;
            }
            
        }
        i++;
    }
    return true;

}

void _write_file_by_fileIO()
{
    int fdes;
    cout << "Enter File Descriptor: ";
    cin >> fdes;
    if(fdes < 0 || fdes > 31)
    {
        cout << "Wrong File Descriptor" << endl;
        return;
    }
    if(file_open_desc[fdes].first == "" && file_open_desc[fdes].second == '\0')
    {
        cout << "File not opened yet" << endl;
        return;
    }
    if(file_open_desc[fdes].second != 'w')
    {
        cout << "wrong mode selected!! It is for write mode." << endl;
        return;
    }

    string file_name = file_open_desc[fdes].first;
    string file_effective_path = PATH + file_name;
    // string content;
    // cout << "Enter content: ";
    // getline(cin, content);
    cout << "Enter content (write -1 in new line to stop) >> " << endl;
    ofstream fout;
 
    string line;
    fout.open(file_effective_path);
    cin.ignore();
    // Execute a loop If file successfully opened
    while (fout) 
    {
        // Read a Line from standard input
        getline(cin, line);
 
        // Press -1 to exit
        if (line == "-1")
        {
            break;
        }
 
        // Write line in file
        fout << line << endl;
    }
 
    // Close the File
    fout.close();

}

void _read_file_by_fileIO()
{
    int fdes;
    cout << "Enter File Descriptor: ";
    cin >> fdes;
    if(fdes < 0 || fdes > 31)
    {
        cout << "Wrong File Descriptor" << endl;
        return;
    }
    if(file_open_desc[fdes].first == "" && file_open_desc[fdes].second == '\0')
    {
        cout << "File not opened yet" << endl;
        return;
    }
    if(file_open_desc[fdes].second != 'r')
    {
        cout << "wrong mode selected!! It is for READ mode." << endl;
        return;
    }

    string file_name = file_open_desc[fdes].first;
    string file_effective_path = PATH + file_name;
    // string content;
    // cout << "Enter content: ";
    // getline(cin, content);
    cout << "The content of file is >> " << endl;
    fstream fin;
 
    string line;
    fin.open(file_effective_path, ios::in);
    cin.ignore();
    // Execute a loop If file successfully opened
    while (fin) 
    {
        getline(fin, line);
        cout << line << endl;
    }
 
    // Close the File
    fin.close();

}

void _append_file_by_fileIO()
{
    int fdes;
    cout << "Enter File Descriptor: ";
    cin >> fdes;
    if(fdes < 0 || fdes > 31)
    {
        cout << "Wrong File Descriptor" << endl;
        return;
    }
    if(file_open_desc[fdes].first == "" && file_open_desc[fdes].second == '\0')
    {
        cout << "File not opened yet" << endl;
        return;
    }
    if(file_open_desc[fdes].second != 'a')
    {
        cout << "wrong mode selected!! It is for append mode." << endl;
        return;
    }

    string file_name = file_open_desc[fdes].first;
    string file_effective_path = PATH + file_name;

    cout << "Enter content (write -1 in new line to stop) >> " << endl;
    ofstream outfile;

    outfile.open(file_effective_path, std::ios_base::app); // append instead of overwrite
    string line;
    cin.ignore();
    // Execute a loop If file successfully opened
    while (outfile) 
    {
        // Read a Line from standard input
        getline(cin, line);
 
        // Press -1 to exit
        if (line == "-1")
        {
            break;
        }
 
        // Write line in file
        outfile << line << endl;
    }
 
    // Close the File
    outfile.close();
    
    // outfile << "Data"; 
}

bool print_list_of_open_files()
{
    bool flag;
    cout << "The list of all opened files is " << endl;
    map<int, pair<int, int>>:: iterator it;
    for (it=file_descriptor_map.begin(); it!=file_descriptor_map.end(); it++)
    {
        int curr_inode = it->second.first;
        int mapping = it->second.second;
        int fdes = it->first;
        int file_mode = file_descriptor_mode_map[fdes];
        string fileName = inode_to_file_map[curr_inode];
        switch (file_mode)
        {
        case 0:
            cout << fileName << " is opened, Mode = READ mode, Descriptor = " << fdes << endl;
            break;
        case 1:
            cout << fileName << " is opened, Mode = WRITE mode, Descriptor = " << fdes << endl;
            break;
        case 2:
            cout << fileName << " is opened, Mode = APPEND mode, Descriptor = " << fdes << endl;
            break;
        default:
            break;
        }
    }
    flag = true;
    return flag;
}

bool print_list_of_files()
{
    bool flag;
    cout << "The list of all files is " << endl;
    map<string, int>:: iterator it;
    for(it = file_to_inode_map.begin(); it!=file_to_inode_map.end(); it++)
    {
        string fileName = it->first;
        int fileInode = it->second;
        cout << "File Name: " << fileName << ", Inode: " << fileInode << endl;
    }
    flag = true;
    return flag;
}

void mounting_menu()
{
    for(int i=0; i<31; i++)
    {
        file_open_desc[i].first = "";
        file_open_desc[i].second = '\0';
    }
    int option;
    int file_descriptor;
    while(true)
    {
        cout << endl;
        cout << "1 : Create File" << endl;
        cout << "2 : Open File " << endl;
        cout << "3 : Read File" << endl;
        cout << "4 : Write File " << endl;
        cout << "5 : Append File " << endl;
        cout << "6 : Close File " << endl;
        cout << "7 : Delete File " << endl;
        cout << "8 : List of Files " << endl;
        cout << "9 : List of opened Files " << endl;
        cout << "10 : Unmount Disk " << endl;

        cin >> option;
        switch (option)
        {
        case 1:
            create_file();
            break;
        case 2:
            open_file();
            break;
        case 3:
            // read_file();
            cout << "Not Implemented!!" << endl;
            break;
        case 4:
            cout << "Not Implemented!!" << endl;
            // write_file();
            break;
        case 5:
            cout << "Not Implemented!!" << endl;
            // append_file();
            break;
        case 6:
            close_file_descriptor();
            break;
        case 7:
            delete_file();
            break;
        case 8:
            print_list_of_files();
            break;
        case 9:
            print_list_of_open_files();
            break;
        case 10:
            unmount_disk();
            return;
            break;
        default:
            cout << "Please choose correct option!!" << endl;
            break;
        }
    }
}

int main()
{
    int option;
    bool flag;
    while(true)
    {
        cout << "1. Create Disk" << endl;
        cout << "2. Mount Disk" << endl;
        cout << "3. EXIT" << endl;

        cin >> option;

        if(option == 1)
        {
            cout << "Enter Disk Name: ";
            cin >> disk_name;
            create_disk(disk_name);
        }
        else if(option == 2)
        {
            cout << "Enter Disk Name: ";
            cin >> disk_name;
            bool open_res = mount_disk(disk_name);
            if(open_res == true)
            {
                mounting_menu();
            }
            else
            {
                // do nothing
            }
        }
        else if(option == 3)
        {
            break;
        }
        else
        {
            cout << "Invalid option" << endl;
        }
    }
}