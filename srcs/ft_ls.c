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

void	print_permissions(t_file *file)
{
	int			mode;

	mode = file->stats.st_mode;
	ft_printf((mode & S_IRUSR) ? "r" : "-");
	ft_printf((mode & S_IWUSR) ? "w" : "-");
	if (mode & S_ISUID)
		ft_printf((mode & S_IXUSR) ? "s" : "S");
	else
		ft_printf((mode & S_IXUSR) ? "x" : "-");
	ft_printf((mode & S_IRGRP) ? "r" : "-");
	ft_printf((mode & S_IWGRP) ? "w" : "-");
	if (mode & S_ISGID)
		ft_printf((mode & S_IXGRP) ? "s" : "S");
	else
		ft_printf((mode & S_IXGRP) ? "x" : "-");
	ft_printf((mode & S_IROTH) ? "r" : "-");
	ft_printf((mode & S_IWOTH) ? "w" : "-");
	if (mode & S_ISVTX)
		ft_printf((mode & S_IXUSR) ? "t" : "T");
	else
		ft_printf((mode & S_IXOTH) ? "x" : "-");
}

void		display_dir_entries(t_list *parent, t_list *files, t_opts *opts)
{
	DIR				*folder;
	struct dirent	*file;
	t_file			*entry;
	t_list			*entries;
	t_list			*tmp_entries;
	struct stat		stats;
	size_t			count;
	size_t			i;
	int				a;
	char			*parent_folder;

	i = -1;
	a = 0;
	count = 0;
	entries = NULL;

	// printf("1st elem:\n");
	// print_list(files);

	// printf("parent: %s\n", ((t_file *)parent->content)->name);
	// printf("current: %s\n", ((t_file *)files->content)->name);
	if(!ft_strequ(((t_file *)parent->content)->name, ((t_file *)files->content)->name))
	{
		// printf("update dir: %s\n", ((t_file *)files->content)->name);
		parent_folder = ft_strjoin(((t_file *)parent->content)->name, ((t_file *)files->content)->name);
		((t_file *)files->content)->name=ft_strjoin(parent_folder, "/");
		// printf("to: %s\n", ((t_file *)files->content)->name);
	} else {
		((t_file *)files->content)->name=ft_strjoin(((t_file *)files->content)->name, "/");		
	}
	printf("%s:\n", ((t_file *)files->content)->name);
	// printf("2:%s\n",((t_file *)files->content)->name);
	folder = opendir(((t_file *)files->content)->name);
	entry = (t_file *)ft_memalloc(sizeof(t_file));
	// printf("3\n");
	file = readdir(folder);
	// printf("\n");
	while (file)
	{
		// printf("FILE NAME %s\n", file->d_name);
		if(opts->a || file->d_name[0] != '.')
		{
			// printf("4\n");
			entry->name = ft_strdup(file->d_name);
			stat(ft_strjoin(((t_file *)files->content)->name, file->d_name), &(entry->stats));
			ft_list_add_back(&entries, ft_lstnew(entry, sizeof(t_file)));			
		}
		file = readdir(folder);
	}
	free(entry);
	// printf("5\n");
	list_sort(entries, (opts->t) ? cmp_time : cmp_lex, opts->r);
	tmp_entries = entries;
	// printf("6\n");
	while (entries)
	{
		display_stats((t_file *)entries->content, opts);
		entries = entries->next;
	}
	entries = tmp_entries;
	printf("\n");
	// print_list(entries);
	if (opts->R)
	{
		while (entries)
		{
			//printf("go deep-> %i\n", ((t_file *)entries->content)->stats.st_mode);
			//print_permissions((t_file *)entries->content);
			if (S_ISDIR(((t_file *)entries->content)->stats.st_mode))
				display_dir_entries(files, entries, opts);
			entries = entries->next;
		}
	}
	// printf("done!\n");
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
		if (!file_exists(argv[i]) && !ft_strchr(&argv[i][0], '-'))
			ft_printf("ls: %s: No such file or directory\n", argv[i]);
		else if (ft_strchr(&argv[i][0], '-') == 0 && !ft_strequ(argv[i], "./ft_ls"))
		{
			//printf("ARGV: %s\n", argv[i]);
			tmp->name = ft_strdup(argv[i]);
			stat(tmp->name, &(tmp->stats));
			ft_list_add_back(&files, ft_lstnew(tmp, sizeof(t_file)));
		}
	}
	//print_list(files);
	if (!ft_list_size(files) && (!argv[1] || file_exists(".")))
	{
		tmp->name = ft_strdup(".");
		stat(tmp->name, &(tmp->stats));
		ft_list_add_back(&files, ft_lstnew(tmp, sizeof(t_file)));
	}
	else
		list_sort(files, (opts->t) ? cmp_time : cmp_lex, opts->r);
	//print_list(files);
	free(tmp);
	while (files)
	{
		if (S_ISDIR(((t_file *)files->content)->stats.st_mode))
		{
			ft_printf("\n%s:\n", ((t_file *)files->content)->name);
			if (opts->l)
				print_total(files);
			display_dir_entries(files, files, opts);
		}
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