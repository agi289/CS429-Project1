To compile the assembler I used the command
gcc -o assem assembler.c assembler.h
./assem "t1.tk" 
~ I replaced "t1.tk" with whichever input file I used, for example t2.tk and so forth

To compile the sinulator I used the command
gcc -o sim simulator.c
./sim "sample.tko" 
~ if the input file was t1.tk the output would be t1.tko
