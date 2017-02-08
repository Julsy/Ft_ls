#include "../includes/ft_ls.h"

const char*			current_dir = ".";
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

t_opts		*get_opts(int argc, char **argv)
{
	int		i;
	int		j;
	t_opts	*opts;

	i = -1;
	opts = (t_opts *)ft_memalloc(sizeof(t_opts));
	while (++i < argc)
	{
		j = 0;
		while (argv[i][++j] != '\0' && argv[i][0] == '-')
		{
			if (argv[i][j] == 'l')
				opts->l = 1;
			else if (argv[i][j] == 'R')
				opts->R = 1;
			else if (argv[i][j] == 'a')
				opts->a = 1;
			else if (argv[i][j] == 'r')
				opts->r = 1;
			else if (argv[i][j] == 't')
				opts->t = 1;
			else if (argv[i][j] != 'l' && argv[i][j] != 'R'&& argv[i][j] != 'a' &&
			argv[i][j] != 'r' && argv[i][j] != 't') 
			{
				ft_putstr_fd("ls: illegal option -- ", 2);
				write(2, &argv[i][j], 1);
				ft_putstr_fd("\nusage: ls [-Ralrt] [file ...]\n", 2);
				exit(1);
			}
		}
	}
	return (opts);
}

void		display_dir_entries(char *dir, t_opts *opts)
{
	DIR				*folder;
	struct dirent	*file;

	folder = opendir(dir);
	while ((file = readdir(folder)) != NULL)
	{
		if (opts->a || file->d_name[0] != '.')
		{
			if (opts->l)
				display_stats(file->d_name, opts);
			else
				printf("%s\n", file->d_name);
		}
	}
	closedir(folder);
}

void		scan_dirs(int argc, char **argv, t_opts *opts)
{
	int			i;
	struct stat	stats;

	i = 0;
	//printf("l: %i, R: %i, a: %i, r: %i, t: %i\n", opts->l, opts->R, opts->a, opts->r, opts->t);
	while (++i < argc)
	{
		if (ft_strchr(&argv[i][0], '-') == NULL)
		{
			if (!(stat(argv[i], &stats) == 0 && S_ISDIR(stats.st_mode)))
				printf("%s\n", argv[i]); // not a dir
			if (stat(argv[i], &stats) == 0 && S_ISDIR(stats.st_mode))
			{
				printf("%s:\n", argv[i]); // multiple dirs
				display_dir_entries(argv[i], opts);
			}
		}
	}
}

int			main(int argc, char **argv)
{
	scan_dirs(argc, argv, get_opts(argc, argv));
}