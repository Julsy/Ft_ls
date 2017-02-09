/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iiliuk <iiliuk@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/03 14:37:11 by iiliuk            #+#    #+#             */
/*   Updated: 2017/02/03 14:37:11 by iiliuk           ###   ########.fr       */
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

void				display_stats(char* file, t_opts *opts);
void				print_total(int count);

#endif


// fix time issues
// fix number of links
// rewrite qsort algo
// -a opt: add 'total' # of files