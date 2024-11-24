#include <iostream>
#include <chrono>
#include <random>
#include <fstream> 
#include <ctime>
#include <iomanip>
#include <algorithm>

uint8_t Xtime_4(uint8_t x) {
	return (((x << 1)) ^ ((x & 0x08) ? 0x03 : 0x00)) & 0x0F;
}

uint8_t mds[4][4] = {
	2,3,1,1,
	1,2,3,1,
	1,1,2,3,
	3,1,1,2
};

uint16_t cal_mds_16bit(uint16_t mds_input, uint8_t mds[][4]) {
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
				tempmultiply ^= ((mds[i][j] >> k) & 0x01) * temp[j][k];
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



void recover_lsb() {

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 g1(4);

	uint16_t diffs[15] = {
		59867, 61845, 6222, 53786, 
		15297, 9103, 51796, 37927, 
		32252, 26034, 35945, 17981, 
		45030, 47016, 24179
	};

	uint16_t correct_pairs[8][2] = { 0 };

	uint16_t r1, r2;

	for (int i = 0; i < 8; i++) {
		r1 = g1() & 0x0FFFF;
		r2 = g1() % 15;
		correct_pairs[i][0] = r1;
		correct_pairs[i][1] = r1 ^ diffs[r2];
	}

	uint8_t correct_pairs2[8][2][4] = { 0 };
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 4; k++) {
				correct_pairs2[i][j][k] = (correct_pairs[i][j] >> ((3 - k) * 4)) & 0x0F;
			}
		}
	}


	std::vector<unsigned long long> rest;

	std::vector<int> permutation;
	uint32_t mds_io1, mds_io2;



     
	for (unsigned long long i = 0; i < 87178291200 ; i++) {
		permutation = num2order(i);
		mds_io1 = 0;
		mds_io2 = 0;
		for (int i = 0; i < 4; i++) {
			mds_io1 = mds_io1 << 4;
			mds_io1 = mds_io1 ^ permutation[correct_pairs2[0][0][i]];
			mds_io2 = mds_io2 << 4;
			mds_io2 = mds_io2 ^ permutation[correct_pairs2[0][1][i]];
		}
		mds_io1 = cal_mds_16bit(mds_io1, mds);
		mds_io2 = cal_mds_16bit(mds_io2, mds);
		if (((mds_io1 ^ mds_io2) & 0x0FFF) == 0) {
			rest.push_back(i);
		}
	}
	
	std::ofstream ofs;
	ofs.open("results.txt", std::ios::app);
	ofs << "one correct:" << rest.size() << std::endl;
	std::cout<< "one correct:" << rest.size() << std::endl;
	for (int i = 1; i < 7; i++) {
		

		for (auto it = rest.begin(); it != rest.end();) {
			permutation = num2order(*it);

			mds_io1 = 0;
			mds_io2 = 0;
			for (int j = 0; j < 4; j++) {
				mds_io1 = mds_io1 << 4;
				mds_io1 = mds_io1 ^ permutation[correct_pairs2[i][0][j]];
				mds_io2 = mds_io2 << 4;
				mds_io2 = mds_io2 ^ permutation[correct_pairs2[i][1][j]];
			}
			mds_io1 = cal_mds_16bit(mds_io1, mds);
			mds_io2 = cal_mds_16bit(mds_io2, mds);
			if (((mds_io1 ^ mds_io2) & 0x0FFF) != 0) {
				it = rest.erase(it);
			}
			else
			{
				++it;
			}

		}

		ofs << "another:" << rest.size() << std::endl;
		std::cout << "another:" << rest.size() << std::endl;

	}
	ofs << "-------" << std::endl;
	ofs.close();

}


int main() {
	recover_lsb();
	return 0;
}
