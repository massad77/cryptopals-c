#include <stdio.h>
#include <stdlib.h>
#include "base64_conversion.h"

/* reads base64 encoded from stdin and prints decoded to stdout */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	size_t nread = 0;
	nread = getline(&line, &len, stdin);

	int out_len = 0;
	char *out = decode_base64(line, nread, &out_len);
	printf("%s", out);

	free(line);
	free(out);
	return 0;
}
