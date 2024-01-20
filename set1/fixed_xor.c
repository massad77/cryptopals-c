
int fixed_xor(char *in1, char *in2, char *result, int length)
{
	for(int i = 0; i < length; ++i)
	{
		result[i] = in1[i] ^ in2[i];
	}
	return 0;
}
