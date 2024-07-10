The target cipher has 32 bits block length. T-box used is 8 bits, with m = 5, t = 3. 
Before the attack begin, we have generated the 3-bit permutations P[720] and T-box  in advance. 
If you want to change to other T-boxes, please run the function gen_Tbox(Tbox1) and gen_Tbox_inv(Tbox1) and modify the corresponding data according to the result.
And the permutations can also be replaced only if they are the representative of the affine equivalence class.



For the simplicity of the code, we directly add permutation after the original T-box to find the correct permutation in the first round.
The Steps are as follows:
Step1: use the function DivideIntoAi_SecondRound() to get the 8-bit pairs set {p1,p2} that lsb_3(p1)=lsb_3(p2). Then use the function MergeSetsAi(pairs) to merge the sets whose intersection is not empty. Thus, we obtain Ai, and the value i is unknown.
Step2: we first need to get 2^{10} good pairs by using the function GetGoodPairs_FirstRound(), and replace your data into the corresponding variables goodpairs[1024][2].
Step3: use the function GetCorrectPermutation_FirstRound(goodpairs) to find the right permutation. (Our result is the 66-th permutation which is a linear transform).In this point, we get the subscript of Ai. Thus, write Ai into the funtion 
Step4: use the function GetPossibleRightPairs_FirstRound(66, goodpairs) to get possible right pairs. And write the data to possible_right_paris[][2].
Step5: use the function GetRightParis_FirstRound(possible_right_paris) to get right pairs
Step6: use the function DivideIntoBj_FirstRound(right_pairs, Ai) to get Bj, and the subscripts j are arbitrary. In this point, we can recover the T-box in first round.

Next, we write T-box recovered to Tbox1_recovered[256] and then continue to recover the second round.
The difference from the recovery of the first round is that there is an additional partial decryption process. In the CC set it is a  partial encryption process.
We ignore the description of this process

Finally, we obtain a new cipher Recoveryoroi(), which has the same functionality with yoroi().

------------------------------

Before testing Rpw, we should use genration


