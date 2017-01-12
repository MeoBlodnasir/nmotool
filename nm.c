/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 13:53:53 by aduban            #+#    #+#             */
/*   Updated: 2017/01/12 19:10:00 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	print_list(t_elem *elems)
{
	t_elem *tmp;
	tmp = elems;
	while (tmp)
	{
		if (tmp->value != 0)
		ft_printf("%.16x %c %s\n", tmp->value, tmp->type, tmp->str);
		else
		ft_printf("                 %c %s\n",tmp->type, tmp->str);
		tmp = tmp->next;
	}
}

char get_corresponding_sect(uint8_t nsect, t_sect *sects)
{
	t_sect *tmp = sects;
	while (tmp)
	{
		if (tmp->i == nsect)
		{
			if (!ft_strcmp(tmp->name, SECT_DATA))
				return ('D');
			else if (!ft_strcmp(tmp->name, SECT_BSS))
				return ('B');
			else if (!ft_strcmp(tmp->name, SECT_TEXT))
				return ('T');
			else
				return ('S');
		}
		tmp = tmp->next;
	}
	return '?';
}

t_elem *sort_elems(t_elem *elems)
{
	t_elem *tmp = elems;
	t_elem *tmp2 = malloc(sizeof(t_elem));
	t_elem *other = elems;

	while (other)
	{
		tmp = elems;
		while (tmp)
		{
			if (tmp->next && ft_strcmp(tmp->str, tmp->next->str) > 0)
			{
				tmp2->value = tmp->value;
				tmp2->type = tmp->type;
				tmp2->str = tmp->str;

				tmp->value = tmp->next->value;
				tmp->type = tmp->next->type;
				tmp->str = tmp->next->str;

				tmp->next->value = tmp2->value;
				tmp->next->type = tmp2->type;
				tmp->next->str = tmp2->str;
			}
			tmp = tmp->next;
		}
		other = other->next;
	}
	free(tmp2);

	return (elems);
}

void	fill_list(int nsyms, int symoff, int stroff, char *ptr, t_sect *sects)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array;
	t_elem *elems = NULL;
	t_elem *elem;
	t_elem *tmp;

	array = (void*)ptr + symoff;
	stringtable = (void*)ptr + stroff;
	i = -1;
	while (++i < nsyms)
	{
		elem = malloc(sizeof(t_elem));
		int t = (uint8_t)(array[i].n_type) & N_TYPE;
		if (((uint8_t)(array[i].n_sect)) != NO_SECT && t == N_SECT)
			elem->type = get_corresponding_sect(((uint8_t)(array[i].n_sect)), sects);
		else if (t == N_PBUD)
			elem->type = 'U';
		else if (t == N_UNDF)
		{
			elem->type = 'U';
			//sometimes C if value if set ( ??? )
		}
		else if (t == N_ABS)
			elem->type = 'A';
		else if (t == N_INDR)
			elem->type = 'I';
		else
			elem->type = '?';
		elem->str = stringtable + array[i].n_un.n_strx;
		if ((t & N_STAB) != 0 || elem->type == '?' || !ft_strcmp(elem->str, ""))
		{
			free(elem);
			continue ;
		}
		if ((array[i].n_type & N_EXT) == 0 && elem->type != '?')
			elem->type = ft_tolower(elem->type);
		elem->value = array[i].n_value;
		elem->next = NULL;
		if (elems == NULL)
		{
			elems = elem;
			elems->next = NULL;
			elems->prev = NULL;
		}
		else
		{
			tmp = elems;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = elem;
			tmp->next->prev = tmp;
		}
	}

	elems = sort_elems(elems);

	print_list(elems);
}

t_sect *	add_section(struct segment_command_64 *lc, t_sect *sects)
{
	struct section_64			*sec;
	t_sect *section;
	static int n = 1;
	sec = (struct section_64*)(lc + sizeof(lc) / sizeof(void*));
	int i = 0;
	while (i < lc->nsects)
	{
		section = malloc(sizeof(t_sect));
		section->name = (char*)(sec->sectname);
		section->i = n;
		n++;
		section->next = NULL;
		if (!sects)
			sects = section;
		else
		{
			t_sect *tmp = sects;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = section;
		}
		sec++;
		i++;
	}
	return sects;


}

t_sect *	get_sections(char *ptr, int ncmds, struct segment_command_64 *lc)
{
	t_sect *sects = NULL;

	int i = -1;
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			sects = add_section(lc, sects);
		}
		lc = (void*)lc + lc->cmdsize;
	}

	return sects;
}

void	handle_64(char *ptr)
{
	int						ncmds;
	int						i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(*header);
	i = 0;


	t_sect *sects = get_sections(ptr, ncmds, (struct segment_command_64*)lc);

	while (++i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command*) lc;
			fill_list(sym->nsyms, sym->symoff, sym->stroff, ptr, sects);
			return ;
		}
		lc = (void*)lc + lc->cmdsize;
	}
}



void	nm(void *ptr)
{
	unsigned int		number;

	number = *(int *)ptr;
	if (number == MH_MAGIC_64)
	{
		handle_64(ptr);
	}
	else if (number == FAT_MAGIC || number == FAT_CIGAM)
	{
		//go_fat(ptr);
	}
	else if (!ft_strncmp(ptr, ARMAG, SARMAG))
	{
		//go_archive(ptr, name);
	}
	else
		ft_putendl("Wrong binary format");

}

int main(int ac, char **av)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		ft_printf("Error opening binary\n");
		exit(-1);
	}
	if (fstat(fd, &buf) < 0)
	{
		ft_printf("fstat failed\n");
		exit(-1);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE,fd, 0)) == MAP_FAILED)
	{
		ft_printf("mmap failed\n");
		return(-1);
	}
	nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
	{
		ft_printf("unmap failed\n");
		exit(-1);
	}

}
