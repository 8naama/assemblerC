# Assembler in C
Code of assembler in C for University project.

The assembler translates assembly code from given `.as` files to an encrypted binary file and creates the below files in the proccess (`XXX` stands for the original filename without the `.as` extention):
- `XXX.am` - contains the original code with spreaded macros
- `XXX.en` - contains entry variables and their values
- `XXX.ex` - contains external variables and the lines (decimal addresses) were they are used in the code
- `XXX.ob` - the encoded binary file

## How to run the program
1. Make the binary file by running
``` shell
make 
```
2. Run the program
``` shell
./assembler <<PATH_TO_ASSEMBLY_FILE>>
```
**Note**: You can pass multiple file paths.
