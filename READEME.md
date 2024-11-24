# Code for "A More Pratical Attack against Yoroi"

## Environment
- g++ 9.4 or higher

## Code Structure
- get_correct_pairs.cpp:  The experiment that tests how many right pairs can be collected on Yoroi-16.
- recover_correct_permutation.cpp: The experiment that tests how many right pairs recover the correct permutation on Yoroi-32.
- test_Rw.cpp: The experment test the value of Rw. 


## Reproduce
- Compile and execute these codes, and the results are stored in the same path as the code.
- For "get_correct_pairs.cpp", the results are saved in "correct_pairs_tested2_.txt". Run 1000 experiments at a time, and separate the results with "-----", A number greater than 2 means a correct pair has been found.
- For "recover_correct_permutation.cpp", the result is saved in "". Run 1 experiment with 8 correct pairs at a time, The number represents the number of correct permutations left
- For "test_Rw.cpp", the result is saved in "", Run 1000 experiments at a time. Each time, 2^20 candidate permutations are randomly selected. The results represent the number of correct permutations recommended for a correct pair.
- Have fun!

