# HLang, A Toy Compiler

This project is yet another toy compiler project.

For the front end I am writing a Lexer, Parser, and intemediate code generator in C.

The backend will also be written in C. My code generator will generate x86_64 assembly.
For now I will be using the GNU Assembler to generate machine code. Who knows, I may get around to writing an assembler myself.

Example of the lexer so far:
```console
$ cat helloWorld.hl
message = "hello, world!";
print message;
$
$ ./hi helloWorld.hl
<TOK_IDENT, "message">
<=>
<TOK_STR_LIT, "hello, world!">
<;>
<TOK_KEYWORD, "print">
<TOK_IDENT, "message">
<TOK_EOF>
$
```

