/*

 g++ v3_merged_shell.cpp -o v3 && ./v3 2> debuglog.txt

*/

#include <string>
#include <bits/stdc++.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <ctime>
#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <algorithm>
#include <cctype>
#include <locale>

using namespace std;

#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#define MOD 1000000007
#define MOD1 998244353
#define INF 1e18
#define nline "\n"
#define pb push_back
#define ppb pop_back
#define mp make_pair
#define ff first
#define ss second
#define PI 3.141592653589793238462
#define set_bits __builtin_popcountll
#define sz(x) ((int)(x).size())
#define all(x) (x).begin(), (x).end()

typedef long long ll;
typedef unsigned long long ull;
typedef long double lld;
// typedef tree<pair<int, int>, null_type, less<pair<int, int>>, rb_tree_tag, tree_order_statistics_node_update > "pbds; // find_by_order, order_of_key

#ifndef ONLINE_JUDGE
#define debug(x)       \
    cerr << #x << " "; \
    _print(x);         \
    cerr << endl;
#else
#define debug(x)
#endif

void _print(ll t)
{
    cerr << t;
}
void _print(int t) { cerr << t; }
void _print(string t) { cerr << t; }
void _print(char t) { cerr << t; }
void _print(lld t) { cerr << t; }
void _print(double t) { cerr << t; }
void _print(ull t) { cerr << t; }

template <class T, class V>
void _print(pair<T, V> p);
template <class T>
void _print(vector<T> v);
template <class T>
void _print(set<T> v);
template <class T, class V>
void _print(map<T, V> v);
template <class T>
void _print(multiset<T> v);

template <class T, class V>
void _print(pair<T, V> p)
{
    cerr << "{";
    _print(p.ff);
    cerr << ",";
    _print(p.ss);
    cerr << "}";
}

template <class T>
void _print(vector<T> v)
{
    cerr << "[ ";
    for (T i : v)
    {
        _print(i);
        cerr << " ";
    }
    cerr << "]";
}

template <class T>
void _print(set<T> v)
{
    cerr << "[ ";
    for (T i : v)
    {
        _print(i);
        cerr << " ";
    }
    cerr << "]";
}

template <class T>
void _print(multiset<T> v)
{
    cerr << "[ ";
    for (T i : v)
    {
        _print(i);
        cerr << " ";
    }
    cerr << "]";
}

template <class T, class V>
void _print(map<T, V> v)
{
    cerr << "[ ";
    for (auto i : v)
    {
        _print(i);
        cerr << " ";
    }
    cerr << "]";
}

int last_row, first_col;
struct winsize win_size;
string promptInput;
string HOST, USER, CWD, HOME;
pid_t SHELL_ID, CHILD_ID = -1;
struct termios orig_termios_settings;

string CURR_JOB;
string alarm_message;
int alarm_seconds;
vector<string> hist;
int no, background_n;
int hist_i;

unordered_map<string, string> aliasMap;
unordered_map<string, string> exportMap;
bool status = true;

string hist_filename = "history.txt";

string PS1 = "";

struct bg_job
{
    char job_name[300];
    pid_t PID;
};

// bg_job jobs[50];
vector<bg_job> jobs;

//////////////////////////////////////////////////
// HEADERS BEGIN

void printBadMessage(string msg);
void execute_given_command(string command);
void populateAliasMap();
// HEADERS end

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

int setenviron(vector<string> s_args)
{
    int no_args = s_args.size();
    char *args[32];

    for (int i = 0; i < s_args.size(); i++)
    {
        args[i] = (char *)s_args[i].c_str();
    }

    if (s_args.size() != 2)
    {
        if (s_args.size() != 3)
        {
            cout << "Syntax 'setenv key value'" << endl;
            return -1;
        }
    }

    if (s_args.size() == 3)
    {
        if (setenv(args[1], args[2], 2) < 0)
        {
            printBadMessage("Error in setenv");
            return -1;
        }
        return 0;
    }

    if (s_args.size() == 2)
    {
        if (setenv(args[1], "\0", 2) < 0)
        {
            printBadMessage("Error in setenv.");
            return -1;
        }
    }

    return 0;
}

void unsetenviron(vector<string> s_args)
{
    int no_args = s_args.size();
    char *args[32];

    for (int i = 0; i < s_args.size(); i++)
    {
        args[i] = (char *)s_args[i].c_str();
    }

    if (!s_args.empty())
    {
        if (s_args.size() != 2)
        {
            // printf("Incorrect number of arguments. Provide only 1 argument.\nSyntax: `unsetenv var`\n");
            printBadMessage("Syntax : unsetenv key");
            return;
        }
    }

    if (!s_args.empty())
    {
        if (unsetenv(args[1]) < 0)
        {
            printBadMessage("Error in unsetenv");
            return;
        }
    }

    return;
}

string read_file(unsigned int n, string filename)
{

    char filename_buff[512];
    strcpy(filename_buff, filename.c_str());

    char line[512];
    char *info = (char *)calloc(100, sizeof(char));
    ;
    int i = 0;

    FILE *file = fopen(filename_buff, "r");

    for (i = 0; fgets(line, sizeof(line), file);)
    {
        i = i + 1;

        if (i != n)
        {
        }
        else
        {
            strcpy(info, line);
        }
    }

    fclose(file);
    return string(info);
}

void reduce_left_jobs(int i)
{
    int j = i;

    while (j < jobs.size() - 1)
    {

        jobs[i].PID = jobs[i + 1].PID;
        strcpy(jobs[i].job_name, jobs[i + 1].job_name);

        j = j + 1;
    }

    while (true)
    {
        break;
    }

    while (true)
    {
        background_n = background_n - 1;
        break;
    }
}

