ft_strqsort(&argv[i], argc - i)

static void	ft_strswap(char **str1, char **str2)
{
	char	*tmp;

	tmp = *str1;
	*str1 = *str2;
	*str2 = tmp;
}

static void	rec_sort(char *strings[], int begin, int end)
{
	register int	left;
	register int	right;
	char			*pivot;

	if (begin >= end)
		return ;
	pivot = strings[begin];
	left = begin + 1;
	right = end;
	while (left < right)
	{
		if (ft_strcmp(strings[left], pivot) <= 0)
			left++;
		else if (ft_strcmp(strings[right], pivot) > 0)
			right--;
		else if (left < right)
			ft_strswap(&strings[left], &strings[right]);
	}
	left--;
	ft_strswap(&strings[begin], &strings[left]);
	rec_sort(strings, begin, left);
	rec_sort(strings, right, end);
}

void	ft_strqsort(char *strings[], int size)
{
	rec_sort(strings, 0, size - 1);
}
