/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iiliuk <iiliuk@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/03 14:37:11 by iiliuk            #+#    #+#             */
/*   Updated: 2017/02/22 17:32:45 by iiliuk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H
# define SIX_MONTHS_AGO 15552000

# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <getopt.h>
# include <grp.h>
# include <pwd.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <sysexits.h>
# include <time.h>
# include <unistd.h>
# include <errno.h>
# include "libft.h"
# include "ft_printf.h"

typedef struct		s_opts
{
	int				l;
	int				R;
	int				a;
	int				r;
	int				t;
}					t_opts;

typedef struct		s_file
{
	char			*name;
	struct stat		stats;
}					t_file;

int					file_exists(char *filename);
void				display_stats(t_file *file, t_file *parent, t_opts *opts, int *width, int spec);
void				print_total(t_list *folder);
void				list_sort(t_list *start, int (*f)(t_file *, t_file *, int), int order);
int					cmp_lex(t_file *, t_file *, int order);
int					cmp_time(t_file *, t_file *, int order);
int					get_width_if_spec(t_list *files, int width[6]);

void	print_list(t_list *list);

#endif

// check correction pages

