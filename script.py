

Tbox1 = [ 106,230,232,219,82,71,93,235,88,166,9,79,157,104,20,66,
108,54,237,190,129,1,171,89,161,109,214,210,99,203,175,206,
44,249,119,252,152,192,215,140,164,72,36,98,94,153,196,127,
124,184,174,150,250,40,245,165,186,58,130,229,55,116,138,197,
187,120,22,173,133,16,167,170,101,114,195,102,160,216,83,85,
163,60,48,134,144,176,24,122,91,234,67,222,13,202,80,126,
240,156,243,151,35,68,5,181,220,211,77,223,17,115,146,84,
227,208,212,51,45,95,233,78,251,4,47,69,76,7,221,3,
2,213,254,121,162,49,59,241,111,194,143,63,23,199,0,42,
117,244,12,209,14,158,135,183,96,70,112,231,43,123,27,224,
159,87,21,218,253,238,56,75,191,34,62,41,248,38,8,228,
242,90,30,64,52,105,50,147,201,188,19,113,31,149,107,200,
239,37,18,100,29,185,236,193,81,128,39,25,247,217,177,198,
65,28,189,225,180,32,103,154,15,131,246,182,169,142,136,110,
204,145,205,172,148,155,10,139,207,53,178,92,118,97,141,255,
57,132,61,73,179,125,74,26,33,86,46,6,11,137,168,226, ]
Tbox2= [75,182,242,159,71,150,30,52,15,180,81,34,252,0,69,166,
78,98,167,165,3,149,138,91,56,2,24,169,40,164,222,72,
79,236,112,97,156,35,68,211,239,192,43,11,77,245,136,33,
21,8,212,121,64,85,74,116,38,106,92,48,223,83,127,125,
45,225,231,131,23,50,6,218,26,109,183,157,250,243,44,104,
76,57,238,86,255,46,59,60,251,27,135,132,55,7,201,53,
197,137,237,93,175,62,20,213,134,12,73,158,230,65,151,171,
232,153,124,174,10,210,200,107,123,209,189,9,39,102,19,206,
141,61,22,28,17,194,227,160,54,25,202,140,199,94,248,247,
5,226,217,221,184,143,176,154,246,29,129,128,190,42,214,144,
122,110,229,241,51,142,195,114,186,58,111,244,66,249,168,100,
191,36,185,96,155,133,198,49,117,178,90,204,173,47,253,88,
87,101,240,84,152,4,187,233,31,41,177,146,139,188,181,82,
37,103,115,170,215,105,118,163,228,254,219,13,99,32,63,220,
70,145,80,162,148,108,235,67,113,216,18,147,193,179,14,196,
120,119,172,203,16,89,205,234,130,161,224,208,207,95,1,126, ]
Tbox3 = [ 106,38,181,101,200,63,154,95,54,57,34,44,65,73,80,233,
160,191,121,161,202,171,122,9,173,159,251,113,245,24,74,35,
116,13,21,45,206,158,155,199,174,72,190,249,224,225,175,59,
64,107,86,188,30,131,246,14,26,112,87,195,11,18,105,203,
230,177,192,31,185,4,179,214,140,194,120,75,247,217,96,248,
187,229,157,115,90,197,49,137,133,19,78,198,39,180,28,88,
127,55,37,226,20,117,168,145,215,33,85,150,29,123,111,1,
118,110,243,99,240,211,47,204,151,23,109,41,15,12,108,220,
83,218,193,170,209,51,178,84,237,196,67,216,231,22,184,213,
166,102,8,162,40,169,76,130,207,208,136,163,42,135,6,186,
3,91,119,156,92,46,16,244,176,149,147,210,61,227,27,142,
50,141,114,183,222,144,201,52,53,56,172,0,205,253,212,79,
43,189,134,164,182,98,81,60,235,126,125,232,143,254,32,223,
219,255,153,17,82,241,69,77,167,7,239,71,36,252,129,124,
238,104,2,221,5,48,94,10,242,68,103,165,234,89,138,228,
128,132,62,139,66,152,146,100,25,97,70,58,250,148,236,93,]

def GetAi(sbox):
    Ai = [[],[],[],[],[],[],[],[],]
    for i in range(256):
        lsb = sbox[i] & 0x07
        Ai[lsb].append(i)

    for i in Ai:
        print(i)

def GetBj(sbox):
    Bj = [[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],]
    for i in range(256):
        msb = sbox[i] >> 3
        Bj[msb].append(i)

    for i in Bj:
        print(i)

def GetTboxFromAiBj(Ai,Bj):
    sbox = []
    for i in range(256):
        lsb = 0
        msb = 0
        for j in range(8):
            if i in Ai[j]:
                lsb = j
                break
        for j in range(32):
            if i in Bj[j]:
                msb = j
                break
        sbox.append((msb<<3)+lsb) 
    print(sbox)

