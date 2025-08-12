#include "../includes/minishell.h"

static size_t	calculate_len(char *s1, char delimiter)
{
	if (delimiter)
		return (ft_strlen(s1) + 2);
	return (ft_strlen(s1) + 1);
}

static char	*create_single_char_str(char c)
{
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	return (ft_strdup(str));
}

char	*ft_strjoin_char(char *s1, char c, char delimiter)
{
	char	*dest;
	size_t	len;
	size_t	i;

	if (!s1)
		return (create_single_char_str(c));
	len = calculate_len(s1, delimiter);
	dest = malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (free(s1), NULL);
	i = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	if (delimiter)
		dest[i++] = delimiter;
	dest[i++] = c;
	dest[i] = '\0';
	return (free(s1), dest);
}
