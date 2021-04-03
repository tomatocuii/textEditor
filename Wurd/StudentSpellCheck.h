#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>
using namespace std;

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() {
        rootNode = newTrieNode();
    }
    virtual ~StudentSpellCheck();
    bool load(std::string dict_file);
    bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
    void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:

    
    struct trie {
        trie *letters [27]; //all alphabet letters + apostrophe, initialize as nullptrs
        bool end; //end of word
    };
    
    void insert (string word);
    bool find (string search);
    
    
    trie* rootNode;
    void deleteTrie (trie* root); 
    trie* newTrieNode (); 
    
};

#endif  // STUDENTSPELLCHECK_H_
