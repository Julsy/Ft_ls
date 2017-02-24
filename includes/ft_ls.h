/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iiliuk <iiliuk@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/03 14:37:11 by iiliuk            #+#    #+#             */
/*   Updated: 2017/02/23 18:45:12 by iiliuk           ###   ########.fr       */
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

typedef struct	s_opts
{
	int			l;
	int			big_r;
	int			a;
	int			r;
	int			t;
}				t_opts;

typedef struct	s_file
{
	char		*name;
	struct stat	stats;
}				t_file;

int				file_exists(char *filename);
void			display_stats(t_file *f, t_file *p, t_opts *o, int *width);
void			print_total(t_list *folder);
void			list_sort(t_list *s, int (*f)(t_file *, t_file *, int), int o);
int				cmp_lex(t_file *p1, t_file *p2, int order);
int				cmp_time(t_file *p1, t_file *p2, int order);
void			get_width_and_spec(t_list *files, int width[7]);
void			display_args(t_list	*files, t_opts *opts, int size);
t_list			*open_dir(t_list *parent, t_list *files, t_opts *opts);
void			do_recursion(t_list	*entries, t_list *files, t_opts *opts);

#endif

// norm display_agrs.c and open_read_dir
// test again against correction sheet + dev + usr

/*
**  Program works as follows(not including handling errors):
**  - Read arguments and sort them.
**  - For the list of received files execute ft_ls.
**  - If there is only one file that isn't a directory, display that file.
**  - If there is one file that is a directory, pass entries to ft_ls.
**  - If there are many files, pass all to ft_ls.
**  - ft_ls:
**       1. If -a not specified and file isn't specified in the
**          command line, exclude ".*" files.
**       2. Gather info about each file and format it according to options.
**       3. Sort entries either by time (if -t specified) or
**          lexicographically by name.
**       4. If -r specified, reverse entries.
**       5. If a file isn't specified in the command line or
**          there are multiple file arguments, display name of file.// ?
**       6. Print formatted entries of the list. If file is a directory and
**          is specified in the command line, don't display it.
**       7. If -R specified or it is directory specified in the command line:
**             ~ For each directory collect entries.
**             ~ For each directory execute ft_ls for each entry
*/
