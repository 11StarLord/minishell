#include "minishell.h"

void	ft_free(void *ptr_to_free)
{
	if (ptr_to_free)
		free(ptr_to_free);
	ptr_to_free = NULL;
}

void	ft_free_tokens(t_token *tokens)
{
	int		i;

	if (tokens == NULL)
		return ;
	i = 0;
	while (tokens[i].str)
	{
		ft_free(tokens[i].str);
		i++;
	}
	ft_free(tokens);
}

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i++])
		free(matrix[i]);
	free(matrix);
	matrix = NULL;
}