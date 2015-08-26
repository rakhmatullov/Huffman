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

map<char, vector<bool> > table;
vector<bool> code;

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

    cout << "Number of symbols" << " : "<<symbolsWeights.size() << "\n";
    
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
    
    cout << "\n";
    
    for(map<char, vector<bool> >::iterator  it=table.begin(); it!=table.end();it++){
        cout << it->first << " ";
        vector<bool> sec = it->second;
        for(int i = 0; i<sec.size();i++){
            cout << sec[i]?0:1;
        }
        cout << "\n";
            
    }
    
    inputFile.clear();inputFile.seekg(0);
    
    ofstream outputFile("../encoded.txt",ios::out |ios::binary);       
    int count = 0; char buf;
    cout << "\n";
    while(!inputFile.eof()){
        char c = inputFile.get();
        vector<bool> var = table[c];
        for(int i = 0; i < var.size();i++){
            buf = buf | var[i]<<(7-count);
            count++;
            cout << var[i];
            if (count==8){outputFile<<buf;buf = 0;};            
        }
    }
    inputFile.close();
    outputFile.close();
        
    return 0;
}

