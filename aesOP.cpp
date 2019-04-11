//
//  aesOP.cpp
//  aes
//
//  Created by 吴怡凡 on 10/04/2019.
//  Copyright © 2019 evian. All rights reserved.
//

#include "aesOP.hpp"
#include <map>
//#define debug 0

void printtext(bitset<bytelen> b[16]){
    for (int i = 0; i < 16; i ++){
        cout<<b[i]<<" ";
    }
    cout<<endl;
}

string AES::encode(const string plain, const string k){
    bitset<bytelen> roundK[11][16];
    bitset<bytelen> key[16], pltxt[16];
    string cptxt;
    
    // Check the length of key and plaintext
    if (k.length() != 32 || plain.length() != 32){
        return "invalid key or plaintext";
    }
    
    // Initialize bitset from string
    for (int i = 0; i < 32; i += 2){
        key[i / 2] = str2bitset(k.substr(i, 2));
    }
    
    for (int i = 0; i < 32; i += 2){
        pltxt[i / 2] = str2bitset(plain.substr(i, 2));
    }
    
    KeyExpansion(key, roundK);
    
    // First step: xor on plaintext
    for (int i = 0; i < 16; i ++){
        pltxt[i] ^= roundK[0][i];
    }
    
    
    // Second step: Rounds begin
    for (int r = 0; r < nRound; r ++){
        // SubBytes
        bitset<bytelen> roundMat[matsize][matsize];
        for (int i = 0; i < 16; i ++){
            pltxt[i] = SubBytes(pltxt[i]);
        }
        
        
        // ShiftRows
        for (int i = 0; i < matsize; i ++){
            for (int j = 0; j < matsize; j++){
                roundMat[i][j] = pltxt[j * 4 + i];
            }
        }
        
        ShiftRows(roundMat);
        
        
        // MixColumns
        if (r != 9){
            MixColumn(roundMat);
        }
        
        // AddRoundKey
#ifdef debug
        cout<<"Round "<<r<<endl;
        
        for (int i = 0; i < 16; i ++){
            pltxt[i] = roundMat[i % 4][i / 4];
        }
        string tmp;
        for (int i = 0; i < 16; i ++) {
            tmp += bit2hex(pltxt[i]);
        }
        
        cout<<tmp<<endl;
#endif
        for (int i = 0; i < 16; i ++){
            pltxt[i] = roundMat[i % 4][i / 4] ^ roundK[r + 1][i];
        }
        
#ifdef debug
        tmp.clear();
        cout<<"Round "<<r<<endl;
        for (int i = 0; i < 16; i ++) {
            tmp += bit2hex(pltxt[i]);
        }
        
        cout<<tmp<<endl;
#endif
        
    }
    for (int i = 0; i < 16; i ++) {
        cptxt += bit2hex(pltxt[i]);
    }
    
    return cptxt;
}


string AES::decode(const string cipher, const string k){
    bitset<bytelen> roundK[11][16];
    bitset<bytelen> key[16], cptxt[16];
    string pltxt;
    
    // Check the length of key and plaintext
    if (k.length() != 32 || cipher.length() != 32){
        return "invalid key or plaintext";
    }
    
    // Initialize bitset from string
    for (int i = 0; i < 32; i += 2){
        key[i / 2] = str2bitset(k.substr(i, 2));
    }
    
    for (int i = 0; i < 32; i += 2){
        cptxt[i / 2] = str2bitset(cipher.substr(i, 2));
    }
    
    KeyExpansion(key, roundK);
    
    // Second step: Rounds begin
    for (int r = nRound; r > 0 ; r --){
        bitset<bytelen> roundMat[matsize][matsize];
        // AddRoundKey
        for (int i = 0; i < 16; i ++){
            cptxt[i] = cptxt[i] ^ roundK[r][i];
        }
        
        
        for (int i = 0; i < matsize; i ++){
            for (int j = 0; j < matsize; j++){
                roundMat[i][j] = cptxt[j * 4 + i];
            }
        }
        
        // MixColumns
        if (r != nRound){
            inv_MixColumn(roundMat);
        }
        
        //shiftRows
        inv_ShiftRows(roundMat);
        
        
        for (int i = 0; i < 16; i ++){
            cptxt[i] = roundMat[i % 4][i / 4];
        }
        
        // SubBytes
        for (int i = 0; i < 16; i ++){
            cptxt[i] = inv_SubBytes(cptxt[i]);
        }
        
        
    }
    
    for (int i = 0; i < 16; i ++){
        cptxt[i] ^= roundK[0][i];
    }
    
    for (int i = 0; i < 16; i ++) {
        pltxt += bit2hex(cptxt[i]);
    }
    
    return pltxt;
}

