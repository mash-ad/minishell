/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wsh_parse_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ouar <oel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 11:36:56 by mashad            #+#    #+#             */
/*   Updated: 2021/03/24 17:25:35 by mashad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

extern int	is_and(const char *str, int p, int q_flag, int pipe)
{
	if (str[p] == AND && str[p - 1] != ESC && !q_flag)
		return (1);
	if (str[p] == PIPE && pipe == 1 && str[p - 1] != ESC && !q_flag)
		return (1);
	if (str[p] == SPACE && pipe == 2 && str[p - 1] != ESC && !q_flag)
		return (1);
	if (str[p] == INRID && pipe == 3 && str[p - 1] != ESC && !q_flag)
		return (1);
	if (str[p] == OUTRID && pipe == 3 && str[p - 1] != ESC && !q_flag)
		return (1);
	if (str[p] == OUTRID && str[p + 1] == OUTRID
		&& pipe == 3 && str[p - 1] != ESC && !q_flag)
		return (1);
	return (0);
}

char	*ft_lowerit(char *string)
{
	char	*str;
	int		c_j;

	c_j = 0;
	str = (char *)malloc(sizeof(char) * ft_strlen(string) + 1);
	while (string[c_j] != EOL)
	{
		str[c_j] = ft_tolower(string[c_j]);
		c_j++;
	}
	str[c_j] = EOL;
	return (str);
}

int	is_escaped(char *string, int pos)
{
	int	c_c;

	c_c = 0;
	pos--;
	while (pos >= 0)
	{
		if (string[pos] == ESC)
			c_c++;
		else
			break ;
		pos--;
	}
	if (c_c % 2 != 0)
		return (1);
	return (0);
}

extern int	wsh_scan_commands(char *new, const char *str, int pipe)
{
	static int	counter = INIT;
	int			scount;
	int			quote_flag;

	scount = INIT;
	quote_flag = 0;
	while (str[counter] == ' ')
		counter++;
	while (!is_and(str, counter, quote_flag, pipe) && str[counter] != EOL)
	{
		if (ft_isin(str[counter], "\'\"") && !is_escaped((char *)str, counter)
			&& str[counter - 1] != ESC && quote_flag == 0)
			quote_flag = 1;
		else if (ft_isin(str[counter], "\'\"")
			&& !is_escaped((char *)str, counter) && quote_flag == 1)
			quote_flag = 0;
		new[scount++] = str[counter++];
	}
	new[scount] = EOL;
	if (str[counter++] == EOL)
	{
		counter = 0;
		return (0);
	}
	return (1);
}

int	wsh_tokenizer(char cmd[][1024], char *string, int pipe)
{
	char	*token;
	int		i;

	i = 0;
	token = (char *)malloc(sizeof(char) * 1024);
	if (!token)
		return (ERROR);
	while (wsh_scan_commands(token, (const char *) string, pipe))
		ft_strlcpy((char *)cmd[i++], (const char *)token, ft_strlen(token) + 1);
	ft_strlcpy((char *)cmd[i++], (const char *)token, ft_strlen(token) + 1);
	cmd[i][0] = EOL;
	free(token);
	token = NULL;
	return (1);
}
