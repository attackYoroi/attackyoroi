#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>


uint8_t sbox[256] = { 0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x1,0x67,0x2b,0xfe,0xd7,0xab,0x76
,0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0
,0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15
,0x4,0xc7,0x23,0xc3,0x18,0x96,0x5,0x9a,0x7,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75
,0x9,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84
,0x53,0xd1,0x0,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf
,0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x2,0x7f,0x50,0x3c,0x9f,0xa8
,0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2
,0xcd,0xc,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73
,0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0xb,0xdb
,0xe0,0x32,0x3a,0xa,0x49,0x6,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79
,0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x8
,0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a
,0x70,0x3e,0xb5,0x66,0x48,0x3,0xf6,0xe,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e
,0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf
,0x8c,0xa1,0x89,0xd,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0xf,0xb0,0x54,0xbb,0x16
};

uint8_t M16[8][8] = {
	0x5,0x4,0xa,0x6,0x2,0xd,0x8,0x3,
	0x4,0x5,0x6,0xa,0xd,0x2,0x3,0x8,
	0xa,0x6,0x5,0x4,0x8,0x3,0x2,0xd,
	0x6,0xa,0x4,0x5,0x3,0x8,0xd,0x2,
	0x2,0xd,0x8,0x3,0x5,0x4,0xa,0x6,
	0xd,0x2,0x3,0x8,0x4,0x5,0x6,0xa,
	0x8,0x3,0x2,0xd,0xa,0x6,0x5,0x4,
	0x3,0x8,0xd,0x2,0x6,0xa,0x4,0x5
};

uint8_t M16_inv[8][8] = {
	7,3,14,11,8,1,6,12,
	3,7,11,14,1,8,12,6,
	14,11,7,3,6,12,8,1,
	11,14,3,7,12,6,1,8,
	8,1,6,12,7,3,14,11,
	1,8,12,6,3,7,11,14,
	6,12,8,1,14,11,7,3,
	12,6,1,8,11,14,3,7,
};


uint8_t A16[2][2] = {
	0x2,0x1,
	0x3,0x2
};


uint16_t T1[65536] = {};
uint16_t T2[65536] = {};
uint16_t T3[65536] = {};
uint16_t T1_inv[65536] = {};
uint16_t T2_inv[65536] = {};
uint16_t T3_inv[65536] = {};

std::vector<std::vector<uint16_t>> lsb_T1(16, std::vector<uint16_t>(0, 0));


uint8_t Xtimes(uint8_t x) {
	return ((x << 1) ^ ((x & 0x80) ? 0x1b : 0x00));
}


uint16_t Sin_round(uint16_t Nin, uint16_t subkey) {
	uint16_t ret = 0x00;
	uint8_t temp[2][2] = {};
	//uint8_t temp_b[2] = {};
	uint8_t l8 = (Nin >> 8) & 0x00FF;
	uint8_t r8 = (Nin >> 0) & 0x00FF;
	l8 = sbox[l8];
	r8 = sbox[r8];
	temp[0][0] = l8;
	temp[1][0] = r8;
	for (int i = 0; i < 2; i++) {
		for (int j = 1; j < 2; j++) {
			temp[i][j] = Xtimes(temp[i][j - 1]);
		}
	}
	
	for (int i = 0; i < 2; i++) {	
		uint8_t tempsum = 0x00;
		for (int j = 0; j < 2; j++) {			
			uint8_t tempmultiply = 0x00;
			for (int k = 0; k < 2; k++) {
				tempmultiply ^= (((A16[i][j] >> k) & 0x01) * temp[j][k]);
			}
			tempsum = tempsum ^ tempmultiply;			
		}
		ret = ret << 8;
		ret = (ret & 0xff00) | tempsum;
		
	}
	ret = ret ^ subkey;
	return ret;
}


void table_gene(uint16_t T[], uint16_t T_inv[], uint16_t key[]) {
	uint16_t input = 0;
	for (int i = 0; i < 65536; i++) {
		T[i] = input;
		input++;
	}
	for (int r = 0; r < 32; r++) {
		for (int i = 0; i < 65536; i++) {
			T[i] = Sin_round(T[i], key[r]);
		}
	}

	for (int i = 0; i < 65536; i++) {
		T_inv[T[i]] = i;
	}

}

uint8_t Xtime_4(uint8_t x) {
	return ((x << 1) ^ ((x & 0x08) ? 0x03 : 0x00)) & 0x0F;
}



