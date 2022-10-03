#include <string.h>
#include <ft_getopt.h>
#include <stdio.h>


/*
	isopt checks if @s is a command line option, and of which kind.
	Returns the option kind
*/
static int isopt(const char *s) {
	if (strlen(s) < 2 || s[0] != '-')
		return no_opt;
	if (s[1] != '-')
		return short_opt;
	if (strlen(s) > 2)
		return long_opt;
	return break_opt;
}

int ft_getopt(int ac, char **av, const char *optstring, char **optarg) {
	return -1;
}

int ft_getopt_long(int ac, char **av, const option_t *opts, char **optarg) {
	static int	optind = 1;
	static char	*nextchar = NULL;
	int optc = 0;

	// If netxtchar isn't NULL, we are inside a short options list (e.g. -tlnp)
	if (nextchar != NULL) {
		for (const option_t *opt = opts; opt->longname != NULL || opt->shortname != 0; opt += 1) {
			if (opt->shortname == *nextchar) {
				optc = opt->shortname;
				break;
			}
		}
		if (optc == 0)
			printf("%s: Invalid short (f) option -- '%c'\n", av[0], *nextchar);
		nextchar++;
		if (*nextchar == 0) {
			nextchar = NULL;
			optind++;
		}
		return optc != 0 ? optc : '?';
	}

	if (optind < ac) {
		int opt_type = isopt(av[optind]);
		if (opt_type == long_opt) {
			for (const option_t *opt = opts; opt->longname != NULL; opt += 1) {
				if (strcmp(&av[optind][2], opt->longname) == 0) {
					optc = opt->shortname;
					break;
				}
			}
			if (optc == 0)
				printf("%s: Invalid long option -- '%s'\n", av[0], &av[optind][2]);
			optind++;
			return optc != 0 ? optc : '?';
		}
		if (opt_type == short_opt) {
			for (const option_t *opt = opts; opt->shortname != 0; opt += 1) {
				if (opt->shortname == av[optind][1]) {
					optc = opt->shortname;
					break;
				}
			}
			if (optc == 0)
		 		printf("%s: Invalid short option -- '%c'\n", av[0], av[optind][1]);
			if (av[optind][2] == 0) {
				optind++;
				nextchar = NULL;
			} else {
				nextchar = &av[optind][2];
			}
			return optc != 0 ? optc : '?';
		}
	}
	return -1;
}

int main(int ac, char **av) {
	option_t opts[] = {
		{"verbose",		'v',	no_argument},
		{"help",		'h',	no_argument},
		{"count",		'c',	required_argument},
		{"timestamp",	'D',	no_argument},
		{"flood",		'f',	no_argument},
		{"interval",	'i',	required_argument},
		{"quiet",		'q',	no_argument},
		{NULL,			'\0',	no_argument},
	};
	int opt;
	char *optarg;

	while ((opt = ft_getopt_long(ac, av, opts, &optarg)) != -1) {
		switch (opt) {
		case 'v':
			printf("-v\n");
			break;
		case 'h':
			printf("-h\n");
			break;
		case 'c':
			printf("-c: %s\n", optarg);
			break;
		case 'D':
			printf("-D\n");
			break;
		case 'f':
			printf("-f\n");
			break;
		case 'i':
			printf("-i: %s\n", optarg);
			break;
		case 'q':
			printf("-q\n");
			break;
		}
	}

}
