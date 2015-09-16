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
#include <sys/stat.h>

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
    bitset<8> bits;
    int count;
    ifstream encodedFile;
    char tempChar;
    char lastChar;
    bool breakRequired;
    bool finalReadingStarted;
    bool lastSymbolMet;
    int leftBitsToRead;
    
    
public:
    BufferedReader(const char*filePath){
        encodedFile.open(filePath, ifstream::in | ios::binary);
        count = 8;
        breakRequired = false;
        finalReadingStarted = false;
        lastSymbolMet = false;                
        tempChar = encodedFile.get();
        lastChar = encodedFile.get();
    }
    ~BufferedReader(){
        encodedFile.close();
    }       
    
    bool get(){
        
    }

    bool ReadBit(){
        if (count==8){
            char ch;
            bits = bitset<8>(tempChar);            
            if (!encodedFile.eof()){
                tempChar = lastChar;
                lastChar = encodedFile.get();
            }
            if (encodedFile.eof()){
                if(!lastSymbolMet){
                    bitset<8> lastByteBits(tempChar);
                    bitset<3> lastThreeBits;
                    for(int i=0;i<3;i++){
                        lastThreeBits[i]=lastByteBits[i];
                    };
                    leftBitsToRead = static_cast<int>(lastThreeBits.to_ulong());                
                    lastSymbolMet = true;
                    if (leftBitsToRead <= 5){
                        leftBitsToRead+=8;
                    }
                }
            }                        
            count=0;
        }
        if(lastSymbolMet)leftBitsToRead--;
        bool bit = bits[7-count];    
        count++;
        return bit;
    };

    char ReadByte(){
        bitset<8> chBits;
    
        for(int i = 0; i<8;i++){   
            if (count==8){
                //char ch = encodedFile.get();
                bits = bitset<8>(tempChar);
                if (!encodedFile.eof()){
                    tempChar = lastChar;
                    lastChar = encodedFile.get();
                }
                if (encodedFile.eof()){
                    if(!lastSymbolMet){
                        bitset<8> lastByteBits(tempChar);
                        
                        bitset<3> lastThreeBits;
                       for(int i=0;i<3;i++){
                           lastThreeBits[i]=lastByteBits[i];
                        };
                        leftBitsToRead = static_cast<int>(lastThreeBits.to_ulong());
                        
                        lastSymbolMet = true;
                        if (leftBitsToRead <= 5){
                            leftBitsToRead+=8;
                        }
                    }
                }               
                count = 0;
            };
            if(lastSymbolMet)leftBitsToRead--;
            bool b = bits[7-count];
            chBits[7-i]=b;
            count++;
        }
        return static_cast<char>(chBits.to_ulong());
    };
    bool EndOfStream(){
        return (lastSymbolMet&&(leftBitsToRead==0));
    }
};

Node* ReadTreeElement(BufferedReader&reader){
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
 * 
 */

int main(int argc, char** argv) {
    // reading file path from argument or setting default one
    const char*filePath;
    if(argc>1) filePath = argv[1];
    else filePath = "../encoded.txt";
    
    //quit if file doesn't exist
    struct stat buffer;
    if(!(stat(filePath,&buffer)==0)){
        cout<<"File doesn't exist";
        return 1;
    }
    BufferedReader reader(filePath);
    
    ofstream decodedFile ("../decoded.txt",ios::out |ios::binary);
    //tempChar = encodedFile.get();
    Node*currentNode;
    Node *topNode;
    topNode = ReadTreeElement(reader);
    currentNode = topNode;
    
    cout<<"\n";
    while(!reader.EndOfStream()){
        bool b = reader.ReadBit();
        if (!b) currentNode = currentNode->left;
        else    currentNode = currentNode->right;
        
        if(currentNode->left==NULL){
           if (reader.EndOfStream())break;
           decodedFile << currentNode->ch;
           currentNode = topNode;           
        }
    }
    decodedFile.close();
    return 0;
}

