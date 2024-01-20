#include <stdio.h>
#include <stdlib.h>
#include "fixed_xor.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	char *line = NULL;
	size_t len = 0;
	size_t nread = 0

	if(argc < 2) {
		printf("ERROR: Specify key\n");
		exit(EXIT_FAILURE);
	}
	char *key = argv[1];

	nread = getline(&line, &len, stdin);

	char *out = XORencode(line, nread, key, get_length(key));
	printf("%s", out);

	free(line);
	free(out);
	exit(EXIT_SUCCESS);
}
