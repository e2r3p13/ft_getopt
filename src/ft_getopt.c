// SPDX-FileCopyrightText: 2022 lfalkau
// SPDX-License-Identifier: Apache-2.0

#include <ft_string.h>
#include <ft_getopt.h>
#include <stdio.h>
#include <unistd.h>

/*
   getoptft library implements a custom version of ft_getopt_long.
   It doesn't make use of global variables, and use custom version
   of some basic functions, to match the 42 school rules.
*/

/*
	isopt checks if @s is a command line option, and of which kind
	Returns the option kind
*/
static int getargtype(const char *s) {
	if (ft_strlen(s) < 2 || s[0] != '-')
		return no_opt;
	if (s[1] != '-')
		return short_opt;
	if (ft_strlen(s) > 2)
		return long_opt;
	return break_opt;
}

/*
	findopt iterates through an option_t array and tries to match the next option character/string with.
	Returns a pointer to the matched option, or NULL if no match is found
	
	An error message is printed if no match is found.
	TODO: Add an interface to diasble error message e.g. environment variable
*/
static const option_t *findopt(char **av, const int av_type, char *nextchar, int optindex, const option_t *opts) {
	for (const option_t *o = opts; o->longname != NULL || o->shortname != '\0'; o += 1) {
		if (av_type == short_opt && ((nextchar == NULL && av[optindex][1] == o->shortname) || (nextchar != NULL && *nextchar == o->shortname))) {
			return o;
		}
		if (av_type == long_opt && ft_strcmp(&av[optindex][2], o->longname) == 0) {
			return o;
		}
	}
	return NULL;
}

/*
	updoptptrs updates optindex and nextchar so that the next option can be proceeded.
	optindex stores the index of the next argument to be proceeded in @av
	nextchar stores a pointer to the next character to be proceeded (useful for short options combinations)

	For more details, see the official getopt documentation:
	https://www.man7.org/linux/man-pages/man3/getopt.3.html
*/
static void updoptptrs(char **av, const int av_type, int *oip, char **ncp) {
	if (av_type == short_opt) {
		*ncp = *ncp ? *ncp + 1 : &av[*oip][2];
		if (**ncp == '\0')
			*ncp = NULL;
	}
	if (*ncp == NULL) {
		*oip += 1;
	}
}

/*
	getoptarg return a pointer to the option argument, which is either the nextchar pointer
	if set, or the next av argument, pointed by optindex. It also update the nextchar and
	optindex values
	Returns the argument pointer, or NULL if no argument is present
*/
static char *getoptarg(int ac, char **av, int *oip, char **ncp) {
	char *optarg = NULL;

	if (*oip < ac) {
		optarg = *ncp ? *ncp : av[*oip];
		*oip += 1;
		*ncp = NULL;
	}
	return optarg;
}

/*
 	swaparg put the ith element of av and moves it at the end
	It keeps a counter of how much items it has moved, so that
	when i is ac - count, it means that all non option arguments
	has already been moved
	Returns 0 if the argument is moved, -1 if all elements has
	already been moved
	// TODO: Add an interface to dissble swaping arguments e.g. with an environment variable
*/
static int swaparg(int ac, char **av, int i) {
	static int count = 0;
	char *tmp;

	// We take ac - 1 else we'll try to swap the last element with itself
	if (i + count >= ac - 1) 
		return -1;

	tmp = av[i];
	while (i < ac - 1) {
		av[i] = av[i + 1];
		i++;
	}
	av[ac - 1] = tmp;

	count += 1;
	return 0;
}

int ft_getopt_long(int ac, char **av, const option_t *opts, char **optarg) {
	static int optindex = 1;
	static char	*nextchar = NULL;
	const option_t	*opt = NULL;
	int av_type;

	if (optindex >= ac)
		return -1;

	do {
		av_type = getargtype(av[optindex]);
		if (av_type == break_opt || (av_type == no_opt && swaparg(ac, av, optindex) < 0))
			return -1;
	} while (av_type != short_opt && av_type != long_opt);

	opt = findopt(av, av_type, nextchar, optindex, opts);
	if (opt == NULL) {
		if (av_type == short_opt)
			printf("%s: Invalid short option -- '%c'\n", av[0], av[optindex][1]);
		else
			printf("%s: Invalid long option -- '%s'\n", av[0], &av[optindex][2]);
	}
	updoptptrs(av, av_type, &optindex, &nextchar);
	
	if (opt && opt->has_arg) {
		*optarg = getoptarg(ac, av, &optindex, &nextchar);
		if (*optarg == NULL) {
			if (av_type == short_opt)
				printf("%s: Option requires an argument -- '%c'\n", av[0], av[optindex - 1][1]);
			else
				printf("%s: Option requires an argument -- '%s'\n", av[0], &av[optindex - 1][2]);
		}
	}
	return opt ? opt->shortname : '?';
}
