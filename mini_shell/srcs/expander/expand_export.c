#include "../../includes/minishell.h"

static char    *build_line_from_env(t_env *env, global_struct *global_struct)
{
    char    *line;
    char    *temp;

    if (!env || !global_struct)
        return (free_environment(&env), NULL);
    temp = ft_strdup(env->key);
    line = ft_strjoin(env->key, env->value, 0);
    env->key = temp;
    return (free_environment(&env), line);
}

static char *remove_and_add_quotes(char *value)
{
    char *result;
    int i = 0;
    int j = 0;

    if (!value)
        return (NULL);
    result = remove_quotes(value);
    free(value);
    if (!result)
        return (NULL);
    value = malloc(sizeof(char) * (ft_strlen(result) + 3));
    value[i++] = '"';
    while (result[j])
        value[i++] = result[j++];
    value[i++] = '"';
    value[i] = '\0';
    free(result);
    return (value);
}
static char    *expand_env_value(char *value, int should_split, global_struct *global_struct, int quote_state)
{
    char    *result;
    char    **arr;
    int     i;

    result = expand(value, global_struct, quote_state);
    if (!result)
        return (free(value), NULL);
    i = 0;
    arr = NULL;
    if (should_split)
    {
        arr = line_to_args(result);
        free(result);
        if (!arr)
            return (NULL);
        result = NULL;
        while (arr[i])
            result = ft_strjoin(result, arr[i++], ' ');
    }
    else
        result = remove_and_add_quotes(result);
    return (free_args(arr, i), result);
}

int handle_quote_state(char *str)
{
    int i;
    int quote_state;

    if (!str)
        return (0);
    i = 0;
    quote_state = NO_QUOTE;
    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
            update_quote_state(str[i], &quote_state);
        i++;
    }
    return (quote_state);
}

static char *expand_arg(char *arg, int should_split, global_struct *global_struct)
{
    t_env   *env;
    char    *temp;
    int     should_split_value;

    if (!arg || !global_struct)
        return (NULL);
    env = create_node(arg, 1);
    if (!env)
        return (NULL);
    temp = env->key;
    should_split_value = should_split || (check_quotes_type(env->key) != NO_QUOTE);
    env->key = expand(temp, global_struct, NO_QUOTE);
    free(temp);
    if (!env->key)
        return (free_environment(&env), NULL);
    temp = env->value;
    env->value = expand_env_value(temp, should_split_value, global_struct, handle_quote_state(env->key));
    free(temp);
    return (build_line_from_env(env, global_struct));
}

char *expand_export(char *line, global_struct *global_struct)
{
    int     should_split;
    char    *expanded;
    char    *result;
    char    **arr;
    int     i;

    if (!line || !global_struct)
        return (NULL);
    arr = line_to_args(line);
    if (!arr)
        return (NULL);
    should_split = (check_quotes_type(arr[0]) != NO_QUOTE);
    i = 0;
    result = NULL;
    while (arr[i])
    {
        expanded = expand_arg(arr[i], should_split, global_struct);
        if (!expanded)
            return (free_args(arr, i), NULL);
        result = ft_strjoin(result, expanded, ' ');
        free(expanded);
        if (!result)
            return (free_args(arr, i), NULL);
        i++;
    }
    return (free_args(arr, i),result);
}
