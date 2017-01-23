/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 13:54:08 by aduban            #+#    #+#             */
/*   Updated: 2017/01/23 15:46:43 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>
# include "libft/libft.h"
# include <mach-o/ranlib.h>
# include <mach-o/fat.h>
# include <ar.h>

typedef struct			s_elem
{
	long int			value;
	char				type;
	char				*str;
	struct s_elem		*next;
	struct s_elem		*prev;
}						t_elem;
typedef struct			s_sect
{
	char				*name;
	int					i;
	struct s_sect		*next;
}						t_sect;

typedef struct			s_offlist
{
	uint32_t			off;
	uint32_t			strx;
	struct s_offlist	*next;
}						t_offlist;
typedef struct			s_norm
{
	int					ncmds;
	int					i;
}						t_norm;

void					nm(void *ptr, char *file, uint32_t
		file_size, int multiple);
char					get_corresponding_sect(uint8_t nsect,
		t_sect *sects);
char					get_elem_type(struct nlist_64 *array, int i, int t);
t_elem					*add_elem(t_elem *elems, t_elem *elem);
t_elem					*sort_elems(t_elem *elems);
t_sect					*get_sections(char *ptr, int ncmds, struct
		segment_command_64 *lc);
t_sect					*get_sections_32(char *ptr, int ncmds, struct
		segment_command *lc);
void					fill_list(struct symtab_command *sym, char
		*ptr, t_sect *sects);
int						get_name_size(char *name);
uint32_t				swap(uint32_t val);
void					handle_archive(char *ptr, char *file,
		uint32_t file_size);
void					handle_fat(char *ptr, int file_size, char *file);
char	get_elem_type_32(struct nlist *array, int i, int t);
t_sect	*get_sections_32(char *ptr, int ncmds, struct segment_command *lc);
t_sect	*add_sec_tolist(int n, char *name, t_sect *sects);
void		fill_list_32(struct symtab_command *sym, char *ptr, t_sect *sects);
int64_t		swap_endian_64(int64_t i);
int	set_swap(int i);
int	set_swap_fat(int i);
uint32_t		swap_32_fat(uint32_t i);
uint32_t		swap_32(uint32_t i);
int64_t		swap_64(int64_t i);
void	handle_64(char *ptr);
void	handle_32(char *ptr);
#endif
