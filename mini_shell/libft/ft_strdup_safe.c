#include "../includes/minishell.h"

char	*ft_strdup_safe(const char *s)
{
	size_t	len;
	char	*d;

	if (!s)
		return (NULL);
	len = strlen(s) + 1;
	d = malloc(len);
	if (d)
		ft_memcpy(d, s, len);
	return (d);
}