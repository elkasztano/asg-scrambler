// For demonstration purposes only. Not intended to be used to encrypt confident data. 
// The alternating step generator (ASG) has been described by C.G. Günther in 1986 and has been patented in 1989.
// According to patents.google.com the patent has already expired.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

// declare lfsr state arrays, max. 128 bit = 2 x 64 bit
uint64_t state128[2];
uint64_t state127[2];
uint64_t state126[2];

// 128 bit lfsr with taps at 128, 127, 126, 121. Period = (2^128)-1
int lfsr128(void)
{
	state128[0] = state128[0] >> 1 | state128[1] << 63;
	state128[1] = (-(state128[0] & 1ULL) & 0xe100000000000000ULL) ^ (state128[1] >> 1);
}

// 127 bit lfsr with taps at 127 and 126. Period = (2^127)-1
int lfsr127(void)
{
	state127[0] = (state127[0] >> 1) | (state127[1] << 63);
	state127[1] = (-(state127[0] & 1ULL) & 0x6000000000000000ULL) ^ (state127[1] >> 1);
}

// 126 bit lfsr with taps at 126, 124, 122, 119. Period = (2^126)-1
int lfsr126(void)
{
	state126[0] = (state126[0] >> 1) | (state126[1] << 63);
	state126[1] = (-(state126[0] & 1ULL) & 0x2a40000000000000ULL) ^ (state126[1] >> 1);
}

// Print content of variable in binary format. Only used for debugging.
int bitview(uint64_t x)
{
	for (int i = 63; i >= 0; --i) printf("%d", (int)((x >> i) & 1ULL));
}

int main(int argc, char *argv[])
{
	// initializing array for lfsr128 start state
	uint64_t start_state128[2] = {0ULL};
	
	char *seed;
	int opt;
	int count=0;
	
	// setting default state for 128 bit lfsr
	state128[0] = 0x17ee9999ab123450ULL; 
	state128[1] = 0xf01f4fdee002f1f8ULL;

	// parsing command line arguments
	while((opt = getopt(argc, argv, ":hp:")) != -1)
	{
		switch(opt)
		{
			case 'h':
				printf("-h show help, -p password/seed\n");
				return 0;
			case 'p':
				// get length of seed
				while(optarg[count] != '\0') ++count;
				
				// copy user input to seed array
				seed = (char*)calloc(count, sizeof(char));
				for (int i = 0; i < count; ++i) seed[i] = optarg[i];				    
				
				// XOR initialized state value with seed. 
				for (int i = 0; i < count; ++i) start_state128[(i/8)%2] ^= ((uint64_t)seed[i] << ((i%8)*8));

				break;
			case ':':
				printf("option needs a value\n");
				return 1;
			case '?':
				printf("unknown option: %c\n", optopt);
				return 1;
		}
	}


	// set start state for 128 bit lfsr
	for (int i = 0; i < 2; ++i) state128[i] = start_state128[i];

	// set start state for 127 bit lfsr
	state127[0] = 0x7fe1105ab10c5e4fULL;
	state127[1] = 0x9feb12821dde00ULL;
	
	// set start state for 126 bit lfsr
	state126[0] = 0x1c0fe2ca3e3c9fe4ULL;
	state126[1] = 0xca1fee6caf0900ULL;

	// let the lfsrs run a few times to wash out the start values
	for (int i = 0; i < 3334; ++i) lfsr128();
	for (int i = 0; i < 2736; ++i) lfsr127();
	for (int i = 0; i < 4101; ++i) lfsr126();

	int d;
	char c, temp;

	// get bytes from stdin and perform bitwise XOR-operation using the output bit from the alternating step generator
	while ((d = fgetc(stdin)) != EOF)
	{
		c = (char)d;
		temp = 0;
		for (int i = 0; i < 8; ++i)
		{
			// if the least significant bit of the 128 bit lfsr is 1 run the 127 bit lfsr, else run the 126 bit lfsr
			lfsr128();
			if (state128[0] & 1ULL)
			{
				lfsr127();
				temp |= (state127[0] & 1ULL) << i;
			}
			else
			{
				lfsr126();
				temp |= (state126[0] & 1ULL) << i;
			}
		}
		c ^= temp;
		
		// write scrambled bytes to stdout
		fputc(c, stdout);
	}

	return 0;
}
