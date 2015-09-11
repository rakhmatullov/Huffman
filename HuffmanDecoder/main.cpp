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
ifstream encodedFile("../encoded.txt", ifstream::in | ios::binary);
ofstream decodedFile ("../decoded.txt",ios::out |ios::binary);
char tempChar;
int sh=0;
bool breakRequired=true;

bool ReadBit(){
    if (count==8){
        char ch;
        if (breakRequired) {ch = tempChar; breakRequired = false;}
        else ch = encodedFile.get();
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

int main(int argc, char** argv) {
    tempChar = encodedFile.get();
    Node*currentNode;
    Node *topNode;
    if(!(((int)tempChar)==-1)){
        topNode = ReadTreeElement();
        currentNode = topNode;
    }
    cout<<"\n";
    while(!encodedFile.eof() || count<8){
        bool b = ReadBit();
        if (!b) currentNode = currentNode->left;
        else    currentNode = currentNode->right;
        
        if(currentNode->left==NULL){
           if (encodedFile.eof())break;
           if(((int)currentNode->ch)==-1){
               breakRequired=true;
               tempChar = encodedFile.get();
               if (encodedFile.eof()) break;
           }
           decodedFile << currentNode->ch;
           currentNode = topNode;           
        }
    }
    encodedFile.close();
    decodedFile.close();
    return 0;
}

