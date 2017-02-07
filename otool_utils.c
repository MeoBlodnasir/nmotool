/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/19 18:29:32 by aduban            #+#    #+#             */
/*   Updated: 2017/02/07 16:01:17 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

void		print_otool(struct section_64 *sec, char *ptr)
{
	void	*start;
	void	*end;
	void	*p;
	int		i;

	start = (void*)ptr + sec->offset;
	end = start + sec->size;
	p = start;
	i = 0;
	handle_segv(NULL, 0, p);
	while (p < end)
	{
		ft_printf("%016llx\t", sec->addr + (p - start));
		i = -1;
		while (++i < 16 && p < end)
		{
			ft_printf("%02hhx ", *(char*)p);
			++p;
			handle_segv(NULL, 0, p);
		}
		ft_printf("\n");
	}
}

char		*ft_strchrsegv(const char *s, int c)
{
	int t;

	t = 0;
	if ((char)c == '\0')
		return ((char*)(s + ft_strlen(s)));
	handle_segv(NULL, 0, (void*)s + t);
	while (s[t] != (char)c && s[t])
	{
		t++;
		handle_segv(NULL, 0, (void*)s + t);
	}
	if (s[t] == (char)c)
		return ((char*)(s + t));
	return (0);
}

int			get_name_size(char *name)
{
	int		size;
	char	*str;

	str = ft_strchrsegv(name, '/') + 1;
	size = ft_atoi(str);
	if (size == 0)
		exit(0);
	return (size);
}

char		*get_ptr(char *ptr, char *file, struct stat *buf)
{
	int			fd;

	if ((fd = open(file, O_RDONLY)) < 0)
	{
		ft_printf("Error opening binary\n");
		return (NULL);
	}
	if (fstat(fd, buf) < 0)
	{
		ft_printf("fstat failed\n");
		return (NULL);
	}
	if ((ptr = mmap(0, buf->st_size, PROT_READ, MAP_PRIVATE, fd,
					0)) == MAP_FAILED)
	{
		ft_printf("mmap failed\n");
		return (NULL);
	}
	return (ptr);
}

void		handle_segv(void *ptr, int size, void *totest)
{
	static void	*start;
	static void	*end;

	if (ptr)
	{
		start = ptr;
		end = start + size;
		return ;
	}
	if (totest < ptr || totest >= end)
		exit(0);
}
