/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marihovh <marihovh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 05:01:16 by marihovh          #+#    #+#             */
/*   Updated: 2023/10/09 05:40:46 by marihovh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_com(t_token *com, t_token *tmp, t_token *stream)
{
	if (com && (com != stream || com->type == PIPE))
		com->next = tmp->next;
	if (tmp->next != NULL)
		tmp->next->prev = com;
	else
		com->next = NULL;
}

void	free_pip(int **pip, int pip_cnt)
{
	int	i;

	i = -1;
	while (++i < pip_cnt)
	{
		close(pip[i][1]);
		close(pip[i][0]);
		free(pip[i]);
	}
	free(pip);
}

void	write_here_doc(int fd, char *filename, t_envies *env)
{
	char	*line;
	char	*dol;
	char	*tmp;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if ((line == NULL || ft_strcmp(line, filename) == 0))
			break ;
		dol = ft_strchr(line, '$');
		if (dol)
		{
			tmp = ft_strdup(line);
			free(line);
			line = open_dol(dol, tmp, env);
			free(tmp);
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	close(fd);
}

int	my_helper(int fd)
{
	close(fd);
	return (2);
}

int	my_open(t_token **stream, t_envies *env, int *sign)
{
	int		fd;
	t_token	*re;
	char	*filename;

	*sign = 0;
	re = (*stream);
	filename = file_name((*stream));
	(*stream) = re;
	if (valid_name(filename) == 1)
		return (1);
	if ((*stream)->type == REDIR_IN || (*stream)->type == REDIR_SO)
		*sign = 1;
	fd = open_fd((*stream), filename, env);
	if (fd == -42)
	{
		free(filename);
		return (3);
	}
	if (!(*stream))
		return (my_helper(fd));
	free(filename);
	(*stream) = find_com(stream, fd, *sign);
	return (0);
}
