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

class BufferedReader{
    int count=8;
    bitset<8> bits;
    ifstream encodedFile;
    char tempChar;
public: 
    BufferedReader(char*filePath){
        tempChar = encodedFile.get();
        encodedFile.open(filePath, ifstream::in | ios::binary);
    }
    ~BufferedReader(){
        encodedFile.close();
    }
    bool ReadBit(){
    if (count==8){
        bits = bitset<8>(tempChar);
        tempChar = encodedFile.get();
        count=0;
    };
    bool bit = bits[7-count];    
    count++;
    return bit;
    }
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
    }
    bool EndOfStream(){
        return encodedFile.eof();
    }
};

Node* ReadTreeElement(BufferedReader reader){
        bool b = reader.ReadBit();
        if (b){
            char ch = reader.ReadByte();
            return new Node(ch);
        }else
        {
            return new Node(ReadTreeElement(reader), ReadTreeElement(reader));
        }
};

/*
 * Decodes original file from its compressed/encoded image file
 */

int main(int argc, char** argv) {
    BufferedReader reader;
    //getting location of encoded file in the argument or setting default one
    const char *fileLocation;
    if(argc>1) fileLocation = argv[1];
    else fileLocation = "../encoded.txt";

    //checking if encoded file exists
    struct stat buffer;    
    if (stat(fileLocation,  &buffer)!=0){
        cout<<"Input file cannot be open";
        return 1;
    } else{
        reader = new BufferedReader(fileLocation);
    }
    
    Node *currentNode;
    Node *topNode;
    if(!(((int)reader.ReadByte())==-1)){
        topNode = ReadTreeElement(reader);
        currentNode = topNode;
    }
    cout<<"\n";
    
    ofstream decodedFile("../decoded.txt",ios::out |ios::binary);    
    while(!reader.EndOfStream()){
        bool b = reader.ReadBit();
        if (!b) currentNode = currentNode->left;
        else    currentNode = currentNode->right;
        
        if(currentNode->left==NULL){
           if (reader.EndOfStream()) break;
           
           decodedFile << currentNode->ch;
           currentNode = topNode;           
        }
    }
    decodedFile.close();
    return 0;
}

