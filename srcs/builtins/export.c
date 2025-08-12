#include "../../includes/minishell.h"

int	ft_export(char **args, t_env **env)
{
	int	i;
	int	result;

	if (!args[1])
		return (print_sorted_env(*env));
	result = 0;
	i = 1;
	while (args[i])
	{
		if (ft_process_export_arg(args[i], env))
			result = 1;
		i++;
	}
	return (result);
}
