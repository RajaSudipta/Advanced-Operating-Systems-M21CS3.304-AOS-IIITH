# POSIX Shell

shellFish

## Installation



```bash
g++ shellFish.cpp -o shellFish && ./shellFish
```

### Go through the `Slides Project.pptx`. The screenshot of commands and working of them are described there. 

### Commands
```
echo $$
```
```
autocompetion using <tab> key.
```
```
echo HOST
```
```
root user (#) prompt, 
normal user (%) prompt.
```
```
PS1="=>"
```
```
PS1=DEFAULT
```
```
cd ..
```
```
cd HOME
```
```
cd ~
```
```
alarm 4 you are late
```
```
cat a.txt | head -4 | tail -3 | sort
```
```
cat a.txt > op.txt
```
```
cat a.txt | head -4 | tail -3 | sort > op.txt
```
```
cat a.txt  >> op.txt
```
```
./a.out < input
```
```
alias c = cat
### 1. alias: you should type like alias 'll = ls -la'(ignore the quotes, the space should be given as shown)
```
```
export a = string
export -p # print all
export -n keyname
### 2. export: type like 'export x = 3'(ignore the quotes, the space should be given as shown), export -p, export -n name
```
```
pwd
```
```
history
```
```
clear
```
```
open a.mp4
```
```
sleep 100 &
```
```
fg jobnumber
```
```
cat ok.txt | grep pattern
```
```
ls, echo, touch, mkdir, grep, pwd, cd, cat, 
```
```
head, tail, chmod, exit, history, clear, cp
```
