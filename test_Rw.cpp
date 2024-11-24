#include <stdint.h>
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <algorithm>

using namespace std;
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

std::vector<uint64_t> random_number() {
	const uint64_t num_count = 1048576;
	const uint64_t range_min = 0;         
	const uint64_t range_max = 87178291199;

	std::unordered_set<uint64_t> unique_numbers;
	std::vector<uint64_t> random_numbers;

	

	std::mt19937 g2(64);
	std::uniform_int_distribution<> dist(range_min, range_max);

	
	while (unique_numbers.size() < num_count) {
		uint64_t num = dist(g2);
		if (unique_numbers.insert(num).second) {
			random_numbers.push_back(num);
		}
	}

	
	
	//std::cout << std::endl;
	return random_numbers;
}

void test_32(vector<uint64_t> permutations) {

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
	uint16_t input1, input2;
	uint16_t mds_io1, mds_io2;
	int count = 0;
	uint16_t dif_o = 0;
	uint8_t random_diff = g1() % 15;
	input1 = g1() & 0x0FFFF;
	input2 = input1 ^ dif2[random_diff];

	uint8_t correct_pairs[2][4] = {0};

	for (int i = 0; i < 4; i++) {
		correct_pairs[0][i] = (input1 >> ((3 - i) * 4)) & 0x0F;
		correct_pairs[1][i] = (input2 >> ((3 - i) * 4)) & 0x0F;
	}
	vector<int> permutation;

	for (auto it = permutations.begin(); it != permutations.end();++it) {
		permutation = num2order(*it);
		mds_io1 = 0;
		mds_io2 = 0;
		for (int i = 0; i < 4; i++) {
			mds_io1 = mds_io1 << 4;
			mds_io1 = mds_io1 ^ permutation[correct_pairs[0][i]];
			mds_io2 = mds_io2 << 4;
			mds_io2 = mds_io2 ^ permutation[correct_pairs[1][i]];
		}

		 
		dif_o = cal_mds_32(mds_io1 ^ mds_io2);

		if ((dif_o & 0x0FFF) == 0) {
			count++;
		}

	}

	
	ofstream ofs;
	ofs.open("yoroi_32_20.txt", ios::app);
	ofs << count << endl;
	ofs.close();

}


void test_16(vector<uint64_t> permutations) {

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 g1(seed1);  // mt19937 is a standard mersenne_twister_engine
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
	uint32_t input1, input2;
	uint32_t mds_io1, mds_io2;
	int count = 0;
	uint32_t dif_o = 0;
	uint8_t random_diff = g1() % 15;
	input1 = g1();
	input2 = input1 ^ dif1[random_diff];

	uint8_t correct_pairs[2][8] = { 0 };

	for (int i = 0; i < 8; i++) {
		correct_pairs[0][i] = (input1 >> ((7 - i) * 4)) & 0x0F;
		correct_pairs[1][i] = (input2 >> ((7 - i) * 4)) & 0x0F;
	}

	vector<int> permutation;

	for (auto it = permutations.begin(); it != permutations.end(); ++it) {
		permutation = num2order(*it);
		mds_io1 = 0;
		mds_io2 = 0;
		for (int i = 0; i < 8; i++) {
			mds_io1 = mds_io1 << 4;
			mds_io1 = mds_io1 ^ permutation[correct_pairs[0][i]];
			mds_io2 = mds_io2 << 4;
			mds_io2 = mds_io2 ^ permutation[correct_pairs[1][i]];
		}

		dif_o = cal_mds_16(mds_io1 ^ mds_io2);
		if ((dif_o & 0x0FFFFFFF) == 0) {
			count++;
		}
	}
	ofstream ofs;
	ofs.open("yoroi_16_20.txt", ios::app);
	ofs << count << endl;
	ofs.close();
}
int main() {
	for (int i = 0; i < 1000; i++) {
		vector<uint64_t> permutations = random_number();
		test_16(permutations);
		test_32(permutations);
	}
	return 0;
}