int32_t fg(vector<string> s_args)
{
    char *args[32];

    for (int i = 0; i < s_args.size(); i++)
    {
        args[i] = (char *)s_args[i].c_str();
    }

    int no_args = s_args.size();
    if (s_args.size() != 2)
    {
        cout << "Syntax = fg <jobNumber>\n"
             << endl;
        return -1;
    }
    int needed_id;
    int job_no = stoi(string(args[1]));
    int status;

    if (job_no <= jobs.size())
    {
        while (true)
        {

            tcsetpgrp(STDIN_FILENO, getpgrp());
            break;
        }
    }
    else
    {

        cout << "Not valid job Id." << endl;
        return -1;
    }

    needed_id = jobs[jobs.size() - 1].PID;
    // strcpy(CURR_JOB, jobs[job_no - 1].job_name);
    CURR_JOB = string(jobs[job_no - 1].job_name);
    CHILD_ID = needed_id;

    reduce_left_jobs(jobs.size() - 1);
    // TODO: delete last char

    while (true)
    {
        signal(SIGTTOU, SIG_IGN);

        break;
    }

    signal(SIGTTIN, SIG_IGN);

    while ((true))
    {
        tcsetpgrp(STDIN_FILENO, CHILD_ID);

        break;
    }

    int sigcont_var = 18;

    kill(needed_id, 18);

    while (true)
    {

        waitpid(needed_id, &status, WUNTRACED);
        break;
    }

    while (true)
    {

        tcsetpgrp(STDIN_FILENO, getpgrp());
        break;
    }

    signal(SIGTTOU, SIG_DFL);

    while (true)
    {

        signal(SIGTTIN, SIG_DFL);

        break;
    }

    if (!(WIFSTOPPED(status)))
    {

        cout << "Error in WIFSTOPPED\n"
             << endl;
    }
    else
    {

        jobs[background_n].PID = CHILD_ID;

        while (true)
        {
            strcpy(jobs[background_n].job_name, CURR_JOB.c_str());
            break;
        }

        while (true)
        {
            background_n = background_n + 1;
            break;
        }
    }

    return 0;
}

void see_jobs()
{

    char *status = (char *)calloc(52, sizeof(char));

    for (int i = 0; i < jobs.size(); i++)
    {
        FILE *f;

        while (true)
        {
            sprintf(status, "/proc/%d/status", jobs.at(i).PID);

            break;
        }

        while (true)
        {
            if (!(f = fopen(status, "r")))
            {

                string msg = "Error in see_jobs() while fopen()";
                printBadMessage(msg);
                return;
            }
            break;
        }

        while (true)
        {
            fclose(f);
            break;
        }

        string status_info = read_file(3, string(status));

        string temp = status_info;
        char *t1 = (char *)status_info.c_str();
        t1 = strtok(t1, " :\n\t\r");
        while (true)
        {
            break;
        }

        while (true)
        {
            t1 = strtok(NULL, " :\n\t\r");

            break;
        }

        while (true)
        {

            if (status_info[0] != 'T')
            {
                status_info = "Ongoing";
            }
            else
            {
                status_info = "Terminated";
            }

            break;
        }

        cout << "{" << i + 1 - 1 << "} " << jobs[i].job_name << " " << status_info << " {" << jobs[i].PID << "}";
    }
}

string removeCharacters(string S, char c)
{

    S.erase(remove(
                S.begin(), S.end(), c),
            S.end());

    return S;
}

void printGoodMessage(string msg)
{
    cout << BOLDGREEN << msg << RESET << endl;
}

void printBadMessage(string msg)
{
    cout << RED << msg << RESET << endl;
}

void printNeutralMessage(string msg)
{
    cout << BOLDWHITE << msg << RESET << endl;
}

// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

void shell();

bool goCursorPointer(int row, int col)
{

    if (row < 0 || col < 0)
    {
        return false;
    }

    string op = "\033[";
    op.append(to_string(row));
    op.append(";");
    op.append(to_string(col));
    op.append("H");
    std::cout << op;
    fflush(stdout);

    return true;
}
void clearLineMacro()
{
    // printf("\033[K");
    // printf("\033[1K");
    cout << "\033[2K";
}

void custom_exit(string msg)
{
    cout << RED << "In custom exit " << msg << RESET << endl;
    exit(EXIT_FAILURE);
}

void getIntoRawMode()
{
    // cout << "raw mode  begins" << endl;
    tcgetattr(STDIN_FILENO, &orig_termios_settings);
    struct termios raw = orig_termios_settings;
    raw.c_lflag &= ~(ECHO | ICANON);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != 0)
    {
        custom_exit("Unable to switch to Raw Mode");
    }
    // cout << "raw mode  begins" << endl;
}

void exitRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios_settings) != 0)
    {

        cout << RED << "shellid = " << SHELL_ID << " exiting from pid = " << getpid() << " parent id = " << getppid() << RESET << endl;
        custom_exit("exitRawMode() : Unable to switch to Normal Mode");
    }
    // cout << endl << "exited raw mode" << endl;
}

void getIntoRawModeWithEcho()
{
    // cout << "raw mode  begins" << endl;
    tcgetattr(STDIN_FILENO, &orig_termios_settings);
    struct termios raw = orig_termios_settings;
    raw.c_lflag &= ~(ICANON);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != 0)
    {
        perror("Unable to switch to Raw Mode with echo");
    }
    // cout << "raw mode  begins" << endl;
    // printf("In raw mode.");
}

void load_history()
{
    // strcpy(hist_filename, HOME);
    hist_filename = string(getenv("PWD"));
    // strcat(hist_filename, "/history.txt");
    hist_filename += "/history.txt";
    // debug(hist_filename)
    FILE *file = fopen(hist_filename.c_str(), "r");
    if (file != NULL)
    {
        // debug("loading history")
        char line[128];
        int no = 0;
        while (fgets(line, sizeof(line), file) != NULL)
        {
            // debug(string(line))
            hist.push_back(string(line));
            no++;
        }

        fclose(file);
    }
    else
    {
        custom_exit("History.txt failed");
    }
    return;
}

void save_history()
{
    FILE *file = fopen(hist_filename.c_str(), "a");

    for (int i = 0; i < hist.size(); i++)
    {
        trim(hist[i]);
        hist[i] = removeCharacters(hist[i], '\n');
        hist[i] = removeCharacters(hist[i], '\t');
        if (!hist[i].empty())
        {
            fprintf(file, "\n%s", hist[i].c_str());
        }
    }

    fclose(file);
}

// void history(char *command)
// {
//     char *token = command;
//     token = strtok(NULL, " \"\n\t\r");

//     int n = 10;
//     if (token != NULL)
//         n = atoi(token);

//     int count = n;

//     for (int i = hist_i - count; count > 0; i++, count--)
//     {
//         printf("%s", hist[i % 20].c_str());
//         //count++;
//     }
// }

