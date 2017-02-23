#include "../includes/ft_ls.h"

static void		print_permissions(mode_t mode)
{
	ft_putchar((mode & S_IRUSR) ? 'r' : '-');
	ft_putchar((mode & S_IWUSR) ? 'w' : '-');
	if (mode & S_ISUID)
		ft_putchar((mode & S_IXUSR) ? 's' : 'S');
	else
		ft_putchar((mode & S_IXUSR) ? 'x' : '-');
	ft_putchar((mode & S_IRGRP) ? 'r' : '-');
	ft_putchar((mode & S_IWGRP) ? 'w' : '-');
	if (mode & S_ISGID)
		ft_putchar((mode & S_IXGRP) ? 's' : 'S');
	else
		ft_putchar((mode & S_IXGRP) ? 'x' : '-');
	ft_putchar((mode & S_IROTH) ? 'r' : '-');
	ft_putchar((mode & S_IWOTH) ? 'w' : '-');
	if (mode & S_ISVTX)
		ft_putchar((mode & S_IXUSR) ? 't' : 'T');
	else
		ft_putchar((mode & S_IXOTH) ? 'x' : '-');
}

static void		print_filetype(mode_t mode)
{
	if ((mode & S_IFMT) == S_IFREG)
		ft_putchar('-');
	else if ((mode & S_IFMT) == S_IFDIR)
		ft_putchar('d');
	else if ((mode & S_IFMT) == S_IFLNK)
		ft_putchar('l');
	else if ((mode & S_IFMT) == S_IFCHR)
		ft_putchar('c');
	else if ((mode & S_IFMT) == S_IFBLK)
		ft_putchar('b');
	else if ((mode & S_IFMT) == S_IFSOCK)
		ft_putchar('s');
	else if ((mode & S_IFMT) == S_IFIFO)
		ft_putchar('f');
}

static void		print_name_or_link(t_file *file, t_file *parent, mode_t mode)
{
	char		link_buf[1024];
	ssize_t		count;
	char		*path;

	if ((mode & S_IFMT) == S_IFLNK)
	{
		ft_bzero(link_buf, sizeof(link_buf));
		if (parent)
		{
			path = ft_strjoin(parent->name, "/");
			path = ft_strjoin(path, file->name);
		}
		else
			path = ft_strdup(file->name);
		count = readlink(path, link_buf, sizeof(link_buf));
		if (count >= 0)
		{
			link_buf[count] = '\0';
			ft_printf(" %s -> %s\n", file->name, link_buf);
			free(path);
		}
		else
		{
			free(path);
			perror("readlink");
			exit(EX_IOERR);
		}
	}
	else
		ft_printf(" %s\n", file->name);
}

static void		print_time(time_t mod_time)
{
	char	*str;
	char	*end;
	char	*start;
	time_t	ct;

	ct = time(NULL);
	if ((str = ctime(&mod_time)) == NULL)
		return ;
	start = str + 4;
	end = str + 10;
	*end = 0;
	ft_putstr(start);
	ft_putchar(' ');
	if (mod_time > ct || mod_time + SIX_MONTHS_AGO < ct)
	{
		start = str + 20;
		end = str + 24, ft_putchar(' ');
	}
	else
	{
		start = str + 11;
		end = str + 16;
	}
	*end = 0;
	ft_putstr(start);
}

void			print_total(t_list *folder)
{
	blkcnt_t blocks;

	blocks = 0;
	while (folder)
	{
		blocks += ((t_file *)folder->content)->stats.st_blocks;
		folder = folder->next;
	}
	ft_printf("total %zu\n", (blocks));
}

int				get_width_if_spec(t_list *files, int width[6])
{
	int	special;

	special = 0;	
	width[0] = 0;
	width[1] = 0;
	width[2] = 0;
	width[3] = 0;
	width[4] = 0;
	width[5] = 0;
	while (files)
	{
		width[0] = MAX(ft_strlen_num(((t_file *)files->content)->stats.st_nlink), width[0]);
		width[1] = MAX(ft_strlen((char *)getpwuid(((t_file *)files->content)->stats.st_uid)) + 1, width[1]);
		width[2] = MAX(ft_strlen((char *)getgrgid(((t_file *)files->content)->stats.st_gid)), width[2]);
		width[3] = MAX(ft_strlen_num(((t_file *)files->content)->stats.st_size), width[3]);
		width[4] = MAX(ft_strlen_num(((t_file *)files->content)->stats.st_rdev >> 24), width[4]);
		width[5] = MAX(ft_strlen_num(((t_file *)files->content)->stats.st_rdev & 0xFFFFFF), width[5]);
		if ((((t_file *)files->content)->stats.st_mode == S_IFBLK) || (((t_file *)files->content)->stats.st_mode == S_IFCHR))
			special = 1;
		files = files->next;
	}
	width[0]++;
	return (special);
}

void			display_stats(t_file *file, t_file *parent, t_opts *opts, int width[6], int spec)
{
	if (opts->l == 0)
	{
		ft_printf("%s\n", file->name);
		return ;
	}
	print_filetype(file->stats.st_mode);
	print_permissions(file->stats.st_mode);
	ft_printf(" %*d ", width[0], file->stats.st_nlink);
	ft_printf(spec ? "%-*s " : "%*s ", width[1], getpwuid(file->stats.st_uid)->pw_name);
	ft_printf(spec ? "%-*s " : "%*s ", width[2], getgrgid(file->stats.st_gid)->gr_name);
	if (spec)
	{
		if (file->stats.st_mode == S_IFBLK)
			ft_printf("%*s ", width[4], "");
		else
			ft_printf("%*d,", width[4], file->stats.st_rdev >> 24);
		ft_printf(" %*d", width[5], file->stats.st_rdev & 0xFFFFFF);
	}
	else
		ft_printf("% *lld ", width[3], file->stats.st_size);
	print_time(file->stats.st_mtime);
	print_name_or_link(file, parent, file->stats.st_mode);
}
