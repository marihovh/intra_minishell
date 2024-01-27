/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marihovh <marihovh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 11:08:50 by marihovh          #+#    #+#             */
/*   Updated: 2023/10/09 05:41:06 by marihovh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	fd_error(int sign, char *filename)
{
	ft_putstr_fd("shyshell: ", 2);
	ft_putstr_fd(filename, 2);
	if (sign == 0)
		ft_putstr_fd(": Can't open file\n", 2);
	else
		ft_putstr_fd(": No such file or directory\n", 2);
	g_exit_statuss = 1;
	free(filename);
	return (-42);
}

int	open_fd(t_token *stream, char *filename, t_envies *env)
{
	int	fd;

	fd = 0;
	if (stream->type == REDIR_IN)
	{
		fd = open(filename, O_RDONLY);
		if (fd < 0)
			return (fd_error(1, filename));
	}
	else if (stream->type == REDIR_OUT && stream->next)
	{
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (-42);
	}
	else if (stream->type == REDIR_AP && stream->next)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (stream->type == REDIR_SO && stream->next)
	{
		fd = for_heredoc(filename, env);
		if (fd == -2)
			return (-42);
	}
	return (fd);
}

t_token	*set_coma(t_token *com, t_token *tmp, t_token *redir)
{
	if (com->next)
	{
		com->prev->next = com->next;
		com->next->prev = tmp;
	}
	else
		com->prev->next = NULL;
	if (redir->prev)
	{
		com->prev = redir->prev;
		redir->prev = com;
		com->next = redir;
	}
	else
	{
		redir->prev = com;
		com->next = redir;
	}
	return (com);
}

t_token	*try(t_token **stream)
{
	t_token	*com;
	t_token	*tmp;
	t_token	*redir;

	com = (*stream);
	redir = (*stream);
	tmp = *stream;
	if ((*stream)->next)
		tmp = (*stream)->next;
	if ((com)->prev)
		while (com && ((com)->type != PIPE && com->type != WORD \
			&& com->type != FIELD && com->type != EXP_FIELD))
			com = com->prev;
	soo_word(&tmp);
	if ((com && com->type == PIPE) || !com || com->op == 1)
	{
		com = NULL;
		if (tmp->next && tmp->next->type != PIPE)
			com = tmp->next;
		while (com && (com->type != WORD && com->type != PIPE))
			com = com->next;
		if (com)
			return (set_coma(com, tmp, redir));
	}
	return (*stream);
}

int	redirs(t_token **stream, t_envies *env)
{
	int		sign;
	t_token	*ret;
	int		flag;

	ret = (*stream);
	while (*stream != NULL)
	{
		if ((*stream)->op == 1)
		{
			flag = my_open(stream, env, &sign);
			if (flag == 1)
				return (1);
			else if (flag == 2)
				return (2);
			else if (flag == 3)
			{
				(*stream) = ret;
				return (3);
			}
		}
		(*stream) = (*stream)->next;
	}
	(*stream) = ret;
	return (0);
}
