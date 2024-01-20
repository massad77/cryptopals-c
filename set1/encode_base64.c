#include <stdio.h>
#include <stdlib.h>
#include "base64_conversion.h"

/* reads from stdin and prints base64 encoded to stdout */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	size_t nread = 0;
	nread = getline(&line, &len, stdin);
	line[nread-1] = '\0'; // remove '\n'

	int out_len = 0;
	char *out_base64 = encode_base64(line, nread-1, &out_len);
	printf("%s", out_base64);

	free(line);
	free(out_base64);
	return 0;
}
