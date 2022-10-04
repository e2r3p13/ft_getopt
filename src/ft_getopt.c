#include <string.h>
#include <ft_getopt.h>
#include <stdio.h>


/*
	isopt checks if @s is a command line option, and of which kind.
	Returns the option kind
*/
static int getargtype(const char *s) {
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
	static int	optindex = 1;
	static char	*nextchar = NULL;
	const option_t *opt = NULL;
	int av_type;

	if (optindex >= ac)
		return -1;
	av_type = getargtype(av[optindex]);
	if (av_type == no_opt || av_type == break_opt) {
		// TODO: reorganize arguments it no_opt, to accept post arg options
		return -1;
	}

	for (const option_t *o = opts; o->longname != NULL || o->shortname != '\0'; o += 1) {
		if (av_type == short_opt && ((nextchar == NULL && av[optindex][1] == o->shortname) || (nextchar != NULL && *nextchar == o->shortname))) {
			opt = o;
			break;
		}
		if (av_type == long_opt && strcmp(&av[optindex][2], o->longname) == 0) {
			opt = o;
			break;
		}
	}

	if (opt == NULL) {
		if (av_type == short_opt)
			printf("%s: Invalid short option -- '%c'\n", av[0], av[optindex][1]);
		else
			printf("%s: Invalid long option -- '%s'\n", av[0], &av[optindex][2]);
	}

	if (av_type == short_opt) {
		nextchar = nextchar ? nextchar + 1 : &av[optindex][2];
		if (*nextchar == '\0')
			nextchar = NULL;
	}
	if (nextchar == NULL)
		optindex += 1;
	
	if (opt && opt->has_arg) {
		*optarg = nextchar ? nextchar : av[optindex];
		optindex += 1;
		nextchar = NULL;
	}

	return opt ? opt->shortname : '?';
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
		case '?':
			printf("Usage: ...\n");
			break;
		}
	}

}
