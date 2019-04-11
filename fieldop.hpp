//
//  fieldop.hpp
//  aes
//
//  Created by 吴怡凡 on 10/04/2019.
//  Copyright © 2019 evian. All rights reserved.
//

#ifndef fieldop_hpp
#define fieldop_hpp

#include <stdio.h>
#include <bitset>
using namespace std;

#define bytelen 8

bitset<bytelen> FieldInv(const bitset<bytelen> ele);
bitset<bytelen> FieldMult(const bitset<bytelen> ele1, const bitset<bytelen> ele2);
bitset<bytelen> FieldAdd(const bitset<bytelen> ele1, const bitset<bytelen> ele2);

#endif /* fieldop_hpp */
