#include "minishell.h"

void	handle_sigin(int sig)
{
	char	buff[4029];

	(void)sig;
	if (g_pid == 0 || g_pid == -1)
	{
		getcwd(buff, 4029);
		ft_putstr_fd("\n\x1B[36mwsh\x1B[0m\x1B[34m :: \x1B[0m", 0);
		ft_putstr_fd("\x1B[32m", 0);
		ft_putstr_fd(buff, 0);
		ft_putstr_fd("\x1B[0m\x1B[31m » \x1B[0m", 0);
		g_status = 1;
		g_pid = -1;
	}
	else if (g_pid != 0)
	{
		write(1, "\n", 1);
		g_status = 130;
	}
	return ;
}

void	handle_quit(int sig)
{
	(void)sig;
	if (g_pid != 0)
	{
		ft_putstr_fd("Quit: 3\n", 1);
		g_status = 131;
	}
	return ;
}

void	*wsh_fillargs(t_wsh_list *wsh_list, t_wsh_tokens *wsh_token,
			char wsh_args[][1024], int *position)
{
	int		counter;

	counter = 0;
	if (wsh_args[*position][0] != '-')
		return (NULL);
	wsh_token->wsh_arg = (char **)malloc(sizeof(char *) * 1024);
	if (!wsh_token->wsh_arg)
		return (NULL);
	while (wsh_args[*position][0] == '-' && wsh_args[*position][0] != EOL)
		wsh_token->wsh_arg[counter++] = wsh_escape(wsh_list, wsh_args[(*position)++]);
	if (wsh_args[*position][0] == '-' && wsh_args[*position][0] != EOL)
		wsh_token->wsh_arg[counter++] = wsh_escape(wsh_list, wsh_args[(*position)++]);
	wsh_token->wsh_arg[counter] = 0;
	return (NULL);
}

int	expo(char *param, char **envs, char **c_var, int *c_j)
{
	int	c_p;

	c_p = 0;
	if (wsh_findeq(param))
	{
		*c_var = ft_substr(param, 0, wsh_findeq(param));
		c_p = wsh_searchenvx(envs, *c_var);
		if (c_p && param)
			wsh_removevarandadd(envs, param, c_p);
		else
		{
			envs[(*c_j)++] = ft_strdup(param);
			envs[*c_j] = NULL;
		}
	}
	if (wsh_expo(param, envs, c_var, c_j))
		return (1);
	if (*c_var != NULL && *c_var[0] != '\0')
		wsh_free(*c_var);
	return (0);
}
