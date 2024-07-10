import random

def factorial(i):
    if i == 0:
        return 1
    else:
        return i*factorial(i-1)

def num2order(num, length=None):
    if length is None:	# 缺省长度为该整数可映射的最小长度
        length = 1
        while factorial(length) <= num:
            length += 1
    elif num >= factorial(length):
        return False
    num_lst = []
    while length != 0:
        length -= 1
        fac = factorial(length)
        num_lst.append(int(num/fac))
        num %= fac
    num_lst.reverse()
    lst = []
    for i in range(len(num_lst)):
        lst.insert(num_lst[i], i+2)
    lst.reverse()
    lst.append(0)
    lst.append(1)
    return lst


P = []

while True:
    r = random.randint(0,87178291200)
    a = num2order(r,14)
    if a in P:
        None
    else:
        P.append(a)
    if len(P) >= 262144:
        break


with open("permutation_18_1.txt", "w") as f:
    for i in P:
        f.write(str(i) + "\n")