void print_history()
{
    for (int i = 0; i < hist.size(); i++)
    {
        trim(hist[i]);
        hist[i] = removeCharacters(hist[i], '\n');
        hist[i] = removeCharacters(hist[i], '\t');
        if (!hist[i].empty())
        {
            cout << " - " << hist[i] << endl;
        }
    }
}
////////////////////////////
/** TRIE BEGINS **/

class TrieNode
{
public:
    bool isLeaf;
    string word;
    // TrieNode *arr[26];
    map<char, TrieNode *> children;

    TrieNode()
    {
        // cout << "created new node" << endl;
        this->isLeaf = false;
        this->word = "";
        // this->arr = new TrieNode();
        // for (int i = 0; i < 26; i++)
        // {
        //     this->arr[i] = nullptr;
        // }
    }
};

class Trie
{

public:
    TrieNode *root;

    Trie()
    {
        root = new TrieNode();
    }

    void insert(string word)
    {

        TrieNode *curr = this->root;

        for (int i = 0; i < word.size(); i++)
        {

            int index = word[i] - 'a';

            if (curr->children.find(word[i]) == curr->children.end())
            {
                curr->children[word[i]] = new TrieNode();
            }

            // curr = curr->arr[index];
            curr = curr->children[word[i]];
        }
        curr->isLeaf = true;
        curr->word = word;

        // cout << "insertion done " << curr->word << endl;
    }

    bool search(string word)
    {
        TrieNode *curr = this->root;
        if (curr == nullptr)
        {
            return false;
        }

        for (int i = 0; i < word.size(); i++)
        {

            int index = word[i] - 'a';

            // curr = curr->arr[index];
            curr = curr->children[word[i]];
            if (curr == nullptr)
            {
                return false;
            }
        }

        return curr->isLeaf;
    }

    bool startsWith(string prefix)
    {

        TrieNode *curr = this->root;
        if (curr == nullptr)
        {
            return false;
        }

        for (int i = 0; i < prefix.size(); i++)
        {

            int index = prefix[i] - 'a';

            // curr = curr->arr[index];
            curr = curr->children[prefix[i]];

            if (curr == nullptr)
            {
                return false;
            }
        }

        return true;
    }

    // begin autocomplete

