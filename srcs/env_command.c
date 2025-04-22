#include "../minishell.h"

int handle_env(t_env_list **env)
{
    t_env_list     *head;

    head = *env;
    while (head)
    {
        printf("%s=%s\n", head->key, head->data);
        head = head->next;
    }
    return (0);
}