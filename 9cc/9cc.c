#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    if(argc !=2){
        fprintf(stderr, "ˆø”‚ÌŒÂ”‚ª³‚µ‚­‚ ‚è‚Ü‚¹‚ñ\n");
        return 1;
    }

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("  mov rax, %d\n", atoi(argv[1]));
    printf("  ret\n");
    return 0;
}