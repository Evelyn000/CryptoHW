//
//  main.cpp
//  aes
//
//  Created by 吴怡凡 on 10/04/2019.
//  Copyright © 2019 evian. All rights reserved.
//

#include <iostream>
#include <bitset>
#include <string>
#include "fieldop.hpp"
#include "aesOP.hpp"
using namespace std;
int main(int argc, const char * argv[]) {
    AES aes(128);
    
    string pltxt, k;
    string cptxt;
    int cnt;
    while (true){
        cout<<"Welcome to Yifan's AES-128 test!"<<endl<<"Please choose a functionality from below:"<<endl<<"1. encryption,"<<endl<<"2. decryption,"<<endl<<"3. Exit."<<endl;
        cin>>cnt;
        switch (cnt) {
            case 1:
                cout<<endl<<"Please input plaintext for encryption:"<<endl;
                cin>>pltxt;
                cout<<"Please input key for encryption:"<<endl;
                cin>>k;
                aes.printRoundK(k);
                cptxt = aes.encode(pltxt, k);
                cout<<"plaintext: "<<pltxt<<endl;
                cout<<"key: "<<k<<endl;
                cout<<"encoded string: "<<cptxt<<endl<<endl;
                break;
            
            case 2:
                cout<<endl<<"Please input ciphertext for decryption:"<<endl;
                cin>>cptxt;
                cout<<"Please input key for decryption:"<<endl;
                cin>>k;
                cout<<"ciphertext: "<<cptxt<<endl;
                cout<<"key: "<<k<<endl;
                cout<<"decoded string: "<<aes.decode(cptxt, k)<<endl<<endl;
                break;
            default:
                cout<<"Exiting..."<<endl<<"See you next time ;)"<<endl;
                return 0;
        }
        
        
        
    }
    
    return 0;
}