void AES::printRoundK(const string k){
    bitset<bytelen> roundK[11][16];
    bitset<bytelen> key[16];
    
    if (k.length() != 32){
        cout<<"invalid key or plaintext"<<endl;
        return;
    }
    
    for (int i = 0; i < 32; i += 2){
        key[i / 2] = str2bitset(k.substr(i, 2));
    }
    
    KeyExpansion(key, roundK);
    
    for (int r = 0; r < 11; r ++){
        string now_K;
        for (int i = 0; i < 16; i ++) {
            now_K += bit2hex(roundK[r][i]);
        }
        //cout<<now_K<<endl;
        cout<<"key for Round "<<r<<": "<<now_K<<endl;
        
    }
}


bitset<bytelen> AES::SubBytes_compute(bitset<bytelen> a){
    // Performing substitution on a S-block
    const bitset<bytelen> c("01100011");
    bitset<bytelen> b;
    
    a = FieldInv(a);
    for (int i = 0; i < bytelen; i++){
        b[i] = (a[i] ^ a[ (i + 4) % bytelen] ^ a[(i + 5) % bytelen] ^ a[(i + 6) % bytelen] ^ a[(i + 7) % bytelen] ^ c[i]);
    }
    return b;
}

bitset<bytelen> AES::SubBytes(bitset<bytelen> a){
    // Substitution by looking up the dict
    return SubByteDict[a.to_ulong()];
}

bitset<bytelen> AES::inv_SubBytes(bitset<bytelen> a){
    // Inverse of Substitution by looking up the dict
    bitset<bytelen> ret;
    for (int i = 0; i < (1<<bytelen); i ++ ){
        if (SubByteDict[i] == a){
            ret = bitset<bytelen>(i);
            break;
        }
    }
    return ret;
}

void AES::ShiftRows(bitset<bytelen> mat[matsize][matsize]){
    bitset<bytelen> tmp_mat[matsize][matsize];
    for (int i = 0; i < matsize; i ++){
        for (int j = 0; j < matsize; j++){
            tmp_mat[i][j] = mat[i][j];
        }
    }
    
    // Shifting
    for (int i = 0; i < matsize; i++){
        for (int j = 0; j < matsize; j++){
            mat[i][j] = tmp_mat[i][(j + i) % matsize];
        }
    }
}

void AES::inv_ShiftRows(bitset<bytelen> mat[matsize][matsize]){
    bitset<bytelen> tmp_mat[matsize][matsize];
    for (int i = 0; i < matsize; i ++){
        for (int j = 0; j < matsize; j++){
            tmp_mat[i][j] = mat[i][j];
        }
    }
    
    // re-Shifting
    for (int i = 0; i < matsize; i++){
        for (int j = 0; j < matsize; j++){
            int cnt = j - i;
            if (cnt < 0){
                cnt += matsize;
            }
            mat[i][j] = tmp_mat[i][cnt];
        }
    }
}

void AES::MixColumn(bitset<bytelen> mat[matsize][matsize]){
    const bitset<bytelen> mult[4] = {2, 3, 1, 1};
    
    bitset<bytelen> tmp_mat[matsize][matsize];
    for (int i = 0; i < matsize; i ++){
        for (int j = 0; j < matsize; j++){
            tmp_mat[i][j] = mat[i][j];
        }
    }
    
    for (int c = 0; c < matsize; c ++){
        // Performing over all columns
        for (int i = 0; i < matsize; i ++){
            mat[i][c].reset();
            for (int j = 0; j < matsize; j ++){
                mat[i][c] = FieldAdd(mat[i][c], FieldMult(mult[j], tmp_mat[(i + j) % matsize][c]));
            }
        }
    }
}

