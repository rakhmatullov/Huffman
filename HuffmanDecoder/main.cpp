/* 
 * File:   main.cpp
 * Author: User
 *
 * Created on September 3, 2015, 3:59 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
using namespace std;

ReadBit(){
    
}

ReadByte(){
    
}
ReadTreeElement(){
    
}
/*
 * 
 */
int main(int argc, char** argv) {
    ifstream encodedFile("../encoded.txt", ifstream::in);
    
    char numberOfTreeElements = encodedFile.get();
    cout<< "Number of tree elements: " << (int)numberOfTreeElements <<"\n";
    ReadTreeElement();
    vector<char>symbols;
    while(!encodedFile.eof()){
        symbols.push_back(encodedFile.get());        
    }
    
    for(int i = 0; i<symbols.size();i++){
        cout << symbols[i];
    }
    cout << "\n";
    for(vector<char>::iterator it = symbols.begin(); it!=symbols.end();it++){
        std::bitset<8> bits(*it);            
        cout << bits;
    }
    encodedFile.close();
    return 0;
}