uint32_t cal_mds(uint8_t mds[][8], uint32_t mds_input) {
	uint8_t temp[8][4];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 4; j++) {
			if (0 == j) {
				temp[i][j] = (mds_input >> (7 - i) * 4) & 0x0F;
			}
			else {
				temp[i][j] = Xtime_4(temp[i][j - 1]);
			}
		}
	}

	uint32_t mds_ret = 0;
	for (int i = 0; i < 8; i++) {
		uint8_t tempsum = 0;
		for (int j = 0; j < 8; j++) {
			uint8_t tempmultiply = 0;
			for (int k = 0; k < 4; k++) {
				tempmultiply ^= ((mds[i][j] >> k) & 0x01) * temp[j][k];
			}
			tempsum = tempsum ^ tempmultiply;
		}
		mds_ret = mds_ret << 4;
		mds_ret = mds_ret ^ tempsum;
	}

	return mds_ret;
}

std::vector<uint64_t> roundfun(std::vector<uint64_t> input, uint16_t T[]) {
	uint64_t output[2] = { 0,0 };
	uint16_t table_in = 0x00;
	uint32_t mds_input = 0x00;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			output[i] = output[i] << 16;
			mds_input = mds_input << 4;
			table_in = (input[i] >> ((3 - j) * 16)) & 0x00FFFF;
			uint16_t table_out = T[table_in];
			output[i] = output[i] | table_out;
			mds_input = mds_input | (table_out & 0x000F);
		}
	}

	uint32_t mds_ret = cal_mds(M16, mds_input);

	for (int i = 0; i < 2; i++) {
		uint64_t k = (mds_ret >> (16 * (1 - i))) & 0x0FFFF;
		output[i] = output[i] & 0xFFF0FFF0FFF0FFF0;
		for (int j = 0; j < 4; j++) {
			output[i] = output[i] | (k >> (j * 4) & 0x0F) << (j * 16);
		}
	}

	for (int i = 0; i < 2; i++) {
		input[i] = output[i];
	}

	return input;
}

std::vector<uint64_t> lastround(std::vector<uint64_t> input) {

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			uint16_t bit_16 = (input[i] >> 48) & 0x00FFFF;
			input[i] = (input[i] << 16) & 0xFFFFFFFFFFFF0000;
			bit_16 = T3[bit_16];
			input[i] = input[i] | bit_16;
		}
	}
	return input;
}


std::vector<uint64_t> Yoroi_16(std::vector<uint64_t> input) {
	input = roundfun(input, T1);
	for (int i = 0; i < 6; i++) {
		input = roundfun(input, T2);
	}
	input = lastround(input);
	return input;
}


std::vector<uint64_t> roundfun_inv(std::vector<uint64_t> input, uint16_t T[]) {
	uint64_t output[2] = { 0,0 };
	uint16_t table_in = 0x00;
	uint32_t mds_input = 0x00;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			output[i] = output[i] << 16;
			mds_input = mds_input << 4;
			table_in = (input[i] >> ((3 - j) * 16)) & 0x00FFFF;
			uint16_t table_out = T[table_in];
			output[i] = output[i] | table_out;
			mds_input = mds_input | (table_out & 0x000F);
		}
	}
	uint32_t mds_ret = cal_mds(M16_inv, mds_input);

	for (int i = 0; i < 2; i++) {
		uint64_t k = (mds_ret >> (16 * (1 - i))) & 0x0FFFF;
		output[i] = output[i] & 0xFFF0FFF0FFF0FFF0;
		for (int j = 0; j < 4; j++) {
			output[i] = output[i] | (k >> (j * 4) & 0x0F) << (j * 16);
		}
	}


	for (int i = 0; i < 2; i++) {
		input[i] = output[i];
	}
	return input;
}

std::vector<uint64_t> firstround_inv(std::vector<uint64_t> input, uint16_t T[]) {
	uint64_t output[2] = { 0,0 };
	uint16_t table_in = 0x00;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			output[i] = output[i] << 16;
			table_in = (input[i] >> ((3 - j) * 16)) & 0x00FFFF;
			uint16_t table_out = T[table_in];
			output[i] = output[i] | table_out;
		}
	}

	for (int i = 0; i < 2; i++) {
		input[i] = output[i];
	}

	return input;
}

std::vector<uint64_t> Yoroi_16_inv(std::vector<uint64_t> input) {
	input = roundfun_inv(input, T3_inv);
	for (int i = 0; i < 6; i++) {
		input = roundfun_inv(input, T2_inv);
	}

	input = firstround_inv(input, T1_inv);
	return input;
}



unsigned long long factorial(int n) {
	if (n == 0) return 1;
	unsigned long long result = 1;
	for (int i = 1; i <= n; ++i) {
		result *= i;
	}
	return result;
}

