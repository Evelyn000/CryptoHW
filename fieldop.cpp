//
//  fieldop.cpp
//  aes
//
//  Created by 吴怡凡 on 10/04/2019.
//  Copyright © 2019 evian. All rights reserved.
//

#include "fieldop.hpp"
#include <stdio.h>
using namespace std;

bitset<bytelen> FieldInv(const bitset<bytelen> ele){
    //  A quick dictionary for elements' inverse over multiplication
    const bitset<bytelen> dict[1<<bytelen]  = {
        0, 1, 141, 246, 203, 82, 123, 209, 232, 79, 41, 192, 176, 225, 229, 199, 116, 180, 170, 75, 153, 43, 96, 95, 88, 63, 253, 204, 255, 64, 238, 178, 58, 110, 90, 241, 85, 77, 168, 201, 193, 10, 152, 21, 48, 68, 162, 194, 44, 69, 146, 108, 243, 57, 102, 66, 242, 53, 32, 111, 119, 187, 89, 25, 29, 254, 55, 103, 45, 49, 245, 105, 167, 100, 171, 19, 84, 37, 233, 9, 237, 92, 5, 202, 76, 36, 135, 191, 24, 62, 34, 240, 81, 236, 97, 23, 22, 94, 175, 211, 73, 166, 54, 67, 244, 71, 145, 223, 51, 147, 33, 59, 121, 183, 151, 133, 16, 181, 186, 60, 182, 112, 208, 6, 161, 250, 129, 130, 131, 126, 127, 128, 150, 115, 190, 86, 155, 158, 149, 217, 247, 2, 185, 164, 222, 106, 50, 109, 216, 138, 132, 114, 42, 20, 159, 136, 249, 220, 137, 154, 251, 124, 46, 195, 143, 184, 101, 72, 38, 200, 18, 74, 206, 231, 210, 98, 12, 224, 31, 239, 17, 117, 120, 113, 165, 142, 118, 61, 189, 188, 134, 87, 11, 40, 47, 163, 218, 212, 228, 15, 169, 39, 83, 4, 27, 252, 172, 230, 122, 7, 174, 99, 197, 219, 226, 234, 148, 139, 196, 213, 157, 248, 144, 107, 177, 13, 214, 235, 198, 14, 207, 173, 8, 78, 215, 227, 93, 80, 30, 179, 91, 35, 56, 52, 104, 70, 3, 140, 221, 156, 125, 160, 205, 26, 65, 28
    };
    return dict[ele.to_ulong()];
}

bitset<bytelen> FieldMult(const bitset<bytelen> ele1, const bitset<bytelen> ele2){
    const bitset<bytelen * 2> modfunc("0000000100011011");
    bitset<bytelen> ret;
    bitset<bytelen * 2> a(ele1.to_ulong()), b(ele2.to_ulong()), mul;
    mul.reset();
    
    // Firstly multiply a by b
    for (int k = 0; k < bytelen; k++){
        if (b[k]){
            mul ^= (a<<k);
        }
    }
    
    // Then compute the result over the field
    for (int k = 15; k>=8; k--){
        if (mul[k]){
            mul ^= (modfunc << (k-8));
        }
    }
    
    // Cut the return value to 8 bits
    for (int i = 0; i < bytelen; i++){
        ret[i] = mul[i];
    }
    
    return ret;
}



bitset<bytelen> FieldAdd(const bitset<bytelen> ele1, const bitset<bytelen> ele2){
    // A simple Add OP with XOR
    return ele1 ^ ele2;
}