def MergeSetsAi(pairs):
    n = 0
    while(len(pairs)>8 and n < 500):
        n=n+1
        for i in range(len(pairs)):
            for j in range(i+1, len(pairs)):
                #交集不空
                if pairs[i].intersection(pairs[j]) != set([]):
                    pairs[i].update(pairs[j])
                    pairs[j] = set([])
        L = []
        for i in pairs:
            if i != set([]):
                L.append(i)
        pairs = L
    print(pairs)


def MergeSetsBj(pairs):
    n = 0
    while(len(pairs)>32 and n < 500):
        n=n+1
        for i in range(len(pairs)):
            for j in range(i+1, len(pairs)):
                #交集不空
                if pairs[i].intersection(pairs[j]) != set([]):
                    pairs[i].update(pairs[j])
                    pairs[j] = set([])
        L = []
        for i in pairs:
            if i != set([]):
                L.append(i)
        pairs = L
    print(pairs)
    return pairs



#a = MergeSetsBj(pairs)


Ai= [[128, 4, 11, 140, 15, 149, 23, 161, 39, 40, 47, 53, 54, 57, 58, 59, 63, 198, 74, 75, 209, 82, 86, 91, 93, 96, 230, 107, 110, 241, 120, 121],
[136, 151, 24, 32, 165, 167, 169, 42, 170, 183, 61, 64, 65, 199, 73, 83, 212, 84, 94, 226, 227, 101, 229, 105, 237, 238, 109, 124, 242, 243, 249, 252],
[1, 2, 133, 8, 139, 12, 141, 145, 19, 21, 22, 158, 160, 35, 36, 177, 60, 192, 68, 201, 202, 79, 208, 87, 216, 89, 95, 224, 225, 103, 117, 246],
[5, 134, 137, 150, 156, 30, 34, 41, 172, 173, 180, 181, 182, 185, 188, 62, 194, 196, 200, 204, 207, 80, 215, 98, 234, 112, 114, 115, 254, 122, 123, 126],
[3, 131, 7, 14, 144, 17, 20, 148, 25, 31, 33, 38, 171, 174, 48, 178, 55, 186, 190, 67, 72, 206, 81, 214, 218, 221, 100, 233, 235, 113, 244, 248],
[129, 135, 9, 143, 16, 146, 147, 28, 157, 37, 168, 45, 46, 49, 184, 56, 189, 69, 197, 71, 76, 205, 211, 85, 88, 222, 228, 111, 116, 119, 253, 127],
[130, 6, 10, 138, 18, 27, 155, 29, 164, 43, 44, 176, 50, 179, 187, 193, 66, 195, 203, 77, 219, 220, 223, 99, 102, 231, 108, 239, 118, 247, 251, 255],
[0, 132, 13, 142, 152, 153, 26, 154, 159, 162, 163, 166, 175, 51, 52, 191, 70, 78, 210, 213, 217, 90, 92, 97, 232, 104, 106, 236, 240, 245, 250, 125,]]

Bj = [
    [128, 225, 3, 237, 13, 16, 188, 29], [98, 4, 133, 136, 49, 20, 26, 187], [131, 105, 11, 240, 176, 112, 19, 85], [106, 235, 12, 140, 46, 220, 61, 254], [224, 193, 168, 15, 52, 244, 183, 122], [1, 99, 7, 104, 111, 149, 249, 62], [162, 169, 77, 146, 23, 89, 30, 31], [161, 196, 103, 167, 233, 44, 76, 152], [65, 100, 39, 8, 200, 10, 143, 92], [40, 17, 84, 153, 27, 189, 126, 95], [2, 109, 47, 175, 115, 214, 253, 255], [195, 134, 174, 145, 147, 212, 53, 154], [192, 67, 108, 172, 238, 210, 54, 157], [202, 234, 18, 51, 178, 119, 57, 94], [229, 6, 201, 205, 206, 213, 58, 156], [164, 69, 204, 246, 55, 250, 59, 124], [66, 197, 208, 242, 125, 182, 221, 63], [228, 5, 166, 198, 203, 139, 243, 218], [97, 102, 41, 74, 170, 87, 88, 186], [215, 75, 211, 151, 90, 251, 190, 158], [32, 232, 137, 45, 239, 209, 25, 60], [38, 73, 9, 207, 82, 50, 21, 159], [160, 199, 72, 43, 78, 86, 185, 222], [33, 34, 227, 142, 179, 116, 22, 91], [0, 138, 48, 83, 117, 150, 184, 93], [96, 129, 194, 171, 177, 217, 219, 252], [163, 230, 79, 80, 113, 24, 56, 155], [226, 130, 68, 135, 107, 14, 123, 191], [165, 70, 71, 141, 110, 148, 181, 247], [36, 101, 231, 173, 241, 81, 245, 28], [223, 132, 42, 216, 114, 248, 120, 127], [64, 35, 37, 236, 144, 180, 118, 121]
]
GetTboxFromAiBj(Ai, Bj)


