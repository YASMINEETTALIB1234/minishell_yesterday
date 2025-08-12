#include "../includes/minishell.h"

char	*ft_strrchr(const char *s, int c)
{
	char		ch;
	const char	*last_occr;

	last_occr = NULL;
	ch = (char)c;
	while (*s)
	{
		if (*s == ch)
			last_occr = s;
		s++;
	}
	if (ch == '\0')
		return ((char *)s);
	return ((char *)last_occr);
}
