#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sysexits.h>
#include <time.h>
#include <unistd.h>

#define BUF_SIZE 1024

const char* current_dir = ".";

struct Options
{
    bool using_a_opt;
    bool using_d_opt;
    bool using_g_opt;
    bool using_h_opt;
    bool using_i_opt;
    bool using_l_opt;
    bool using_o_opt;
    bool using_p_opt;
    bool using_Q_opt;
    bool using_r_opt;
    bool using_S_opt;
    bool using_t_opt;
    bool using_U_opt;
} opts;

void init_opts()
{
    opts.using_a_opt = false;
    opts.using_d_opt = false;
    opts.using_g_opt = false;
    opts.using_h_opt = false;
    opts.using_i_opt = false;
    opts.using_l_opt = false;
    opts.using_o_opt = false;
    opts.using_p_opt = false;
    opts.using_Q_opt = false;
    opts.using_r_opt = false;
    opts.using_S_opt = false;
    opts.using_t_opt = false;
    opts.using_U_opt = false;
}

void get_opts(int count, char* args[])
{
    init_opts();

    static struct option long_opts[] = {
        { "all",             optional_argument, NULL, 'a' },
        { "directory",       optional_argument, NULL, 'd' },
        { "",                optional_argument, NULL, 'g' },
        { "human-readable",  optional_argument, NULL, 'h' },
        { "inode",           optional_argument, NULL, 'i' },
        { "",                optional_argument, NULL, 'l' },
        { "",                optional_argument, NULL, 'o' },
        { "file-type",       optional_argument, NULL, 'p' },
        { "quote-name",      optional_argument, NULL, 'Q' },
        { "reverse",         optional_argument, NULL, 'r' },
        { "",                optional_argument, NULL, 'S' },
        { "",                optional_argument, NULL, 't' },
        { "",                optional_argument, NULL, 'U' },
        { NULL,              0,                 NULL,  0  }
    };

    int opt;

    while ((opt = getopt_long(count, args, 
        "adghilopQrStU", long_opts, NULL)) != -1)
    {
        switch (opt)
        {
            case 'a': opts.using_a_opt = true; break;
            case 'd': opts.using_d_opt = true; break;
            case 'g': opts.using_g_opt = true; break;
            case 'h': opts.using_h_opt = true; break;
            case 'i': opts.using_i_opt = true; break;
            case 'l': opts.using_l_opt = true; break;
            case 'o': opts.using_o_opt = true; break;
            case 'p': opts.using_p_opt = true; break;
            case 'Q': opts.using_Q_opt = true; break;
            case 'r': opts.using_r_opt = true; break;
            case 'S': opts.using_S_opt = true; break;
            case 't': opts.using_t_opt = true; break;
            case 'U': opts.using_U_opt = true; break;
            case '?': exit(EX_USAGE);
        }
    }
}

void print_permissions(mode_t mode)
{
    putchar((mode & S_IRUSR) ? 'r' : '-');
    putchar((mode & S_IWUSR) ? 'w' : '-');
    putchar((mode & S_IXUSR) ? 'x' : '-');
    putchar((mode & S_IRGRP) ? 'r' : '-');
    putchar((mode & S_IWGRP) ? 'w' : '-');
    putchar((mode & S_IXGRP) ? 'x' : '-');
    putchar((mode & S_IROTH) ? 'r' : '-');
    putchar((mode & S_IWOTH) ? 'w' : '-');
    putchar((mode & S_IXOTH) ? 'x' : '-');
}

void print_filetype(mode_t mode)
{
    switch (mode & S_IFMT)
    {
        case S_IFREG:  putchar('-'); break;
        case S_IFDIR:  putchar('d'); break;
        case S_IFLNK:  putchar('l'); break;
        case S_IFCHR:  putchar('c'); break;
        case S_IFBLK:  putchar('b'); break;
        case S_IFSOCK: putchar('s'); break;
        case S_IFIFO:  putchar('f'); break;
    }
}

void print_readable_filesize(off_t size)
{
    const char* units[] = { "", "K", "M", "G", "T" };
    int i;

    for (i = 0; size > 1024; ++i, size /= 1024);

    char filesize_buf[BUF_SIZE];

    snprintf(filesize_buf, sizeof(filesize_buf), 
        "%*jd%s ", i, (intmax_t)size, units[i]);

    printf(" %8s", filesize_buf);
}

