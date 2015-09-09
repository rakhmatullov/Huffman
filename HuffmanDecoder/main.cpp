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
//int currentNumber=0;
ifstream encodedFile("../encoded.txt", ifstream::in);
char numberOfTreeElements;

bool ReadBit(){
    if (count==8){
        encodedFile >> bits;
        cout<<bits;        
        count=0;        
    };
    bool bit = bits[7-count];    
    count++;
    return bit;
};

char ReadByte(){
    bitset<8> chBits;
    
    for(int i = 0; i<8;i++){        
        chBits[7-i]=bits[7-count];
        count++;
        if (count==8){
            encodedFile >> bits;
            cout << bits;            
            count = 0;            
        };
    }
    return static_cast<char>(chBits.to_ulong());
};

Node* ReadTreeElement(){
  //      currentNumber++;
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
    //numberOfTreeElements = encodedFile.get();
    //cout<< "Number of tree elements: " << (int)numberOfTreeElements <<"\n";
    Node *topNode = ReadTreeElement();
    Node*currentNode = topNode;
    
    //vector<char>symbols;
    while(!encodedFile.eof()&&count<8){
        bool b = ReadBit();
        if (!b){
            currentNode = currentNode->left;
        }
        else{
            currentNode = currentNode->right;
        }
        if(currentNode->left==NULL){
           cout << currentNode->ch;
           currentNode = topNode;
        }
    }
    /*
    for(int i = 0; i<symbols.size();i++){
        cout << symbols[i];
    }
    cout << "\n";
    for(vector<char>::iterator it = symbols.begin(); it!=symbols.end();it++){
        std::bitset<8> bits(*it);            
        cout << bits;
    } 
    */
    encodedFile.close();
    return 0;
}

