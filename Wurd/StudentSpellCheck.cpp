#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>


SpellCheck* createSpellCheck()
{
    return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
    deleteTrie(rootNode); 
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
    
   ifstream infile (dictionaryFile);
    if (!infile) return false; //couldn't open the file
    
    string s;
    string adjusted;
    
    //O(N) where N is the number of lines
    
    while (getline(infile, s)){ // get each line
        for (int i = 0; i <  s.size(); i++){ // loop through characters (constant upper bound on the length of input line)
            if (isalpha(s[i])){
                adjusted += tolower(s[i]); // lowercase all chars
            }
            if (s[i] == '\''){
                adjusted += s[i];
            }
        }
        if (adjusted.length() > 0){ //add to trie if at least one character remaining
            insert(adjusted);
        }
        adjusted = ""; //reset adjusted
        
    }
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
    for (int i = 0; i < word.length(); i++){
        if (isalpha(word[i])){
            word[i] = tolower(word[i]); // lowercase all letters
        }
    }
    
    bool foundInTrie = find(word); // call function to search for word
    if (foundInTrie == true) return true;
    
    else { // FIND SUGGESTIONS
        if (suggestions.size()!= 0)
        suggestions.clear(); //clear vector
        
        for (int i = 0; i < word.length(); i++){ // same length of word
            for (int j = 0; j < 27; j++){
                if (word[i] != j + 'a' && suggestions.size() < max_suggestions){ // don't replace with same letter
                    string newWord = word;
                    newWord[i] = j + 'a'; //new word
                    if (j == 26) newWord[i] = '\'';
                    
                    //search for new word
                    bool suggestionFound = find(newWord); //O(L^2)
                    if (suggestionFound == true) suggestions.push_back(newWord); // push back onto suggestions vector
                }
            }
            if (suggestions.size() >= max_suggestions) break;
        }

    }
    
    return false; // return with updated suggestions + return false
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
    
    int start = 0;
    int end = 0;
        
 // SEPARATING WORDS AND FINDING POSITIONS //
    string currWord = "";
    if (problems.size() != 0)
    problems.clear(); // clear the vector
    
    // O(s) length of line
    for (int i = 0; i < line.length(); i++){ // loop through line
        if (isalpha(line[i]) || line[i] == '\''){ // check if valid word character
            currWord += line[i];
            end = i; // move the end position
        }
        else { // word ends
            if (currWord.size() > 0){ // check that there's contents in currword
                Position x = {start, end};
                if (find(currWord) == false){ // O(W*L)
                    problems.push_back(x); // only push onto problem stack if not found
                }
                currWord = ""; // reset string
            }
            start = i+1; // reset position tracker
            end = i+1;
        }
    }
    //consider last word of line
    Position lastWord = {start, end};
    if (currWord.size() > 0){
        if (find(currWord) == false) problems.push_back(lastWord);
    }
    
}



// TRIE FUNCTIONS //


StudentSpellCheck:: trie* StudentSpellCheck:: newTrieNode (){
    trie* newNode = new trie;
    newNode->end = false;
    for (int i = 0; i < 27; i++){
        newNode->letters[i] = nullptr; //initialize to nullptr
    }
    return newNode;
}


    
void StudentSpellCheck ::  insert (string word){  //inserting words into trie
    trie* current = rootNode; //pointer to traverse
    
    for (int i = 0; i < word.length(); i++){
        int letterNumber = word[i] - 'a';
        if (word[i] == '\'') letterNumber = 26; //26th index for apostrophe

        
        if (current->letters[letterNumber] == nullptr){ // if path doesn't exist
            
            trie* newInsertion = newTrieNode();
            current->letters[letterNumber] = newInsertion; // add a new tree branch
        }
        current = current->letters[letterNumber]; // move the pointer down the trie

    }
    current->end = true; //set end of word to true
}

bool StudentSpellCheck:: find(string search){
    trie* current = rootNode;
    for (int i = 0; i < search.length(); i++){
        char lowercaseSearch = tolower(search[i]);
        int letterNumber = lowercaseSearch - 'a';
        
        if (search[i] == '\'') letterNumber = 26;
        
        if (current->letters[letterNumber] == nullptr) {
            return false; // not found
        }
         current = current->letters[letterNumber]; // move down the trie
    }
    if (current != nullptr && current->end == true) return true; // return true if all letters found + end of word in dict
    
    return false;
}

void StudentSpellCheck:: deleteTrie(trie* root){
    if (root == nullptr) return; 
    for (int i = 0; i < 27; i++){
        if (root->letters[i] != nullptr) {
            deleteTrie (root->letters[i]);
        }
    }
    delete root;
}
