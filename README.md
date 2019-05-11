# Pong Ultimate
A pong version with 4 paddles all controlled by your cursor. It's implemented in C using SDL for graphics

# Installation
## On Mac:
1. Download the SDL Development Libraries from https://www.libsdl.org/download-2.0.php
2. Double click on the .dmg
3. Copy the SDL2.framework to /Library/Frameworks/
4. Download the code
```
$ git clone https://github.com/noahhaasis/pong_ultimate.git
```
5. Move into the project directory
```
$ cd pong_ultimate
```
6. Build the project
```
$ gcc -ggdb3 -std=c11 -Wall -c -o pong_ultimate.o pong_ultimate.c
$ gcc -ggdb3 -std=c11 -Wall -framework SDL2 -o pong_ultimate pong_ultimate.o
```

# Usage
```
$ ./pong_ultimate
```
