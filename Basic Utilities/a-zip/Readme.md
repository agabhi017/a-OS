# a-zip

* `a-zip` is a file compression tool that uses a simple form of compression called `Run Length Encoding (RLE)`
* RLE is quite simple, upon encountering a string of same characters, it converts it into the form: {character count}{character} where {character count} is a 4 byte integer
* By default, the output is printed onto `stdout` but could be re-directed using `>` (shell re-direction) to a file

### Usage
* Compile `a-zip.c` into a binary (for example a-zip)
* On the prompt, type
```C
prompt> ./a-zip filename [filename2 ..]
```
* If more than one file are passed as inputs, each of the files are compressed into a single file (or output) (and hence, the information about the number of files/content of each file is lost) 
