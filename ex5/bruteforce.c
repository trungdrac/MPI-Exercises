#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <openssl/sha.h>
 
#define HASH_NUM 4
#define PASSWORD_LENGTH 4
#define SIZE 29

char alpabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', 10, 13};

typedef unsigned char byte;
 
int matches(byte *a, byte* b) {
	for (int i = 0; i < 32; i++)
		if (a[i] != b[i])
			return 0;
	return 1;
}
 
 
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
 
void printResult(byte* password, byte* hash, int passwordLength) {
	char sPass[passwordLength + 1];
	memcpy(sPass, password, 5);
	sPass[passwordLength] = 0;
	printf("%s => ", sPass);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		printf("%02x", hash[i]);
	printf("\n");
}
 
int main(int argc, char **argv)
{
	byte* hashedPass[HASH_NUM];
	hashedPass[0] = StringHashToByteArray("4a5c2d660232375d25dc141febdaae056ba05e95fe606e88a350929a36a9ea67");
	hashedPass[1] = StringHashToByteArray("6f32ebbc1ee9cf3867df5f86f071ee147c6190ac7bfd88330fd8996a0abb512e");
	hashedPass[2] = StringHashToByteArray("33c35f8c8515b13ce15324718eccea7fb10e0c8848df3e3e0a7c0e529303828d");
	hashedPass[3] = StringHashToByteArray("dc348085d14fefa692adf1e7d97e2d59253c01189359873186d376ebe0f3ad3a");

#pragma omp parallel
	{
 
#pragma omp for
		for (int a = 0; a < SIZE; a++)
		{
			for (int b = 0; b < SIZE; b++)
				for (int c = 0; c < SIZE; c++)
					for (int d = 0; d < SIZE; d++) {
						byte *hash;
						byte password[PASSWORD_LENGTH] = {alpabet[a], alpabet[b], alpabet[c], alpabet[d]};
						hash = SHA256(password, PASSWORD_LENGTH, 0);
						if (matches(hashedPass[0], hash) || matches(hashedPass[1], hash) 
									|| matches(hashedPass[2], hash) || matches(hashedPass[3], hash))
									printResult(password, hash, PASSWORD_LENGTH);
						// password troll
						byte passwordTroll[PASSWORD_LENGTH + 1] = {alpabet[a], alpabet[b], alpabet[c], alpabet[d], 10};
						hash = SHA256(passwordTroll, PASSWORD_LENGTH + 1, 0);
						if (matches(hashedPass[0], hash) || matches(hashedPass[1], hash) 
									|| matches(hashedPass[2], hash) || matches(hashedPass[3], hash))
									printResult(passwordTroll, hash, PASSWORD_LENGTH + 1);
					}
		}
	}
	
	for (int i = 0; i < HASH_NUM; i++) {
		free(hashedPass[i]);
	}
	return 0;
}