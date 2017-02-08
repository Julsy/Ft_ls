#include "../includes/ft_ls.h"

static void		print_permissions(mode_t mode)
{
	ft_putchar((mode & S_IRUSR) ? 'r' : '-');
	ft_putchar((mode & S_IWUSR) ? 'w' : '-');
	ft_putchar((mode & S_IXUSR) ? 'x' : '-');
	ft_putchar((mode & S_IRGRP) ? 'r' : '-');
	ft_putchar((mode & S_IWGRP) ? 'w' : '-');
	ft_putchar((mode & S_IXGRP) ? 'x' : '-');
	ft_putchar((mode & S_IROTH) ? 'r' : '-');
	ft_putchar((mode & S_IWOTH) ? 'w' : '-');
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

static void		print_name_or_link(char* file, mode_t mode)
{
	char		link_buf[512];
	ssize_t		count;

	if ((mode & S_IFMT) == S_IFLNK)
	{
		count = readlink(file, link_buf, sizeof(link_buf));
		if (count >= 0)
		{
			link_buf[count] = '\0';
			printf(" %s -> %s \n", file, link_buf);
		}
		else
		{
			perror("readlink");
			exit(EX_IOERR);
		}
	}
	else
		printf(" %s\n", file);
}

static void		print_time(time_t mod_time, struct stat stats)
{
	int		diff;
	char	*time_str;
	char	*temp;
	char	*temp2;

	diff = time(NULL) - mod_time;
	time_str = ctime(&mod_time);
	if (diff > SIX_MONTHS_AGO || diff < 0)
	{
		temp = ft_strsub(time_str, 4, 6);
		temp2 = ft_strsub(time_str, 20, 4);
		printf(" %s  %s", temp, temp2);
		free(temp2);
	}
	else
	{
		temp = ft_strsub(time_str, 4, 12);
		printf(" %s", temp);
	}
	free(temp);
}

void			display_stats(char* file, t_opts *opts)
{
	struct stat stats;

	stat(file, &stats);
	if (opts->l == 0)
	{
		printf("%s\n", file);
		return ;
	}
	print_filetype(stats.st_mode);
	print_permissions(stats.st_mode);
	printf("  %jd ", (intmax_t)stats.st_nlink);
	printf("%5s ", getpwuid(stats.st_uid)->pw_name);
	printf("%5s", getgrgid(stats.st_gid)->gr_name);
	printf("%5jd ", (intmax_t)stats.st_size);
	print_time(stats.st_mtime, stats);
	print_name_or_link(file, stats.st_mode);
}