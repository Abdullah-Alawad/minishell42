#include "minishell.h"
#include <stdio.h>

int main(int argc, char **argv, char **env) {
    // for (int i = 0; env[i] != NULL; i++) {
    //     printf("%s\n", env[i]);
    // }
    printf("%s\n", env[10]);
    return 0;
}
