/* 
 * File:   main.cpp
 * Author: User
 *
 * Created on August 25, 2015, 3:49 PM
 */

#include <cstdlib>
#include <map>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <bitset>

using namespace std;

class Node{
public: 
    char ch;
    int a;    
    Node *left, *right;    
    
    Node(){left=right=NULL;}
    Node(Node*l, Node*r){
        left = l;
        right = r;
        a = l->a+r->a;
    }
};

struct CompareElements{
    bool operator()(const Node * first, const Node * second) const{
        return first->a < second->a;
    }
};

ofstream outputFile("../encoded.txt",ios::out |ios::binary);  

char buf;
bitset<8> bits;
int count=0;
map<char, vector<bool> > table;
vector<bool> code;


WriteBit(bool bit){
    bits[7-count]=bit;
    count++;
    if (count==8){
        outputFile << static_cast<char>(bits.to_ulong());
        //cout<<bits;
        bits.reset();
        count=0;
        
    };
}

WriteByte(char ch){
    bitset<8> chBits(ch);
    for(int i = 0; i<8;i++){        
        bits[7-count]=chBits[7-i];
        count++;
        if (count==8){
            outputFile << static_cast<char>(bits.to_ulong());
            //cout << bits;
            bits.reset();
            count =0;            
        };
    }
}

WriteNodes(Node*node){
    if (node->left == NULL && node->right == NULL){
        WriteBit(true);
        WriteByte(node->ch);
        }
    else {
        WriteBit(false);
        WriteNodes(node->left);
        WriteNodes(node->right);
        }
    }

BuildTable(Node * el){
    
        
    if (el->left!=NULL){
        code.push_back(0);
        BuildTable(el->left);
    }
    
    if (el->right!=NULL){
        code.push_back(1);
        BuildTable(el->right);
    }  
    
    if(el->left == NULL && el->right == NULL) {
        table[el->ch]=code;
    }
    code.pop_back();
}

/*
 * 
 */
int main(int argc, char** argv) {        
    ifstream inputFile("../text.txt", ifstream::in);
    
    map<char, int> symbolsWeights;
    
    while(!inputFile.eof()){
        char ch = inputFile.get();
        symbolsWeights[ch]++;        
    }    

    list<Node*> elements;
    
    for(map<char, int>::iterator it=symbolsWeights.begin();it!=symbolsWeights.end();it++){
        Node * el = new Node();
        el->ch = it->first;
        el->a = it->second;
        elements.push_back(el);
    }
    
    while(elements.size()>1){
        elements.sort(CompareElements());    
        Node * left = elements.front();
        elements.pop_front();
        Node * right = elements.front();
        elements.pop_front();
        Node * parent = new Node(left, right);
        elements.push_back(parent);
    }
    
    Node * root = elements.front();
    
    BuildTable(root);
    
    inputFile.clear();inputFile.seekg(0);
    
    WriteNodes(root);
    
    while(!inputFile.eof()){
        char c = inputFile.get();
        vector<bool> var = table[c];
        for(int i = 0; i < var.size();i++){
            bool b = var[i];
            bits[7-count]=var[i];
            count++;
            if (count==8){
                outputFile<<static_cast<char>(bits.to_ulong());bits.reset();count=0;
            }            
        }   
    }   
    
    if(count>0){
      outputFile<<static_cast<char>(bits.to_ulong());
    }

    inputFile.close();
    outputFile.close();
        
    return 0;
}
