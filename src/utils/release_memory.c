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

void	free_env(t_env *head)
{
	t_env	*tmp;

	if (!head)
		return ;
	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->key)
			ft_free(tmp->key);
		if (tmp->value)
			ft_free(tmp->value);
		ft_free(tmp);
	}
}