void AES::inv_MixColumn(bitset<bytelen> mat[matsize][matsize]){
    const bitset<bytelen> mult[4] = {14, 11, 13, 9};
    
    bitset<bytelen> tmp_mat[matsize][matsize];
    for (int i = 0; i < matsize; i ++){
        for (int j = 0; j < matsize; j++){
            tmp_mat[i][j] = mat[i][j];
        }
    }
    
    for (int c = 0; c < matsize; c ++){
        // Performing over all columns
        for (int i = 0; i < matsize; i ++){
            mat[i][c].reset();
            for (int j = 0; j < matsize; j ++){
                mat[i][c] = FieldAdd(mat[i][c], FieldMult(mult[j], tmp_mat[(i + j) % matsize][c]));
            }
        }
    }
}

void AES::KeyExpansion(bitset<bytelen> key[16], bitset<bytelen> expKey[11][16]){
    const bitset<bytelen> RCon[10][4] = {
        {1, 0, 0, 0},
        {2, 0, 0, 0},
        {4, 0, 0, 0},
        {8, 0, 0, 0},
        {16, 0, 0, 0},
        {32, 0, 0, 0},
        {64, 0, 0, 0},
        {128, 0, 0, 0},
        {27, 0, 0, 0},
        {54, 0, 0, 0}
    };
    
    bitset<bytelen> w[44][4];
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j ++){
            w[i][j] = key[4 * i + j];
        }
    }
    
    bitset<bytelen> tmp[4];
    for (int i = 4; i < 44; i ++){
        for (int j = 0; j < 4; j++){
            tmp[j] = w[i - 1][j];
        }
        if (i % 4 == 0){
            RotWord(tmp);
            for (int j = 0; j < 4; j ++){
                tmp[j] = SubBytes(tmp[j]);
            }
            for (int j = 0; j < 4; j ++){
                tmp[j] ^= RCon[i / 4 - 1][j];
            }
        }
        for (int j = 0; j < 4; j++){
            w[i][j] = w[i - 4][j] ^ tmp[j];
        }
    }
    
    for (int i = 0; i < 44; i ++){
        for (int j = 0; j < 4; j ++){
            int pos = i * 4 + j;
            expKey[pos / 16][pos % 16] = w[i][j];
        }
    }
}

void AES::RotWord(bitset<bytelen> b[4]){
    bitset<bytelen> tmp_b[4];
    for (int i = 0; i < 4; i++){
        tmp_b[i] = b[i];
    }
    for (int i = 0; i < 4; i ++){
        b[i] = tmp_b[(i + 1) % 4];
    }
}

bitset<bytelen> AES::str2bitset(const string str){
    string tmp;
    tmp = hex2bin(str[0]) + hex2bin(str[1]);
    return bitset<bytelen>(tmp);
}

string AES::bit2hex(bitset<bytelen> bitk){
    int bitnum[2];
    bitnum[0] = ( ((int) bitk.to_ulong()) >> 4);
    bitnum[1] = ( ((int) bitk.to_ulong()) & 0xf );
    string ret;
    for (int l = 0; l < 2; l ++){
        if (bitnum[l] >= 0 && bitnum[l] < 10){
            char tmp = '0' + bitnum[l];
            ret += tmp;
        }
        else if (bitnum[l] >= 10 && bitnum[l] < 16){
            char tmp = 'A' + bitnum[l] - 10;
            ret += tmp;
        }
    }
    
    return ret;
}

string AES::hex2bin(const char hexa){
    switch (hexa) {
        case '0':
            return "0000";
            break;
        case '1':
            return "0001";
            break;
        case '2':
            return "0010";
            break;
        case '3':
            return "0011";
            break;
        case '4':
            return "0100";
            break;
        case '5':
            return "0101";
            break;
        case '6':
            return "0110";
            break;
        case '7':
            return "0111";
            break;
        case '8':
            return "1000";
            break;
        case '9':
            return "1001";
            break;
        case 'A':
        case 'a':
            return "1010";
            break;
        case 'B':
        case 'b':
            return "1011";
            break;
        case 'C':
        case 'c':
            return "1100";
            break;
        case 'D':
        case 'd':
            return "1101";
            break;
        case 'E':
        case 'e':
            return "1110";
            break;
        case 'F':
        case 'f':
            return "1111";
            break;
        default:
            cout << "\nInvalid hexadecimal digit "<<hex<<endl;
            return "invalid";
    }
}