std::vector<int> num2order(unsigned long long num) {

	int length = 14;
	std::vector<int> num_lst;
	while (length != 0) {
		length--;
		unsigned long long fac = factorial(length);
		num_lst.insert(num_lst.begin(), num / fac);
		num %= fac;
	}

	std::vector<int> lst;  // Length + 2 for the added 0 and 1

	for (int i = 0; i < num_lst.size(); ++i) {
		lst.insert(lst.begin() + num_lst[i], i + 2);
	}
	reverse(lst.begin(), lst.end());
	lst.insert(lst.begin(), 1);
	lst.insert(lst.begin(), 0);

	return lst;
}


std::vector < std::vector<uint64_t>> original(16826, std::vector<uint64_t>(2, 0));
std::vector<std::vector<std::vector<uint64_t>>> extend_ciphertexts(
	16826, std::vector<std::vector<uint64_t>>(4096, std::vector<uint64_t>(2, 0))
);


void get_right_pairs2() {
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 g1(seed1);
	std::mt19937_64 g2(seed1);


	uint64_t r1 = g2() & 0x0000FFFFFFFFFFFF;
	uint64_t r2 = g2();

	for (int i = 0; i < 15077; i++) {
		uint64_t msb_16 = i + 1888;
		original[i][0] = r1 + (msb_16 << 48);
		original[i][1] = r2;
		original[i] = Yoroi_16_inv(original[i]);

	}


	int num_correctpairs = 0;
	int num_correctpairs_tested = 0;
	std::ofstream ofs2;
	ofs2.open("correct_pairs_tested2_.txt", std::ios::app);
	std::vector < std::vector<uint64_t>> temp(2, std::vector<uint64_t>(2, 0));
	for (int i = 0; i < 15077; i++) {
		temp[0] = roundfun(original[i], T1);
		for (int j = i + 1; j < 15077; j++) {
			temp[1] = roundfun(original[j], T1);
			if (temp[0][1] == temp[1][1]) {
				if ((((temp[0][0] ^ temp[1][0]) & 0x0000FFFFFFFFFFFF) == 0) && ((((temp[0][0] ^ temp[1][0]) >> 48) & 0x0F) != 0)) {
					num_correctpairs++;
					uint16_t input_T1 = (original[i][0] >> 48) & 0x0FFFF;
					uint8_t lsb_4 = T1[input_T1] & 0x0F;
					uint64_t msb_16;
					for (int k = 0; k < 4096; k++) {
						msb_16 = lsb_T1[lsb_4][k];
						extend_ciphertexts[i][k][0] = (original[i][0] & 0x0000FFFFFFFFFFFF) ^ (msb_16 << 48);
						extend_ciphertexts[i][k][1] = original[i][1];
						extend_ciphertexts[i][k] = Yoroi_16(extend_ciphertexts[i][k]);

					}

					input_T1 = (original[j][0] >> 48) & 0x0FFFF;
					lsb_4 = T1[input_T1] & 0x0F;

					for (int k = 0; k < 4096; k++) {
						msb_16 = lsb_T1[lsb_4][k];
						extend_ciphertexts[j][k][0] = (original[j][0] & 0x0000FFFFFFFFFFFF) ^ (msb_16 << 48);
						extend_ciphertexts[j][k][1] = original[j][1];
						extend_ciphertexts[j][k] = Yoroi_16(extend_ciphertexts[j][k]);

					}


					num_correctpairs_tested = 0;
					for (int k = 0; k < 4096; k++) {
						for (int l = 0; l < 4096; l++) {
							if (extend_ciphertexts[i][k][1] == extend_ciphertexts[j][l][1]) {
								if (((extend_ciphertexts[i][k][0] ^ extend_ciphertexts[j][l][0]) & 0x0000FFFFFFFFFFFF) == 0) {
									num_correctpairs_tested++;
								}
							}
						}
					}
					ofs2 << num_correctpairs_tested << std::endl;


				}
			}
		}
	}
	ofs2 << "-------" << std::endl;
	ofs2.close();
}

void get_lsb_T1() {
	uint8_t lsb = 0;
	for (int i = 0; i < 65536; i++) {
		lsb = T1[i] & 0x00F;
		lsb_T1[lsb].push_back(i);
	}
}


int main() {

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 g1(seed1);
	std::mt19937_64 g2(seed1);

	uint16_t key1[32] = { 0 };
	uint16_t key2[32] = { 0 };
	uint16_t key3[32] = { 0 };

	for (int i = 0; i < 1000; i++) {
		for(int j = 0; j < 32; j++){
			key1[j] = g1() & 0x0FFFF;
			key2[j] = g1() & 0x0FFFF;
			key3[j] = g1() & 0x0FFFF;
		}

		table_gene(T1, T1_inv, key1);
		table_gene(T2, T2_inv, key2);
		table_gene(T3, T3_inv, key3);
		get_lsb_T1();

		get_right_pairs2();

	}

	return 0;


}
