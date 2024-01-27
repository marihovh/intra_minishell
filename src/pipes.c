/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marihovh <marihovh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:40:12 by marihovh          #+#    #+#             */
/*   Updated: 2023/10/09 05:41:40 by marihovh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pip_signal(int status, int last_pid)
{
	int	pid;

	while (1)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (pid == last_pid)
			g_exit_statuss = status / 256;
	}
	if (WIFSIGNALED(status))
	{
		g_exit_statuss = WTERMSIG((status));
		if (g_exit_statuss == SIGINT)
			write(1, "\n", 1);
	}
}

int	exec_2(t_data *data, t_token *stream, t_command *com_stream, int **pip)
{
	pid_t	f;

	under_(data);
	f = fork();
	if (f == 0)
	{
		signals();
		if (is_built_in(com_stream))
		{
			if (stream && stream->type == PIPE)
				while (stream && stream->type == WORD)
					stream = stream->next;
			built_in(com_stream, data, data->envies);
		}
		else
		{
			free_pip(pip, data->pip_cnt);
			ft_run(data);
		}
		exit(g_exit_statuss);
	}
	if (stream && stream->type == PIPE)
		while (stream && stream->type == WORD)
			stream = stream->next;
	return (f);
}

int	**init_pipe(t_data *data)
{
	int	**pip;
	int	i;

	pip = malloc((sizeof(int *)) * data->pip_cnt);
	i = -1;
	while (++i < data->pip_cnt)
	{
		pip[i] = malloc(sizeof(int) * 2);
		if (pipe(pip[i]) == -1)
		{
			printf("Pipe error\n");
			return (0);
		}
	}
	return (pip);
}

int	dup_and_exec(t_data *data, int **pip)
{
	int	last_pid;

	dups(data->com_stream, pip, data);
	last_pid = exec_2(data, data->stream, data->com_stream, pip);
	dup2(data->in_c, 0);
	dup2(data->out_c, 1);
	return (last_pid);
}

int	piping(t_data *data)
{
	int	**pip;
	int	status;
	int	last_pid;

	status = 0;
	pip = init_pipe(data);
	if (!pip)
		return (0);
	data->index = 0;
	while (data->index <= data->pip_cnt && data->com_stream->command)
	{
		if (data->com_stream->command[0])
		{
			last_pid = dup_and_exec(data, pip);
			if (data->index < data->pip_cnt)
				close(pip[data->index][1]);
			while (data->stream && data->stream->type != PIPE)
				data->stream = data->stream->next;
		}
		data->index++;
		data->com_stream = data->com_stream->next;
	}
	free_pip(pip, data->pip_cnt);
	pip_signal(status, last_pid);
	return (0);
}
