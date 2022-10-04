// SPDX-FileCopyrightText: 2022 lfalkau
// SPDX-License-Identifier: Apache-2.0

#include <stddef.h>

size_t ft_strlen(const char *s) {
	size_t i = 0;

	while (s[i])
		i++;
	return i;
}

int ft_strcmp(const char *s1, const char *s2) {
	int i = 0;

	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
