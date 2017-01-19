/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 13:53:53 by aduban            #+#    #+#             */
/*   Updated: 2017/01/19 17:13:51 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	print_list(t_elem *elems)
{
	t_elem	*tmp;

	tmp = elems;
	while (tmp)
	{
		if (tmp->type != 'U')
			ft_printf("%016x %c %s\n", tmp->value, tmp->type, tmp->str);
		else
			ft_printf("                 %c %s\n", tmp->type, tmp->str);
		tmp = tmp->next;
	}
}

char	get_corresponding_sect(uint8_t nsect, t_sect *sects)
{
	t_sect *tmp;

	tmp = sects;
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
	return ('?');
}

void	switch_elems(t_elem **tmp)
{
	t_elem *tmp2;

	tmp2 = malloc(sizeof(t_elem));
	tmp2->value = (*tmp)->value;
	tmp2->type = (*tmp)->type;
	tmp2->str = (*tmp)->str;
	(*tmp)->value = (*tmp)->next->value;
	(*tmp)->type = (*tmp)->next->type;
	(*tmp)->str = (*tmp)->next->str;
	(*tmp)->next->value = tmp2->value;
	(*tmp)->next->type = tmp2->type;
	(*tmp)->next->str = tmp2->str;
}

t_elem	*sort_elems(t_elem *elems)
{
	t_elem *tmp;
	t_elem *tmp2;
	t_elem *other;

	tmp = elems;
	tmp2 = malloc(sizeof(t_elem));
	other = elems;
	while (other)
	{
		tmp = elems;
		while (tmp)
		{
			if (tmp->next && ft_strcmp(tmp->str, tmp->next->str) > 0)
				switch_elems(&tmp);
			tmp = tmp->next;
		}
		other = other->next;
	}
	free(tmp2);
	return (elems);
}

char	get_elem_type(struct nlist_64 *array, int i, int t)
{
	if (t == N_PBUD)
		return ('U');
	else if (t == N_UNDF)
	{
		if (array[i].n_value != 0)
			return ('C');
		else
			return ('U');
	}
	else if (t == N_ABS)
		return ('A');
	else if (t == N_INDR)
		return ('I');
	else
		return ('?');
}

t_elem	*add_elem(t_elem *elems, t_elem *elem)
{
	t_elem *tmp;

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
	return (elems);
}

t_elem	*fill_elem(struct nlist_64 *array, int i, t_sect *sects,
		char *stringtable)
{
	t_elem	*elem;
	int		t;

	elem = malloc(sizeof(t_elem));
	t = (uint8_t)(array[i].n_type) & N_TYPE;
	if (((uint8_t)(array[i].n_sect)) != NO_SECT && t == N_SECT)
		elem->type =
			get_corresponding_sect(((uint8_t)(array[i].n_sect)), sects);
	else
		elem->type = get_elem_type(array, i, t);
	elem->str = stringtable + array[i].n_un.n_strx;
	if ((array[i].n_type & N_EXT) == 0 && elem->type != '?')
		elem->type = ft_tolower(elem->type);
	if ((t & N_STAB) != 0 || elem->type == '?' ||
			!ft_strcmp(elem->str, "") || elem->type == 'u')
	{
		free(elem);
		return (NULL);
	}
	return (elem);
}

void	fill_list(struct symtab_command *sym, char *ptr, t_sect *sects)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array;
	t_elem			*elem;
	t_elem			*elems;

	elems = NULL;
	array = (void*)ptr + sym->symoff;
	stringtable = (void*)ptr + sym->stroff;
	i = -1;
	while (++i < sym->nsyms)
	{
		elem = fill_elem(array, i, sects, stringtable);
		if (!elem)
			continue ;
		elem->value = array[i].n_value;
		elem->next = NULL;
		elems = add_elem(elems, elem);
	}
	elems = sort_elems(elems);
	print_list(elems);
}

t_sect	*add_sec_tolist(int n, char *name, t_sect *sects)
{
	t_sect	*section;
	t_sect	*tmp;

	section = malloc(sizeof(t_sect));
	section->name = name;
	section->i = n;
	section->next = NULL;
	if (!sects)
		sects = section;
	else
	{
		tmp = sects;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = section;
	}
	return (sects);
}

t_sect	*add_section(struct segment_command_64 *lc, t_sect *sects, int mark)
{
	struct section_64	*sec;
	static int			n = 1;
	int					i;

	if (mark)
	{
		n = 1;
		return (NULL);
	}
	sec = (struct section_64*)(lc + sizeof(lc) / sizeof(void*));
	i = 0;
	while (i < lc->nsects)
	{
		sects = add_sec_tolist(n, (char*)sec->sectname, sects);
		n++;
		sec++;
		i++;
	}
	return (sects);
}

t_sect	*get_sections(char *ptr, int ncmds, struct segment_command_64 *lc)
{
	t_sect	*sects;
	int		i;

	sects = NULL;
	i = -1;
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			sects = add_section(lc, sects, 0);
		lc = (void*)lc + lc->cmdsize;
	}
	add_section(lc, sects, 1);
	return (sects);
}

void	handle_64(char *ptr)
{
	t_norm					norm;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	t_sect					*sects;

	header = (struct mach_header_64 *)ptr;
	norm.ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(*header);
	norm.i = 0;
	sects = get_sections(ptr, norm.ncmds, (struct segment_command_64*)lc);
	while (++norm.i < norm.ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command*)lc;
			fill_list(sym, ptr, sects);
			return ;
		}
		lc = (void*)lc + lc->cmdsize;
	}
}

uint32_t	swap(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

void	handle_fat(char *ptr)
{
	struct fat_header	*fathead;
	struct fat_arch		*arch;
	uint32_t			i;
	uint32_t			offset;

	fathead = (void*)ptr;
	i = fathead->nfat_arch;
	i = swap(i);
	arch = (void*)ptr + sizeof(fathead);
	while (i)
	{
		if (swap(arch->cputype) == CPU_TYPE_X86_64)
		{
			offset = arch->offset;
			nm(ptr + swap(offset), NULL, 0, 0);
			return ;
		}
		arch += sizeof(arch) / sizeof(void*);
		i--;
	}
}

int		get_name_size(char *name)
{
	int size;

	size = ft_atoi(ft_strchr(name, '/') + 1);
	return (size);
}

void	handle_archive(char *ptr, char *file, uint32_t file_size)
{
	struct ar_hdr	*arch;
	t_offlist		*lst;
	int				size;
	struct ar_hdr	*tmp;

	lst = NULL;
	arch = (void*)ptr + SARMAG;
	size = get_name_size(arch->ar_name);
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size;
	size += *((int*)(arch));
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size + sizeof(int);
	size += *((int*)(arch));
	arch = (void*)ptr + sizeof(*arch) + SARMAG + size +
		sizeof(int) + sizeof(int);
	tmp = arch;
	while (tmp < (struct ar_hdr*)(file_size + (void*)ptr))
	{
		ft_printf("\n%s(%s):\n", file, (char*)(ft_strstr(tmp->ar_name,
						ARFMAG) + ft_strlen(ARFMAG)));
		nm((char*)(tmp + 1) + get_name_size(tmp->ar_name), NULL, 0, 0);
		tmp = (void*)tmp + ft_atoi(tmp->ar_size) + sizeof(struct ar_hdr);
	}
}

void	nm(void *ptr, char *file, uint32_t file_size, int multiple)
{
	unsigned int		number;

	number = *(int *)ptr;
	if (number == MH_MAGIC_64)
	{
		if (multiple)
			ft_printf("\n%s:\n", file);
		handle_64(ptr);
	}
	else if (number == FAT_MAGIC || number == FAT_CIGAM)
	{
		handle_fat(ptr);
	}
	else if (!ft_strncmp(ptr, ARMAG, SARMAG))
	{
		handle_archive(ptr, file, file_size);
	}
	else
		ft_putendl("Wrong binary format");
}

char	*get_ptr(char *ptr, char *file, struct stat *buf)
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

int	handle_file(char *file, int multiple)
{
	char		*ptr;
	struct stat	buf;

	ptr = get_ptr(ptr, file, &buf);
	if (!ptr)
		exit(0);
	nm(ptr, file, buf.st_size, multiple);
	if (munmap(ptr, buf.st_size) < 0)
	{
		ft_printf("unmap failed\n");
		exit(-1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	char	*file;
	int		multiple;
	int		i;

	multiple = 0;
	if (ac < 2)
	{
		file = ft_strdup("a.out");
		handle_file(file, multiple);
	}
	else
	{
		i = 1;
		if (ac > 2)
			multiple = 1;
		while (i < ac)
		{
			handle_file(av[i], multiple);
			i++;
		}
	}
}
