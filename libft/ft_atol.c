#include "../includes/minishell.h"

long	ft_atol(char *str)
{
	int		i;
	long	nbr;

	i = 0;
	nbr = 0;
	if (!str)
		return (0);
	while (is_space(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		nbr = nbr * 10 + (str[i++] - '0');
		if (nbr > INT_MAX)
			return (-1);
	}
	while (is_space(str[i]))
		i++;
	if (str[i] != '\0')
		return (0);
	return (nbr);
}