void print_time(time_t mod_time)
{
    // get current time with year
    time_t curr_time;
    time(&curr_time);
    struct tm* t = localtime(&curr_time);
    const int curr_mon = t->tm_mon;
    const int curr_yr = 1970 + t->tm_year;

    // get modified time and year
    t = localtime(&mod_time);
    const int mod_mon = t->tm_mon;
    const int mod_yr = 1970 + t->tm_year;

    // determine format based on modification time
    // (past six months)
    const char* format = (mod_yr == curr_yr)
                       && (mod_mon >= (curr_mon - 6))
                           ? "%b %e %H:%M"
                           : "%b %e  %Y";

    char time_buf[BUF_SIZE];

    const size_t bytes_written = 
        strftime(time_buf, sizeof(time_buf), format, t);

    if (bytes_written <= 0)
    {
        exit(EX_IOERR);
    }

    printf("%s", time_buf);
}

struct stat get_stats(const char* filename)
{
    char path_buf[BUF_SIZE];

    const int bytes_written = 
        snprintf(path_buf, sizeof(path_buf), 
        "%s/%s", current_dir, filename);

    if (bytes_written <= 0)
    {
        exit(EX_IOERR);
    }

    struct stat sb;

    if (lstat(path_buf, &sb) < 0)
    {   
        perror(path_buf);
        exit(EX_IOERR);
    }

    return sb;
}

int is_dir(const char* filename)
{
    struct stat sb = get_stats(filename);

    if (lstat(filename, &sb) < 0)
    {
        perror(filename);
        exit(EX_IOERR);
    }

    return sb.st_mode & S_IFDIR;
}

bool is_in_dir(const char* dir, const char* filename)
{
    DIR* dfd = opendir(dir);

    if (!dfd)
    {
        perror(dir);
        exit(EX_IOERR);
    }

    struct dirent* dp = readdir(dfd);

    while (dp)
    {
        if (strcmp(filename, dp->d_name) == 0)
        {
            closedir(dfd);
            return true;
        }      

        dp = readdir(dfd);
    }

    fprintf(stderr, "file \'%s\' not found\n", filename);

    closedir(dfd);

    return false;
}

void print_link(const char* dir, const char* filename)
{
    char path_buf[BUF_SIZE];

    const int bytes_written = 
        snprintf(path_buf, sizeof(path_buf), 
        "%s/%s", dir, filename);

    if (bytes_written <= 0)
    {
        exit(EX_IOERR);
    }

    char link_buf[BUF_SIZE];
    const ssize_t count = 
        readlink(path_buf, link_buf, sizeof(link_buf));

    if (count == -1)
    {
        perror("readlink");
        exit(EX_IOERR);
    }

    link_buf[count] = '\0';

    if (opts.using_Q_opt)
    {
        printf(" \"%s\" -> \"%s\"\n", filename, link_buf);
    }
    else
    {
        printf(" %s -> %s \n", filename, link_buf);
    }
}

void print_name(const char* filename)
{
    if (opts.using_Q_opt)
    {
        printf(" \"%s\"", filename);
    }
    else
    {
        printf(" %s", filename);
    }

    if (opts.using_p_opt && is_dir(filename))
    {
        putchar('/');
    }

    putchar('\n');
}

void display_stats(const char* dir, const char* filename)
{
    if (!is_in_dir(dir, filename))
    {
        return;
    }

    if (!opts.using_g_opt 
        && !opts.using_o_opt 
        && !opts.using_l_opt)
    {
        if (opts.using_Q_opt)
        {
            printf("\"%s\"\n", filename);
        }
        else
        {
            printf("%s\n", filename);
        }

        return;
    }

    struct stat sb = get_stats(filename);

    // print inode number
    if (opts.using_i_opt)
    {
        printf("%ju ", (uintmax_t)sb.st_ino);
    }

    print_filetype(sb.st_mode);

    print_permissions(sb.st_mode);

    printf(" %jd ", (intmax_t)sb.st_nlink);

    // print owner
    if (!opts.using_g_opt)
    {
        printf("%10s ", getpwuid(sb.st_uid)->pw_name);
    }

    // print group
    if (!opts.using_o_opt)
    {
        printf("%10s", getgrgid(sb.st_gid)->gr_name);
    }

    // print file size
    if (opts.using_h_opt)
    {
        print_readable_filesize(sb.st_size);
    }
    else
    {
        printf("%10jd ", (intmax_t)sb.st_size);
    }

    print_time(sb.st_mtime);

    // print name/link
    if ((sb.st_mode & S_IFMT) == S_IFLNK)
    {
        print_link(dir, filename);
    }
    else
    {
        print_name(filename);
    }
}

