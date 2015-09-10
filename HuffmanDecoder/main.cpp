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

class Node{
public: 
    char ch;
    Node*left,*right;
    Node(Node*l,Node*r){
        left = l;
        right = r;
    }
    Node(char cha){
        ch=cha;
        left=right=NULL;
    }
};
bitset<8> bits;
int count=8;
ifstream encodedFile("../encoded.txt", ifstream::in);
ofstream decodedFile ("../decoded.txt",ios::out |ios::binary);
char numberOfTreeElements;

bool ReadBit(){
    if (count==8){
        char ch = encodedFile.get();
        bits = bitset<8>(ch);
        count=0;
    };
    bool bit = bits[7-count];    
    count++;
    return bit;
};

char ReadByte(){
    bitset<8> chBits;
    
    for(int i = 0; i<8;i++){   
        if (count==8){
            char ch = encodedFile.get();
            bits = bitset<8>(ch);
            count = 0;
        };
        bool b = bits[7-count];
        chBits[7-i]=b;
        count++;
    }
    return static_cast<char>(chBits.to_ulong());
};

Node* ReadTreeElement(){
        bool b = ReadBit();
        if (b){
            char ch = ReadByte();
            return new Node(ch);
        }else
        {
            return new Node(ReadTreeElement(), ReadTreeElement());
        }
};
/*
 * 
 */

int sh=0;

int main(int argc, char** argv) {
    Node *topNode = ReadTreeElement();
    Node*currentNode = topNode;
    cout<<"\n";
    while(!encodedFile.eof() || count>0){
        bool b = ReadBit();
        if (!b){
            currentNode = currentNode->left;
        }
        else{
            currentNode = currentNode->right;
        }
        if(currentNode->left==NULL){
           decodedFile << currentNode->ch;
           if(((int)currentNode->ch)==-1)break;
           currentNode = topNode;
        }
    }
    encodedFile.close();
    decodedFile.close();
    return 0;
}

