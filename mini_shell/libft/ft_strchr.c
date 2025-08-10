#include "../includes/minishell.h"

char	*ft_strchr(const char *str, int character)
{
	char	ch;

	ch = (char)character;
	while (*str != '\0')
	{
		if (*str == ch)
			return ((char *)str);
		str++;
	}
	if (ch == '\0')
		return ((char *)str);
	return (NULL);
}