int compare_size(const void* p1, const void* p2)
{
    const char* str1 = *(const char**)p1;
    const char* str2 = *(const char**)p2;

    const off_t size1 = get_stats(str1).st_size;
    const off_t size2 = get_stats(str2).st_size;

    const int return_value = (size2 > size1) - (size2 < size1);

    return (!opts.using_r_opt) ? return_value : -return_value;
}

int compare_time(const void* p1, const void* p2)
{
    const char* str1 = *(const char**)p1;
    const char* str2 = *(const char**)p2;

    const time_t time1 = get_stats(str1).st_mtime;
    const time_t time2 = get_stats(str2).st_mtime;

    const int return_value = (time2 > time1) - (time2 < time1);

    return (!opts.using_r_opt) ? return_value : -return_value;
}

int compare_lex(const void* p1, const void* p2)
{
    const char* str1 = *(const char**)p1;
    const char* str2 = *(const char**)p2;

    return (!opts.using_r_opt) 
        ? strcasecmp(str1, str2)
        : -strcasecmp(str1, str2);
}

void sort_dir(char** entries, size_t entries_count)
{
    int(*comparer)(const void*, const void*);

    if (opts.using_S_opt)
    {
        comparer = compare_size;
    }
    else if (opts.using_t_opt)
    {
        comparer = compare_time;
    }
    else
    {
        comparer = compare_lex;
    }

    qsort(entries, entries_count, sizeof(char*), comparer);
}

void structure_dir_entries(DIR* dfd, char** entries, size_t* entries_count)
{
    struct dirent* dp = readdir(dfd);
    *entries_count = 0;

    while (dp)
    {
        const bool omit_hidden = !opts.using_a_opt
                              && dp->d_name[0] == '.';

        if (!omit_hidden)
        {
            if (*entries_count >= BUF_SIZE)
            {
                entries = realloc(
                    entries, sizeof(entries) * BUF_SIZE);

                if (!entries)
                {
                    perror("realloc");
                    abort();
                }
            }

            entries[*entries_count] = dp->d_name;
            ++(*entries_count);
        }

        dp = readdir(dfd);
    }

    if (!opts.using_U_opt)
    {
        sort_dir(entries, *entries_count);
    }
}

void display_dir_entries(const char* dir)
{
    char** entries = malloc(BUF_SIZE * sizeof(char*));

    if (!entries)
    {
        perror("malloc");
        abort();
    }

    DIR* dfd = opendir(dir);

    size_t entries_count;

    structure_dir_entries(dfd, entries, &entries_count);

    for (size_t i = 0; i < entries_count; ++i)
    {
        display_stats(dir, entries[i]);
    }

    closedir(dfd);

    free(entries);
}

void display_dir_args(int argc, char* argv[])
{
    const bool no_dirs_given = (argc - optind) == 0;

    if (no_dirs_given)
    {
        display_stats(".", ".");
    }

    for (int i = optind; i < argc; ++i)
    {
        display_stats(".", argv[i]);
    }
}

void scan_dir_entries(int argc, char* argv[])
{
    const bool multiple_dirs = (argc - optind) >= 2;

    for (int i = optind; i < argc; ++i)
    {
        if (!is_dir(argv[i]))
        {
            display_stats(".", argv[i]);
            continue;
        }

        if (!is_in_dir(".", argv[i]))
        {
            continue;
        }

        if (multiple_dirs)
        {
            printf("\n%s:\n", argv[i]);
        }

        display_dir_entries(argv[i]);
    }
}

int main(int argc, char* argv[])
{
    get_opts(argc, argv);

    if (opts.using_d_opt)
    {
        display_dir_args(argc, argv);
        return EX_OK;
    }
    else
    {
        if (optind == argc)
        {
            display_dir_entries(".");
            return EX_OK;
        }

        scan_dir_entries(argc, argv);
    }
}
