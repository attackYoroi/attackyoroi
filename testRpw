#include <stdint.h>
#include <iostream>
#include <chrono>
#include <random>

using namespace std;

uint18_t P[262144][16] = {};


uint8_t mds_16[8][8] = {
	 5, 4,10, 6, 2,13, 8, 3,
	 4, 5, 6,10,13, 2, 3, 8,
	10, 6, 5, 4, 8, 3, 2,13,
	 6,10, 4, 5, 3, 8,13, 2,
	 2,13, 8, 3, 5, 4,10, 6,
	13, 2, 3, 8, 4, 5, 6,10,
	 8, 3, 2,13,10, 6, 5, 4,
	 3, 8,13, 2, 6,10, 4, 5,
};

uint8_t mds_16inv[8][8] = {
7, 3,14,11, 8, 1, 6,12
,3, 7,11,14, 1, 8,12, 6
,14,11, 7, 3, 6,12, 8, 1
,11,14, 3, 7,12, 6, 1, 8
,8, 1, 6,12, 7, 3,14,11
,1, 8,12, 6, 3, 7,11,14
,6,12, 8, 1,14,11, 7, 3
,12, 6, 1, 8,11,14, 3, 7
};

uint8_t mds_32inv[4][4] = {
	14,11,13,9,
	9,14,11,13,
	13,9,14,11,
	11,13,9,14
};

uint8_t mds_32[4][4] = {
	2,3,1,1,
	1,2,3,1,
	1,1,2,3,
	3,1,1,2
};

uint8_t Xtime_4(uint8_t x) {
	return (((x << 1)) ^ ((x & 0x08) ? 0x03 : 0x00)) & 0x0F;
}

uint32_t cal_mds_16(uint32_t mds_input) {
	uint8_t temp[8][4] = { 0 };
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 4; j++) {
			if (j == 0) {
				temp[i][j] = (mds_input >> ((7 - i) * 4)) & 0x0F;
			}
			else {
				temp[i][j] = Xtime_4(temp[i][j - 1]);
			}
		}
	}

	uint32_t mds_ret = 0;
	uint8_t tempsum = 0;
	for (int i = 0; i < 8; i++) {
		tempsum = 0;
		for (int j = 0; j < 8; j++) {
			uint8_t tempmultiply = 0;
			for (int k = 0; k < 4; k++) {
				tempmultiply ^= ((mds_16[i][j] >> k) & 0x01) * temp[j][k];
			}
			tempsum = tempsum ^ tempmultiply;
		}
		mds_ret = mds_ret << 4;
		mds_ret = mds_ret | tempsum;
	}
	return mds_ret;
}


uint16_t cal_mds_32(uint16_t mds_input) {
	uint8_t temp[4][4] = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (j == 0) {
				temp[i][j] = (mds_input >> ((3 - i) * 4)) & 0x0F;
			}
			else {
				temp[i][j] = Xtime_4(temp[i][j - 1]);
			}
		}
	}

	uint16_t mds_ret = 0;
	for (int i = 0; i < 4; i++) {
		uint8_t tempsum = 0;
		for (int j = 0; j < 4; j++) {
			uint8_t tempmultiply = 0;
			for (int k = 0; k < 4; k++) {
				tempmultiply ^= ((mds_32[i][j] >> k) & 0x01) * temp[j][k];
			}
			tempsum = tempsum ^ tempmultiply;
		}
		mds_ret = mds_ret << 4;
		mds_ret = mds_ret | tempsum;
	}
	return mds_ret;
}



void test_32() {
	

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 g1(seed1);  // mt19937 is a standard mersenne_twister_engine

	uint16_t dif2[15] = {
		0xe9db,
		0xf195,
		0x184e,
		0xd21a,
		0x3bc1,
		0x238f,
		0xca54,
		0x9427,
		0x7dfc,
		0x65b2,
		0x8c69,
		0x463d,
		0xafe6,
		0xb7a8,
		0x5e73
	};
	uint16_t input3, input4, temp3, temp4;
	int count = 0;
	uint16_t dif_32 = 0;
	for (int i = 0; i < 15; i++) {
		input3 = g1();
		input4 = input3 ^ dif2[i];

		for (int j = 0; j < 262144; j++) {

			temp3 = 0;
			temp4 = 0;
			for (int k = 0; k < 4; k++) {
				temp3 = temp4 << 4;
				temp3 = temp4 ^ P[j][(input3 >> (3 - k) * 4) & 0x0F];

				temp4 = temp4 << 4;
				temp4 = temp4 ^ P[j][(input4 >> (3 - k) * 4) & 0x0F];
			}

			dif_32 = temp3 ^ temp4;
			dif_32 = cal_mds_16(dif_32);

			if ((dif_32 & 0x0FFF) == 0) {
				count++;
			}

		}

	}
	cout << count << endl;



}


void test_16() {
	uint32_t dif1[15] = {
		0x73eb816c,
		0xe6f532cb,
		0x951eb3a7,
		0xfcda64b5,
		0x8f31e5d9,
		0x1a2f567e,
		0x69c4d712,
		0xdb97c85a,
		0xa87c4936,
		0x3d62fa91,
		0x4e897bfd,
		0x274dacef,
		0x54a62d83,
		0xc1b89e24,
		0xb2531f48,

	};

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 g1(seed1);  // mt19937 is a standard mersenne_twister_engine

	uint32_t input1, input2, temp1, temp2;


	int count = 0;
	uint32_t dif_16 = 0;
	for (int i = 0; i < 15; i++) {
		input1 = g1();
		input2 = input1 ^ dif1[i];
		for (int j = 0; j < 262144; j++) {

			temp1 = 0;
			temp2 = 0;
			for (int k = 0; k < 8; k++) {
				temp1 = temp1 << 4;
				temp1 = temp1 ^ P[j][(input1 >> (7 - k) * 4) & 0x0F];

				temp2 = temp2 << 4;
				temp2 = temp2 ^ P[j][(input2 >> (7 - k) * 4) & 0x0F];
			}

			dif_16 = temp1 ^ temp2;
			dif_16 = cal_mds_16(dif_16);

			if ((dif_16 & 0x0FFFFFFF) == 0) {
				count++;
			}

		}
	}

	cout << (count) << endl;




}

int main() {

	

	
	/*
	uint32_t input = 0;
	for (int i = 1; i < 16; i++) {
		input = i << 28;
		dif1[i-1] = cal_mds_16(input);

	}

	for (int i = 1; i < 16; i++) {
		input = i << 12;
		dif2[i-1] = cal_mds_32(input);

	}

	cout << endl;
	*/


	
	


	for (int i = 0; i < 1000; i++) {
		test_16();
	}
	
	return 0;


}

