#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>

typedef unsigned char byte;
// const char *stringtest = "fc7368f13b4ced2fcf36fd0f589a0930928499e9c0f88ea83e5ac9464bdfc25a";
const char *string1 = "4a5c2d660232375d25dc141febdaae056ba05e95fe606e88a350929a36a9ea67";
const char *string2 = "6f32ebbc1ee9cf3867df5f86f071ee147c6190ac7bfd88330fd8996a0abb512e";
const char *string3 = "33c35f8c8515b13ce15324718eccea7fb10e0c8848df3e3e0a7c0e529303828d";
const char *string4 = "dc348085d14fefa692adf1e7d97e2d59253c01189359873186d376ebe0f3ad3a";

const int len = 26;

byte* StringHashToByteArray(const char* s) {
	byte* hash = (byte*) malloc(32);
	char two[3];
	two[2] = 0;
	for (int i = 0; i < 32; i++) {
		two[0] = s[i * 2];
		two[1] = s[i * 2 + 1];
		hash[i] = (byte)strtol(two, 0, 16);
	}
	return hash;
}

int matches(byte *a, byte* b) {
	for (int i = 0; i < 32; i++)
		if (a[i] != b[i])
			return 0;
	return 1;
}

void printResult(byte* password, byte* hash) {
	char sPass[5];
	memcpy(sPass, password, 5);
	sPass[4] = 0;
	printf("%s => ", sPass);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		printf("%02x", hash[i]);
	printf("\n");
}

int main (void) {
	byte s[4] ;
	omp_set_num_threads(676);
	
	// byte* sha256 = StringHashToByteArray(string1);
	// byte* sha256 = StringHashToByteArray(string2);
	// byte* sha256 = StringHashToByteArray(string3);
	byte* sha256 = StringHashToByteArray(string4);
	double start = omp_get_wtime();
	// #pragma omp parallel for private(s)
	for (int i = 97; i < 97+len; i++) {
		s[0] = i;
		for (s[1] = 97; s[1] < 97+len; s[1]++)
			for (s[2] = 97; s[2] < 97+len; s[2]++)
				for (s[3] = 97; s[3] < 97+len; s[3]++) {
					byte *hash = SHA256(s, 4, 0);
					if (matches(sha256, hash))
						printResult(s, hash);
				}
	}

	double delta = omp_get_wtime() - start;
	printf("Total time: %.6f seconds\n", delta);
	return 0;
}