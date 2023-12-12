/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvikvam <patvikvam@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 09:19:41 by pvikvam           #+#    #+#             */
/*   Updated: 2023/12/12 09:53:29 by pvikvam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_read(int fd, char *str)
{
	char	buf[BUFFER_SIZE + 1];
	int		i;

	i = 1;
	while (!ft_strchr(str, '\n') && i != 0)
	{
		i = read(fd, buf, BUFFER_SIZE);
		if (i == -1)
			return (ft_free(str));
		buf[i] = '\0';
		str = ft_strjoin(str, buf);
		if (!str)
			return (NULL);
	}
	return (str);
}

int	ft_con(char *str)
{
	int	j;
	int	i;

	j = 0;
	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		j = i + 2;
	else if (!str[i])
		j = i + 1;
	return (j);
}

char	*ft_line(char *str)
{
	int		i;
	char	*line;

	if (!str[0] || !str)
		return (NULL);
	i = ft_con(str);
	line = (char *)malloc(i * sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		line[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
	{
		line[i] = str[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*ft_new_str(char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
		return (ft_free(str));
	new_str = malloc((ft_strlen(str) - i + 1) * sizeof(char));
	if (!new_str)
		return (ft_free(str));
	i++;
	while (str[i] != '\0')
		new_str[j++] = str[i++];
	new_str[j] = '\0';
	if (!new_str[0])
	{
		free(new_str);
		return (ft_free(str));
	}
	free(str);
	return (new_str);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*str;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	str = ft_read(fd, str);
	if (!str)
	{
		str = NULL;
		return (NULL);
	}
	line = ft_line(str);
	if (!line)
	{
		str = ft_free(str);
		return (NULL);
	}
	str = ft_new_str(str);
	return (line);
}
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd1, fd2, fd3;
    char *line;
    int line_number = 1;
	
    fd1 = open("text1.txt", O_RDONLY);
    if (fd1 == -1)
    {
        perror("Error opening text1.txt");
        return 1;
    }
    fd2 = open("text2.txt", O_RDONLY);
    if (fd2 == -1)
    {
        perror("Error opening text2.txt");
        close(fd1);
        return 1;
    }
    fd3 = open("text3.txt", O_RDONLY);
    if (fd3 == -1)
    {
        perror("Error opening text3.txt");
        close(fd1);
        close(fd2);
        return 1;
    }
    while ((line = get_next_line(fd1)) != NULL)
    {
        printf("File: text1.txt, Line %d: %s\n", line_number++, line);
        free(line);
    }
    while ((line = get_next_line(fd2)) != NULL)
    {
        printf("File: text2.txt, Line %d: %s\n", line_number++, line);
        free(line);
    }
    while ((line = get_next_line(fd3)) != NULL)
    {
        printf("File: text3.txt, Line %d: %s\n", line_number++, line);
        free(line);
    }
    close(fd1);
    close(fd2);
    close(fd3);

    return (0);
}
