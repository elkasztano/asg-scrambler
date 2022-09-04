# asg-scrambler
data scrambler based on an alternating step generator (asg)   
for demonstration purposes only  
Do not use to encrypt confident and/or important data!
## compiling
Compiling is pretty straight forward, as no external libraries are used.  
Example using gcc: `gcc -o asg-scrambler asg-scrambler.c`  
## usage
To prevent the password from being stored in your console history, you can use the following command:  

`read -s foo`  

Provided asg-scrambler is in your $PATH, you can then use one of the following commands:

`cat yourfile | asg-scrambler -p $foo > yourfile.enc` or `asg-scrambler -p $foo < yourfile > yourfile.enc`  

To unscramble just repeat the above shown steps:  

`cat yourfile.enc | asg-scrambler -p $foo > yourfile` or `asg-scrambler -p $foo < yourfile.enc > yourfile`

typing `asg-scrambler -h` will show a very basic help text

Ideally, the password should be used only once. Furthermore, it is very easy to retrieve the keystream if you have both the original and the scrambled file.  
Therefore, the original and the scrambled file should be stored separately from each other.  
For demonstration purposes only - never ever use this code to encrypt confident or important data!  

## further development
Any suggestions for improvement are highly appreciated!  
## reference
https://en.wikipedia.org/wiki/Alternating_step_generator
