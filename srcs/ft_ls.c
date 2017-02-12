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

void		display_dir_entries(t_list *files, t_opts *opts)
{
	DIR				*folder;
	struct dirent	*file;
	t_file			*entry;
	t_list			*entries;
	struct stat		stats;
	size_t			count;
	size_t			i;
	int				a;

	i = -1;
	a = 0;
	count = 0;
	entries = NULL;
	// if (!(stat(dir, &stats) == 0 && S_ISDIR(stats.st_mode)))
	// 	ft_printf("%s\n", dir); // not a dir
	// if (stat(argv[i], &stats) == 0 && S_ISDIR(stats.st_mode))
	// {
	// 	if (opts->l)
	// 		ft_printf("%s:\n", argv[i]); // multiple dirs
	// }
	folder = opendir(((t_file *)files->content)->name);//FIX FILE PROCESSING
	entry = (t_file *)ft_memalloc(sizeof(t_file));
	file = readdir(folder);
	while (file)
	{
		if(opts->a || file->d_name[0] != '.')
		{
			entry->name=ft_strdup(file->d_name);
			stat(entry->name, &(entry->stats));
			ft_list_add_back(&entries, ft_lstnew(entry, sizeof(t_file)));			
		}
		file = readdir(folder);
	}
	free(entry);
	// if (opts->l)
	// 	print_total();
	// 	while ((file = readdir(folder)) != NULL)
	// 	{
	// 		if ((stat(file->d_name, &stats) == 0 && S_ISDIR(stats.st_mode)
	// 			&& (opts->a || file->d_name[0] != '.')))
	// 				a++;
	// 	}
	// }
	// printf("%i\n", a);
	
	// if (opts->t)
	// 	qsort(entries, count, sizeof(char*), cmp_time);	
	// else
	// 	qsort(entries, count, sizeof(char*), cmp_lex);
	list_sort(entries, cmp_lex);
	while (entries)
	{
		display_stats((t_file *)entries->content, opts);
		entries = entries->next;
	}
	closedir(folder);
}

void	print_list(t_list *list)
{
	if (list)
	{
		printf("content: %s\n", ((t_file *)list->content)->name);
		print_list(list->next);
	}
}

void		scan_dirs(int argc, char **argv, t_opts *opts)
{
	int			i;
	t_list		*files;
	t_file		*tmp;

	i = 0;
	files = NULL;
	
	tmp = (t_file *)ft_memalloc(sizeof(t_file));
	
	//printf("l: %i, R: %i, a: %i, r: %i, t: %i\n", opts->l, opts->R, opts->a, opts->r, opts->t);
	while (++i < argc)
	{
		if (argv[i][0] == 0)
		{
			ft_putstr_fd("ls: ft_ls_open: No such file or directory\n", 2);
			exit(1);
		}
		if ((ft_strchr(&argv[i][0], '-') == 0 && !ft_strequ(argv[i], "./ft_ls")))
		{
			//printf("ARGV: %s\n", argv[i]);
			tmp->name=ft_strdup(argv[i]);
			stat(tmp->name, &(tmp->stats));
			ft_list_add_back(&files, ft_lstnew(tmp, sizeof(t_file)));
		}
	}
	free(tmp);
	//print_list(files);
	if (!ft_list_size(files))
	{
		tmp->name=ft_strdup(".");
		stat(tmp->name, &(tmp->stats));
		ft_list_add_back(&files, ft_lstnew(tmp, sizeof(t_file)));
	}
	else
		list_sort(files, cmp_lex);
	//print_list(files);
	while (files)
	{
		if (S_ISDIR(((t_file *)files->content)->stats.st_mode))
			display_dir_entries(files, opts);
		else
			display_stats((t_file *)files->content, opts);
		files = files->next;
	}
}

int			main(int argc, char **argv)
{
	scan_dirs(argc, argv, get_opts(argc, argv));
	return (0);
}