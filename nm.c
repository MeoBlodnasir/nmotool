/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 13:53:53 by aduban            #+#    #+#             */
/*   Updated: 2017/01/16 18:40:43 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	print_list(t_elem *elems)
{
	t_elem *tmp;
	tmp = elems;
	while (tmp)
	{
		if (tmp->type != 'U')
			ft_printf("%016x %c %s\n", tmp->value, tmp->type, tmp->str);
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
			if (array[i].n_value != 0)
				elem->type = 'C';
		}
		else if (t == N_ABS)
			elem->type = 'A';
		else if (t == N_INDR)
			elem->type = 'I';
		else
			elem->type = '?';
		elem->str = stringtable + array[i].n_un.n_strx;
		if ((array[i].n_type & N_EXT) == 0 && elem->type != '?')
			elem->type = ft_tolower(elem->type);
		if ((t & N_STAB) != 0 || elem->type == '?' || !ft_strcmp(elem->str, "") || elem->type == 'u')
		{
			free(elem);
			continue ;
		}
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

t_sect *	add_section(struct segment_command_64 *lc, t_sect *sects, int mark)
{
	struct section_64			*sec;
	t_sect *section;
	static int n = 1;
	if (mark)
	{
		n = 1;
		return (NULL);
	}
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
			sects = add_section(lc, sects, 0);
		}
		lc = (void*)lc + lc->cmdsize;
	}
	add_section(lc, sects, 1);

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

uint32_t	swap(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

void handle_fat(char *ptr)
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
	return size;

}

/*
t_offlist *add_object(t_offlist *lst, uint32_t off, uint32_t strx)
{
	t_offlist *new;
	new = malloc(sizeof(t_offlist));
	new->off = off;
	new->strx = strx;
	new->next = NULL;
	if (!lst)
	{
		lst = new;
		return (lst);
	}
	else
	{
		t_offlist *tmp = lst;
		while (tmp->next)
		{
			if (tmp->off == off)
				return (lst);
			tmp = tmp->next;
		}
		if (tmp->off != off)
			tmp->next = new;
	}
	return (lst);
}
void	print_objects(struct s_offlist *lst, char *ptr, char *file)
{
	struct ar_hdr	*arch;
	arch = (void*)ptr + SARMAG;
	struct s_offlist *tmp = lst;
	while (tmp)
	{
		arch = (void*)ptr + tmp->off;
		ft_printf("\n%s(%s):\n", file, ft_strstr(arch->ar_name , ARFMAG) + ft_strlen(ARFMAG));
		nm((char*)(arch + 1) + get_name_size(arch->ar_name) , NULL, 0);
		tmp = tmp->next;
	}
}

t_offlist *sort_objects(t_offlist *lst)
{
	t_offlist *tmp = lst;
	t_offlist *tmp2 = malloc(sizeof(t_offlist));
	t_offlist *other = lst;

	while (other)
	{
		tmp = lst;
		while (tmp)
		{
			if (tmp->next && tmp->off > tmp->next->off)
			{
				tmp2->off = tmp->off;
				tmp2->strx = tmp->strx;

				tmp->off = tmp->next->off;
				tmp->strx = tmp->next->strx;

				tmp->next->off = tmp2->off;
				tmp->next->strx = tmp2->strx;
			}
			tmp = tmp->next;
		}
		other = other->next;
	}
	free(tmp2);

	return (lst);
}
*/
void	handle_archive(char *ptr, char *file, uint32_t file_size)
{
	struct ar_hdr	*arch;
	t_offlist *lst = NULL;



	arch = (void*)ptr + SARMAG;
	int name_size = get_name_size(arch->ar_name);
	arch = (void*)ptr + sizeof(*arch) + SARMAG + name_size;
	int total_size = *((int*)(arch));
	arch = (void*)ptr + sizeof(*arch) + SARMAG + name_size + sizeof(int) + total_size;
	int stringtablesize = *((int*)(arch));
	arch = (void*)ptr + sizeof(*arch) + SARMAG + name_size + sizeof(int) + total_size + sizeof(int)+  stringtablesize;
	struct ar_hdr *tmp = arch;
	while (tmp < (struct ar_hdr*)(file_size + (void*)ptr))
	{
//		lst = add_object(lst, ran[i].ran_off, ran[i].ran_un.ran_strx);
		ft_printf("\n%s(%s):\n", file, (char*)(ft_strstr(tmp->ar_name , ARFMAG) + ft_strlen(ARFMAG)));
		nm((char*)(tmp + 1) + get_name_size(tmp->ar_name) , NULL, 0, 0);
		tmp = (void*)tmp + ft_atoi(tmp->ar_size) + sizeof(struct ar_hdr);
	}
	/*
	exit(0);
	lst = sort_objects(lst);
	print_objects(lst, ptr, file);
	*/
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

int	handle_file(char *file, int multiple)
{
	char		*ptr;
	int			fd;
	struct stat	buf;
	if ((fd = open(file, O_RDONLY)) < 0)
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
	nm(ptr, file, buf.st_size, multiple);
	if (munmap(ptr, buf.st_size) < 0)
	{
		ft_printf("unmap failed\n");
		exit(-1);
	}
	return (0);
}

int main(int ac, char **av)
{
	char *file;
	int multiple = 0;

	if (ac < 2)
	{
		file = ft_strdup("a.out");
		handle_file(file, multiple);
	}
	else
	{
		int i = 1;
		if (ac > 2)
			multiple = 1;
		while (i < ac)
		{
		//	if (ac > 2)
		//		ft_printf("%s:\n", av[i]);
			handle_file(av[i], multiple);
		/*	if (ac > 2 && i != ac - 1)
			{
				ft_printf("\n");
			}*/
			i++;
		}
	}


}