    bool isLastNode(TrieNode *root)
    {
        if (root->children.size() == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void autocompleteRecUtil(TrieNode *curr, string &currPrefix, vector<string> &res)
    {
        if (curr->isLeaf)
        {
            res.push_back(currPrefix);
        }

        if (isLastNode(curr))
        {
            return;
        }

        for (auto itr = curr->children.begin(); itr != curr->children.end(); itr++)
        {
            char currentChar = itr->first;
            TrieNode *nextChild = itr->second;

            currPrefix.push_back(currentChar);

            autocompleteRecUtil(nextChild, currPrefix, res);

            currPrefix.pop_back();
        }
    }

    void autocomplete(string &prefix, vector<string> &res)
    {
        TrieNode *curr = this->root;

        if (curr == nullptr)
        {
            return;
        }

        // check if prefix itself is present or not
        for (int i = 0; i < prefix.size(); i++)
        {
            if (curr->children[prefix[i]] == nullptr)
            {
                return;
            }
            else
            {
                curr = curr->children[prefix[i]];
            }
        }

        // if prefix is a word add it to result
        if (curr->isLeaf && isLastNode(curr))
        {
            res.push_back(prefix);
            // MAY need to delete this statement because first line in recursive function is doing this
            // OR may node to exactly follow gf reference.
        }

        // if prefix is last node in branch
        if (isLastNode(curr))
        {
            return;
        }

        // if prefix branch has more nodes below
        string tempPrefix = prefix;
        autocompleteRecUtil(curr, tempPrefix, res);

        return;
    }

    // END autocomplete

    // BEGIN autocorrect

    void autocorrectRecUtil(
        TrieNode *curr,
        char ch,
        string &word,
        vector<int> pRow,
        vector<pair<string, int>> &res,
        int &maxCostAllowed)
    {
        int cols = word.size() + 1;
        vector<int> currentRow(1, pRow[0] + 1);

        for (int i = 1; i < cols; i++)
        {
            int insertCost = currentRow[i - 1] + 1;
            int deleteCost = pRow[i] + 1;

            int replaceCost = -1;
            if (word[i - 1] != ch)
            {
                replaceCost = pRow[i - 1] + 1;
            }
            else
            {
                replaceCost = pRow[i - 1];
            }

            int minCost = min(insertCost, min(deleteCost, replaceCost));
            currentRow.push_back(minCost);
        }

        // if the last entry in the row indicates the optimal cost is less than the
        // maximum cost, and there is a word in this trie node, then add it
        int lastIndex = currentRow.size() - 1;
        if (currentRow[lastIndex] <= maxCostAllowed && !(curr->word.empty()))
        { // may need to consider second condition
            res.push_back({curr->word, currentRow[lastIndex]});
        }

        // if any entries in the row are less than the maximum cost, then
        // recursively search each branch of the trie
        int minCurrRow = INT_MAX;
        for (int i = 0; i < currentRow.size(); i++)
        {
            minCurrRow = min(minCurrRow, currentRow[i]);
        }

        if (minCurrRow <= maxCostAllowed)
        {
            for (auto itr = curr->children.begin(); itr != curr->children.end(); itr++)
            {
                char ch = itr->first;
                TrieNode *nextNode = itr->second;

                autocorrectRecUtil(curr->children[ch], ch, word, currentRow, res, maxCostAllowed);
            }
        }
    }

    void autocorrect(string word, int maxCostAllowed, vector<pair<string, int>> &res)
    {
        // creating first row for given word, Trie is not considered at this point
        int wl = word.size();
        vector<int> currentRow(wl + 1);
        for (int i = 0; i < wl + 1; i++)
        {
            currentRow[i] = i;
            // cout << currentRow[i] << endl;
        }

        TrieNode *curr = this->root;

        for (auto itr = curr->children.begin(); itr != curr->children.end(); itr++)
        {
            char ch = itr->first;
            TrieNode *nextNode = itr->second;

            autocorrectRecUtil(curr->children[ch], ch, word, currentRow, res, maxCostAllowed);
        }

        // return res; no need here becuase pass by reference
    }

    // END autocorrect
};

Trie *trie = new Trie();

void insertWordInTrie(string word)
{
    trie->insert(word);
}

void populateTrie()
{
    vector<string> basicCommands = {"ls", "echo", "touch", "mkdir", "grep", "pwd", "cd", "cat", "head", "tail", "exit", "history", "clear", "cp"};

    for (int i = 0; i < basicCommands.size(); i++)
    {
        // trie->insert(basicCommands[i]);
        insertWordInTrie(basicCommands[i]);
    }
}

/** TRIE ENDS **/

///////////////////////////////////
string prompt()
{
    if (!(PS1.size() == 0 || PS1.empty()))
    {
        return PS1;
    }

    string DIR;
    char buff_cwd[1024];
    string CWD = string(getcwd(buff_cwd, sizeof(buff_cwd)));
    string HOME = string(getenv("HOME"));
    char buff[2048];
    gethostname(buff, sizeof(buff));
    HOST = string(buff);

    getlogin_r(buff, sizeof(buff));
    // cout << buff << endl;
    string USER = string(buff);
    // char *DIR;

    string res;
    string user_symbol;
    if (geteuid())
    {
        // cout << "you are not root" << endl;
        user_symbol = "%";
    }
    else
    {

        // cout << "you are  root" << endl;
        user_symbol = "#";
    }
    char *p;
    p = (char *)strstr(CWD.c_str(), HOME.c_str());

    if (p)
    {
        DIR = p + HOME.size();
        // cout << BOLDMAGENTA << "OK" << USER << "@" << HOST << RESET << ":" << BOLDBLUE << "~" << DIR << user_symbol << RESET;
        res = "\n" + string(BOLDMAGENTA) + USER + "@" + HOST + RESET + ":" + BOLDBLUE + "~" + DIR + user_symbol + " " + RESET;
    }
    else
    {
        DIR = CWD;
        // printf("<\x1B[1;32m%s@%s\x1B[0m:\x1B[1;34m%s\x1B[0m>", USER, HOST, DIR);
        // cout << BOLDMAGENTA << "OK" << USER << "@" << HOST << RESET << ":" << BOLDBLUE << "~" << DIR << user_symbol << RESET;
        res = "\n" + string(BOLDMAGENTA) + USER + "@" + HOST + RESET + ":" + BOLDBLUE + "~" + DIR + user_symbol + " " + RESET;
    }

    // }

    return res;
}

string get_input()
{

    string promptInputCopy;
    char ch = ' ';
    while (1)
    {
        ch = cin.get();
        // cout << "ch = " << ch << endl;

        if (ch == '\x1b')
        {
            /* arrow keys ignore. could be later used for history
            cout << "arrow pressed read two more bytes.";
            ch = cin.get(); // ignore
            ch = cin.get(); // actual character A - up, C - right, B - down, D- left
            cout << "actual char is " << ch << endl;
            */
            //    cin.get(); // [ bracker taht marks end of esacape
            //     cin.get(); // actual charavt
            clearLineMacro();
            clearLineMacro();
            clearLineMacro();
            goCursorPointer(last_row - 1, 0);
            cout << prompt() << promptInput;
        }
        else if (ch == 10)
        {
            // enter

            // cout << "command you entered = " << promptInput << " I've cleared promptIput, use returned value" << endl;;
            promptInputCopy = promptInput;
            promptInput.clear();
            // cout << prompt();
            break;
        }
        else if (ch == '\t')
        {
            // should i clear it?
            vector<string> auto_compeltion_results;
            string last_word;
            if (promptInput.find_last_of(' ') != string::npos)
            {
                // cout << promptInput.substr(promptInput.find_last_of(' '))<< " will be choice" << endl;
                last_word = promptInput.substr(promptInput.find_last_of(' '));
            }
            else
            {
                // cout << promptInput << " will be choice" << endl;
                last_word = promptInput;
            }

            trim(last_word);
            trie->autocomplete(last_word, auto_compeltion_results);

            // cout << "autocomplete attempted with input = " << last_word << " found results = " << auto_compeltion_results.size() << endl;
            if (auto_compeltion_results.empty())
            {
                // cout << "no autocompletions" << endl;
                clearLineMacro();
                goCursorPointer(last_row - 1, 0);
                cout << prompt() << promptInput;
            }
            else if (auto_compeltion_results.size() == 1)
            {
                // cout << "picking auto completion result" << endl;
                // promptInput.clear();
                trim(promptInput);
                if (promptInput.find(' ') == std::string::npos)
                {
                    // only one word in promptInput
                    promptInput = auto_compeltion_results[0];
                }
                else
                {
                    promptInput = promptInput.substr(0, promptInput.find_last_of(' '));
                    promptInput = promptInput + " " + auto_compeltion_results[0];
                }
                // cout << "should append to " << promptInput << endl;

                clearLineMacro();
                goCursorPointer(last_row - 1, 0);
                cout << prompt() << promptInput;
            }
            else
            {
                cout << "the proposed autocompletions are" << endl;
                cout << endl; // to show them on new line
                for (int i = 0; i < auto_compeltion_results.size(); i++)
                {
                    cout << auto_compeltion_results[i] << "\t";
                }
                cout << endl; // to preserve them
                // cout << "results done" << endl;

                clearLineMacro();
                goCursorPointer(last_row - 1, 0);
                cout << prompt() << promptInput;
            }
        }
        else if (ch == 127)
        {
            // cout << "popping" << endl;

            if (!promptInput.empty())
            {
                promptInput.pop_back();

                clearLineMacro();
                goCursorPointer(last_row - 1, 0);
                cout << prompt() << promptInput;
            }
            else
            {

                clearLineMacro();
                goCursorPointer(last_row - 1, 0);
                cout << prompt() << promptInput;
            }
        }
        else
        {
            promptInput.push_back(ch);
        }
    }
    return promptInputCopy;
}

vector<string> get_tokens_whole_commands(string input)
{
    // stringstream class check1
    vector<string> tokens;
    stringstream check1(input);

    string intermediate;

    // Tokenizing w.r.t. space ' '
    while (getline(check1, intermediate, ';'))
    {
        trim(intermediate);
        if (!intermediate.empty())
        {
            tokens.push_back(intermediate);
        }
    }

    // Printing the token vector
    // for (int i = 0; i < tokens.size(); i++)
    // {
    //     cout << tokens[i] << '\n';
    // }
    return tokens;
}

vector<string> get_internal_tokens_of_a_single_command(string command)
{

    vector<string> res;
    char copy_command[command.size() + 10];
    strcpy(copy_command, command.c_str());

    //delimiters = space, newline, tab, carriage reurn
    // string str_delim = " \n\t\r";
    char delim[32] = " \n\t\r";

    char *tok = (char *)calloc(2048, sizeof(char));
    tok = strtok(copy_command, delim);

    if (tok == NULL)
    {
        return res;
    }

    while (tok != NULL)
    {
        string str_tok = string(tok);
        trim(str_tok);
        res.push_back(str_tok);
        tok = strtok(NULL, delim);
    }

    // cout << "this command has following internal tokns" << endl;
    // for (int i = 0; i < res.size(); i++)
    // {
    //     cout << res[i] << ", ";
    // }
    // cout << endl;

    return res;
}

bool check_pipes(string command)
{
    size_t found = command.find("|");

    if (found == std::string::npos)
    {
        return false;
    }
    else
    {
        return true;
    }
}

vector<string> get_individual_pipes(string command)
{
    vector<string> res;
    char copy_command[command.size() + 10];
    strcpy(copy_command, command.c_str());

    //delimiters = space, newline, tab, carriage reurn
    char delim[32] = "|";

    // char *tok = (char *)malloc(sizeof(char) * 2048);
    char *tok = (char *)calloc(2048, sizeof(char));
    tok = strtok(copy_command, delim);

    if (tok == NULL)
    {
        return res;
    }

    while (tok != NULL)
    {
        string str_tok = string(tok);
        trim(str_tok);
        res.push_back(str_tok);
        tok = strtok(NULL, delim);
    }

    // cout << "this command has following internal PIPES" << endl;
    // for (int i = 0; i < res.size(); i++)
    // {
    //     cout << res[i] << ", ";
    // }
    // cout << endl;

    return res;
}

void handle_piping(string command)
{
    vector<string> individual_pipes = get_individual_pipes(command);

    // cout << "in handle piping begin" << endl;
    // for(int i = 0; i < individual_pipes.size(); i++){

    //     cout << individual_pipes[i] << endl;
    // }
    // cout << "in handle piping end" << endl;

    int pipes[2], fd = 0;
    pid_t proc;

    individual_pipes.push_back("");
    // individual_pipes.push_back(nullptr);
    if (HOME.empty())
    {

        for (int j = 0; !individual_pipes[j].empty(); j++)
        {
            // cout << "executing pipe = begin" << endl;
            // cout << individual_pipes[j] << endl;
            // cout << "executing pipe = end" << endl;
            pipe(pipes);
            proc = fork();

            if (proc < 0)
            {
                perror("Unable to fork");
            }

            else if (proc == 0)
            {
                dup2(fd, 0);

                if (individual_pipes[j + 1] != "\0" && (!individual_pipes[j + 1].empty()))
                    dup2(pipes[1], 1);

                close(pipes[0]);
                execute_given_command(individual_pipes[j]);

                // necessary because child should finish
                exit(EXIT_SUCCESS);
            }
            else if (proc > 0)
            {
                wait(NULL);
                close(pipes[1]);
                fd = pipes[0];
            }
        }
    }
    // cout << "in handle piping begin" << endl;
    // for(int i = 0; i < individual_pipes.size(); i++){

    //     cout << individual_pipes[i] << endl;
    // }
    // cout << "in handle piping end" << endl;

    int pid = fork();
    if (pid == 0)
    {
        // cout << "goint" << endl;
        execlp("/bin/sh", "/bin/sh", "-c", command.c_str(), (char *)NULL);
    }
    else
    {
        wait(&pid);
        // cout << "done " << pid << endl;
        return;
    }
}

bool check_io_redirection(string command)
{
    size_t found1 = command.find(">");
    size_t found2 = command.find(">>");
    size_t found3 = command.find("<");

    if (found1 == std::string::npos && found2 == std::string::npos && found3 == std::string::npos)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void handle_io_redirection(string command_string)
{
    int copy_stdERR = dup(STDERR_FILENO);
    int copy_stdOUT = dup(STDOUT_FILENO);
    int copy_stdIN = dup(STDIN_FILENO);

    int status;

    char command[2048];
    strcpy(command, command_string.c_str());

    char *output[2], *input[2];
    char *out_file = NULL;
    char *in_file = NULL;

    char *inp = strstr(command, "<");
    int in = !(inp == NULL);
    //printf(" int is %d", in);
    int out_type = 0;
    char *out = strstr(command, ">>");

    if (out != NULL)
    {
        out_type = 2;
    }
    else
    {
        out = strstr(command, ">");
        if (out != NULL)
        {
            out_type = 1;
        }
    }

    output[0] = &command[0];

    if (out_type)
    {
        output[0] = strtok(command, ">");
        output[1] = strtok(NULL, ">");
        out_file = strtok(output[1], " \r\t\n");
    }

    input[0] = output[0];
    if (in)
    {
        input[0] = strtok(input[0], "<");
        input[1] = strtok(NULL, "<");
    }

    char **args = (char **)calloc(512, (sizeof(char *)));

    int no_args = 0;

    if (in)
    {
        if (input[1] == NULL)
        {

            printBadMessage("Give filename which contains input");
            return;
        }

        //printf("\n fk no %s\n",input[1]);
        in_file = strtok(input[1], " \n\r\t");

        bool is_file_valid;
        struct stat file_temp;
        if (stat(in_file, &file_temp) != 0)
        {
            perror("Error in stat");
            is_file_valid = false;
        }
        else
        {
            if (!S_ISDIR(file_temp.st_mode))
            {
                is_file_valid = true;
            }
            else
            {
                is_file_valid = false;
            }
        }

        if (!is_file_valid)
        {

            printBadMessage("Filename doesn't exist.");
            return;
        }
    }

    input[0] = strtok(input[0], " \n\r\t");

    while (input[0] != NULL)
    {
        args[no_args] = (char *)malloc(sizeof(char) * strlen(input[0]) + 10);
        strcpy(args[no_args], input[0]);
        input[0] = strtok(NULL, " \n\t\r");
        no_args++;
    }

    args[no_args] = NULL;

    if (out_type)
    {
        if (out_file == NULL)
        {

            printBadMessage("Output file name not given.");
            return;
        }
    }

    pid_t pid = fork();
    

    if (pid == 0)
    {

        if (in)
        {

            int fd_in = open(in_file, O_RDONLY);
            if (fd_in < 0)
            {

                printBadMessage("Error while redirecting input.");
                return;
            }

            dup2(fd_in, 0);
            close(fd_in);
        }

        if (out_type)
        {
            int fd_out;
            if (out_type == 1)
            {
                fd_out = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }

            else if (out_type == 2)
            {
                fd_out = open(out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }

            if (fd_out < 0)
            {

                printBadMessage("Error while redirecting output.");
                return;
            }

            dup2(fd_out, 1);
            close(fd_out);
        }

        if (execvp(args[0], args) < 0)
        {

            printBadMessage("Execvp: given command doesn't exist.");
            exit(EXIT_FAILURE);
        }

        dup2(copy_stdIN, 0);
        close(copy_stdIN);

        dup2(copy_stdOUT, 1);
        close(copy_stdOUT);
    }
    else if (pid < 0)
    {

        printBadMessage("Error on executing fork()");
        return;
    }

    else
    {
        wait(&status);
        
    }

    

    return;
}

void ignore_teletype()
{

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
}

void default_teletype()
{

    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}

void RUN(vector<string> args, bool should_be_background)
{
    int status;
    pid_t pid = fork(), wpid;
    // args[] = NULL;
    // args.push_back(nullptr);

    if (pid == 0)
    {
        // cout << RED << "In child" << RESET << endl;

        char *parsed[100];
        int i = 0;
        for (i = 0; i < args.size(); i++)
        {
            parsed[i] = (char *)args[i].c_str();
        }
        parsed[i] = NULL;

        // cout << RED << "execvp for - " << parsed[0] << RESET << endl;

        setpgid(0, 0);
        if (execvp(parsed[0], parsed) < 0)
        {
            perror("Command not found");
            status = false;
            cout << RED << args[0] << RESET << endl;
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    else
    {

        if (should_be_background == 0)
        {
            CURR_JOB = args[0];
            CHILD_ID = pid;

            int i = 1;

            while (i < args.size())
            {

                CURR_JOB = CURR_JOB + " ";
                CURR_JOB = CURR_JOB + args[i];

                i = i + 1;
            }

            ignore_teletype();

            tcsetpgrp(STDIN_FILENO, pid);
            wpid = waitpid(pid, &status, WUNTRACED);
            tcsetpgrp(STDIN_FILENO, getpgrp());

            default_teletype();

            while (true)
            {

                if (WIFSTOPPED(status))
                {

                    cout << CURR_JOB << " with PID = " << pid << " suspended." << endl;

                    struct bg_job temp_bg;

                    background_n = background_n + 1;

                    while (true)
                    {

                        strcpy(temp_bg.job_name, CURR_JOB.c_str());
                        temp_bg.PID = CHILD_ID;
                        jobs.push_back(temp_bg);
                        break;
                    }
                }

                break;
            }

            return;
        }

        else
        {

            while (true)
            {

                background_n = background_n + 1;

                struct bg_job temp_bg;

                temp_bg.PID = pid;
                strcpy(temp_bg.job_name, args[0].c_str());

                int i = 1;

                while (i < args.size())
                {

                    strcat(temp_bg.job_name, " ");
                    strcat(temp_bg.job_name, args[i].c_str());

                    i = i + 1;
                }

                cout << "[" << background_n << "] " << pid << endl;
                jobs.push_back(temp_bg);

                break;
            }

            return;
        }
    }
}

void get_pwd()
{
    char buff[1024];
    getcwd(buff, sizeof(buff));

    cout << "PWD = " << string(buff) << endl;

    return;
}

void do_cd(string path)
{

    trim(path);

    path = path.substr(path.find_first_of("cd") + 1 + string("cd").size());
    trim(path);
    // cout << "got path - " << path << endl;

    if (path.empty())
    {
        // cout << "empty path" << endl;
        return;
    }

    if (path == "-")
    {
        // cout << "bad path" << endl;

        return;
    }

    if (path.find("~") != std::string::npos)
    {
        // contains
        path = string(getenv("HOME")) + path.substr(path.find_first_of("~") + 1);
    }

    char buff[PATH_MAX];
    char *rp = realpath(path.c_str(), buff);

    string rps = string(rp);
    trim(rps);
    if (rps.empty())
    {
        // cout << "rps is empty" << endl;
        return;
    }

    // cout << "trying cd into " << rp << endl;
    if (chdir(rp) < 0)
    {
        // cout << "bad path" << endl;
        return;
    }

    // getcwd(buff, sizeof(buff));
    // cout << "cwd is " << string(buff) << endl;
}

void clearScreenMacro()
{
    printf("\033[H\033[J");
}

void do_quit()
{
    save_history();

    cout << " 🙏 " BOLDGREEN << "Ram Ram ji, chalate hai!" << RESET << " 🙏 " << endl;
    exit(0); // exit seuccess
}

// void wakeup(int signum)
// {
//     if (signum == SIGALRM)
//     { //signal handler for SIGALRM

//         cout << alarm_message << endl;
//         alarm(alarm_seconds);
//     }
//     if (signum == SIGINT)
//     { // signal handler for SIGINT
//         // printf("\nSnoozing for 5 seconds....\n");
//         alarm(0);
//         // _exit(EXIT_SUCCESS);
//         // exit(EXIT_SUCCESS);
//     }
// }

void do_alarm(vector<string> msg, int seconds)
{
    if (msg.size() < 3)
    {
        cout << "Invalid alarm" << endl;
        return;
    }

    string sms = "";
    for (int i = 2; i < msg.size(); i++)
    {
        sms += msg[i];
        sms += " ";
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        sleep(seconds);
        cout << sms << endl;
        cout << prompt();
        exit(EXIT_SUCCESS);
    }
    else
    {
        // wait(NULL);
    }
    // signal(SIGALRM, wakeup); // Register signal handler for SIGALRM
    // signal(SIGINT, wakeup);  // Register signal handler for SIGINT

    // alarm_seconds = seconds;
    // alarm_message = msg;

    // alarm(alarm_seconds); // Schedule the first alarm after 2 seconds

    // for (int i = 1;; i++)
    // {
    //     // printf("%d : Inside main function\n",i);
    //     pause(); // waiting until signal is handled
    // }

    // cout << "Exited from alarm" << endl;
}

void openFile(const char *filename)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        close(2);
        execlp("xdg-open", "xdg-open", filename, NULL);
        exit(0);
    }
    // if (fork() == 0)
    // {
    // 	execl("/usr/bin/xdg-open", "xdg-open", filename, (char *)0);
    // 	exit(1);
    // }
}

void execute_given_command(string command)
{
    // cout << "in execute coommand" << endl;
    vector<string> args = get_internal_tokens_of_a_single_command(command);
    hist.push_back(command);

    trim(command);

    // if(command[0] == 'P' && command[1] == 'S' && command[2] == '1'){
    //     cout << "second is wokring" << endl;
    // }

    // cout << RED << command.rfind("PS1", 0) << endl;
    if (command.rfind("PS1", 0) == 0)
    {

        cout << "ps1 detected" << endl;
        command = removeCharacters(command, '"');

        size_t pos = command.find("DEFAULT");
        if (pos != std::string::npos)
        {
            // If found then erase it from string
            command.erase(pos, string("DEFAULT").length());
        }

        PS1 = command.substr(command.find_first_of("=") + 1);
        return;
    }

    if (check_pipes(command))
    {
        handle_piping(command);
        return;
    }

    if (check_io_redirection(command))
    {
        handle_io_redirection(command);
        return;
    }
    if (command == "echo $$")
    {
        cout << getpid() << endl;
    }
    else if (command == "echo $?")
    {
        cout << !(status) << endl;
    }
    else if(command == "echo $HOME"){
        cout << HOME << endl;
    }
    else if(command == "echo $USER"){
        cout << USER << endl;
    }
    else if(command == "echo $HOSTNAME"){
        cout << HOST << endl;
    }


    else if (args[0] == "jobs")
    {
        see_jobs();
    }
    else if (args[0] == "pwd")
    {
        // cout << GREEN << "need pwd()" << RESET << endl;
        get_pwd();
    }
    else if (args[0] == "cd")
    {
        // cout << GREEN << "need cd()" << RESET << endl;
        do_cd(command);
    }
    else if (args[0] == "quit")
    {
        // cout << GREEN << "need exit()" << RESET << endl;
        do_quit();
    }
    else if (args[0] == "history")
    {
        // cout << GREEN << "need history()" << RESET << endl;
        print_history();
    }
    else if (args[0] == "clear")
    {
        // cout << GREEN << "need clear()" << RESET << endl;
        clearLineMacro();
        clearScreenMacro();
        goCursorPointer(last_row - 1, 0);
        // cout << prompt();
    }
    /* export x = 3, export -p, export -n name */
    else if (args[0] == "export")
    {
        int sz = args.size();
        // debug(sz);

        if (sz == 0)
        {
            status = false;
            return;
        }
        if (sz > 1 && args[1].empty())
        {
            status = false;
            return;
        }
        else
        {
            if (sz > 1 && !(args[1] != "-p" || args[1] != "-n"))
            {
                status = false;
                return;
            }
        }

        if (sz > 1 && args[1] == "-p")
        {
            if (exportMap.size() > 0)
            {
                cout << "Export Variables are" << endl;
                for (auto it : exportMap)
                {
                    cout << it.first << " = " << it.second << endl;
                }
            }
        }
        else if (sz > 1 && args[1] == "-n")
        {
            string to_remove = args[2];
            exportMap.erase(to_remove);
        }
        else if (sz == 4)
        {

            exportMap[args[1]] = args[3];
            /* make it avaialable to its child processes */
            /* int setenv(const char *envname, const char *envval, int overwrite); */
            setenv(args[1].c_str(), args[3].c_str(), 1);
            // pid_t pid = fork();
            // if(pid == 0)
            // {
            //     auto val = getenv(args[1].c_str());
            //     cout << "Value = " << val << endl;
            // }
            // else
            // {
            //     wait(NULL);
            // }
            cout << GREEN << "Exported successfully" << RESET << endl;
        }
        else
        {
            status = false;
            cout << "Invalid command" << endl;
        }
        // cout << GREEN << "Exported successfully" << RESET << endl;
    }
    else if (args[0] == "alarm")
    {
        // cout << GREEN << "need alarm()" << RESET << endl;
        do_alarm(args, stoi(args[1]));
    }
    else if (args[0] == "open")
    {
        openFile(args[1].c_str());
        // cout << GREEN << "need open()" << RESET << endl;
    }
    /* checking whether the command is aliased before, search in unordered map */
    else if (aliasMap.find(args[0]) != aliasMap.end())
    {
        string aliasValue = aliasMap[args[0]];

        vector<string> aliasTokens;
        // stringstream class check1
        stringstream check1(aliasValue);

        string intermediate;

        // Tokenizing w.r.t. space ' '
        while (getline(check1, intermediate, ' '))
        {
            aliasTokens.push_back(intermediate);
        }
        for (int i = 1; i < args.size(); i++)
        {
            aliasTokens.push_back(args[i]);
        }
        RUN(aliasTokens, false);
    }
    else if (args[0] == "alias")
    {
        if (args.size() < 4)
        {
            cout << "Invalid alias" << endl;
            status = false;
            return;
        }

        string aliasKey = args[1];
        string aliasValue = "";
        for (int i = 3; i < args.size(); i++)
        {
            aliasValue += args[i] + " ";
        }
        trim(aliasValue);
        // aliasMap.insert(make_pair(aliasKey, aliasValue));
        aliasMap[aliasKey] = aliasValue;

        ofstream outfile;

        outfile.open(".myrc", std::ios_base::app); // append instead of overwrite
        outfile << '\n'
                << command;
        outfile.close();

        populateAliasMap();

        // cout << GREEN << "need alias()" << RESET << endl;
    }

    else if (args[0] == "fg")
    {
        // cout << GREEN << "need fg()" << RESET << endl;
        fg(args);
    }
    else if (args[0] == "setenv")
    {
        // cout << GREEN << "need setenv()" << RESET << endl;
        setenviron(args);
    }
    else if (args[0] == "unsetenv")
    {
        // cout << GREEN << "need unsetenv()" << RESET << endl;
        unsetenviron(args);
    }
    else if (args.back() == "&")
    {
        // args[args.size() - 1] = '\0';
        args.pop_back();

        // cout << RED << "need GENERIC RUN 1 for '&' sign : RUN()" << RESET << endl;
        RUN(args, true);
    }
    else if (args.back().back() == '&')
    {
        args[args.size() - 1][args.back().size() - 1] = '\0';

        // cout << RED << "need GENERIC RUN 2 for '&' sign : RUN()" << RESET << endl;
        RUN(args, true);
    }
    else if (args[0] == "quit")
    {
        // cout << GREEN << "need history store quit()" << RESET << endl;
        do_quit();
    }
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////

    else
    {
        // cout << RED << "need GENERIC RUN : RUN()" << RESET << endl;
        RUN(args, false);
    }
}

vector<string> tokenize_custom_delimeter(string input, char delimeter)
{
    vector<string> tokens;
    stringstream check1(input);

    string intermediate;

    // Tokenizing w.r.t. space ' '
    while (getline(check1, intermediate, delimeter))
    {
        trim(intermediate);
        if (!intermediate.empty())
        {
            tokens.push_back(intermediate);
        }
    }

    // Printing the token vector
    // for (int i = 0; i < tokens.size(); i++)
    // {
    //     cout << tokens[i] << '\n';
    // }
    return tokens;
}

void populateAliasMap()
{
    ifstream infile(".myrc");
    // debug("hello4");
    string line;
    while (getline(infile, line))
    {
        // debug("hello5");
        // cout << line << endl;
        vector<string> myrc_vec = tokenize_custom_delimeter(line, ' ');
        if (!myrc_vec.empty() && myrc_vec[0] == "alias")
        {

            // debug("hello6");
            string aliasKey = myrc_vec[1];
            // debug("hello7");
            string aliasValue = "";
            for (int i = 3; i < myrc_vec.size(); i++)
            {
                aliasValue += myrc_vec[i] + " ";
            }
            trim(aliasValue);
            // aliasMap.insert(make_pair(aliasKey, aliasValue));
            aliasMap[aliasKey] = aliasValue;
        }
        // istringstream iss(line);
        // cout << iss
        // int a, b;
        // if (!(iss >> a >> b)) { break; } // error

        // process pair (a,b)
    }
    infile.close();
    // debug("hello10");

    // cout << "The aliasMap contains" << endl;
    // for(auto it: aliasMap)
    // {
    //     cout << "Key: " << it.first << ", Value: " << it.second << endl;
    // }
}

void shell()
{
    cout << prompt();
    string input;
    // debug("hello");
    populateTrie();
    // debug("hello2");
    populateAliasMap();
    // debug("hell3");
    do
    {
        CHILD_ID = -1;
        // cout << prompt();
        input = get_input();

        // cout << endl << RED << input << RESET << endl;
        trim(input);
        if (input.empty())
        {
            // cout << "no command" << endl;
        }
        else
        {

            // cout << "command you entered = " << input << endl;
            vector<string> tokens = get_tokens_whole_commands(input);

            for (int i = 0; i < tokens.size(); i++)
            {
                execute_given_command(tokens[i]);
            }
            /*
            switch(command)
        
        */
        }

        cout << prompt();
        // char **commands;
        // commands = tokenize(input);
        // for(int i = 0; i < no; i++)
        // execute_com(commands[i]);

    } while (1);
}

/////////////

void handler(int sig)
{
    int pstatus;
    pid_t pid = waitpid(-1, &pstatus, WNOHANG);

    if (pid > 0)
    {
        char j_name[256];
        int index = 0;

        int i = 0;
        while (i < jobs.size())
        {

            if (jobs[i].PID == pid)
            {

                while (true)
                {
                    strcpy(j_name, jobs[i].job_name);

                    while (true)
                    {
                        jobs.erase(jobs.begin() + i);
                        break;
                    }

                    index = 1;
                    break;
                }
                break;
            }

            i = i + 1;
        }

        if (WEXITSTATUS(pstatus) == 0 && WIFEXITED(pstatus) && index)
        {

            cout << prompt();
        }

        else if (index)
        {

            cout << prompt();
        }

        fflush(stdout);
        fflush(stdout);
        fflush(stdout);
    }

    return;
}

void ctrl_c(int signum)
{
    exitRawMode();
    pid_t p = getpid();
    if (p < 0)
    {

        printBadMessage("Error while exiting raw mode");
        return;
    }

    if (p != SHELL_ID)
    {
        return;
    }

    if (CHILD_ID != -1)
    {
        kill(CHILD_ID, SIGINT);
    }

    while (true)
    {

        signal(SIGINT, ctrl_c);
        break;
    }

    return;
}

void ctrl_z(int signum)
{
    exitRawMode();
}

/////////////////////////

/////////////////////////
void init()
{

    char buff[2048];
    gethostname(buff, sizeof(buff));
    HOST = string(buff);

    getlogin_r(buff, sizeof(buff));
    // cout << buff << endl;
    USER = string(buff);
    // char *DIR;

    pid_t p_id;
    p_id = getpid();
    SHELL_ID = p_id;

    HOME = string(getenv("HOME"));

    // debug(getenv("PWD"));
    // debug(HOME);
    // debug(USER);
    // debug(SHELL_ID);
    // debug(HOST);
    load_history();
    // debug((int)hist.size());

    populateTrie();
    getIntoRawModeWithEcho();
    atexit(exitRawMode);

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size);

    last_row = win_size.ws_row;
    first_col = 0;
    goCursorPointer(last_row, first_col);
}

int main()
{
    init();

    if (geteuid())
    {
        // cout << "you are not root" << endl;
    }
    else
    {

        // cout << "you are  root" << endl;
    }

    // signal(SIGTSTP, ctrl_z);
    // signal(SIGTSTP, ctrl_z);
    signal(SIGINT, ctrl_c);
    signal(SIGCHLD, handler);
    // signal(SIGCHLD, handler2);

    // prompt();

    // string promptInputCopy = get_input();
    // cout << "Got : " << promptInput << endl;
    // sleep(5);
    // signal(SIGALRM, &wakeup);

    clearLineMacro();
    clearScreenMacro();
    goCursorPointer(last_row - 1, 0);

    shell();
    return 0;
}