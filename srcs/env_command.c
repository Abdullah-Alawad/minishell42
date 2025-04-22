#include "../minishell.h"

int handle_env(t_env_list **env)
{
    while (*env)
    {
        printf("%s\n", (*env)->full);
        *env = (*env)->next;
    }
    return (0);
}