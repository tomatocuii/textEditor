#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <iostream>
#include <fstream>


using namespace std;

TextEditor* createTextEditor(Undo* un)
{
    return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) { // O(1) constant time
     e_row = 0; // initialize current editing position
     e_column = 0;
     editFile.push_back(""); //O(1) push back
     lineIt = editFile.begin();
}

StudentTextEditor::~StudentTextEditor()
{
    editFile.clear(); // clear all lines; destructs strings O(N)
}

bool StudentTextEditor::load(std::string file) {
    ifstream infile (file);
    if (!infile) return false;
        
    reset(); // O(M) where M is lines in preexisting file; empty old contents of text editor
    editFile.clear();//clear first line
    
    string s;
    while (getline(infile, s)){ // THIS LOOP: O(N) where N is number of lines
        if (!s.empty() && s[s.size() - 1] == '\r') //check for carriage return
            s.erase(s.size() - 1); // remove
        editFile.push_back(s);// push line onto list
    }
    lineIt = editFile.begin(); // reset iterator
    return true;
}


bool StudentTextEditor::save(std::string file) {
   ofstream outfile (file);
    if (!outfile) return false;
    
    list <string> :: iterator it = editFile.begin();
    
    for (int i = 0; i < editFile.size(); i++){ // looping through: O(M) where M is number of lines
        outfile << *it << '\n'; // write the lines to the file w/ a newline character
        it++;
    }
    return true;
}


void StudentTextEditor::reset() {
    
    editFile.clear(); // O(N) to delete each element

    getUndo()->clear();  // clear undo state
    
    e_row = 0; e_column = 0; //reset editing position to top of file
    
    editFile.push_back(""); //reset file and iterator
    lineIt = editFile.begin();
}


void StudentTextEditor::move(Dir dir) {
   
    //O(1) constant time complexity
    
    if (dir == UP){
        if (e_row == 0) return;
        e_row--;
        lineIt--; // move iterator up a line
        e_column = e_column>(*lineIt).size() ? (int)(*lineIt).size() : e_column;
        return;
    }
    if (dir == DOWN){
       if (e_row == editFile.size()-1) return; // already on bottom line
        e_row++;
        lineIt++; // move iterator down a line
        e_column = e_column>(*lineIt).size() ? (int)(*lineIt).size() : e_column;
        return;
    }
    if (dir == LEFT){
        if (e_column == 0 && e_row == 0) return; //top row first column
        if (e_column == 0){ // first column
            e_row--; //move row back
            lineIt--; // move iterator up a line
            e_column = (int)(*lineIt).size();
            return;
        }
        e_column --; //move column left
        return;
    }
    if (dir == RIGHT){
        if (e_row == editFile.size()-1 && e_column == (*lineIt).size()) return;
        if (e_column == (*lineIt).size()){
            e_row++;
            lineIt++; // more iterator down a line
            e_column = 0; //move to first column
            return;
        }
        e_column++; // move column right
        return;
    }
    if (dir == HOME){
        e_column = 0; //move to first character
        return;
    }
    if (dir == END){
        e_column = (int)(*lineIt).size();
        return;
    }
    
}





void StudentTextEditor::del() {
    
    if (e_column == (*lineIt).length() && e_row == editFile.size()-1) return;
    
    if (e_column == (*lineIt).length()){
        lineIt++;
        string newLine = (*lineIt); //text on next line
        lineIt = editFile.erase(lineIt); //delete next line
        
        //time complexity for erase is O(L2)
        
        lineIt--; //move iterator back
        (*lineIt) += newLine; //connect next line's text
        
        // time complexity of appending is O(L1)
        
        getUndo()->submit(Undo::Action::JOIN, e_row, e_column);
        return;
    }
    
    char deleted = (*lineIt).at(e_column); // char at position
    (*lineIt).erase(e_column, 1); //delete character at current position
    
    // time complexity of erase is O(L) where L is length of line
    
    getUndo()->submit(Undo::Action::DELETE, e_row, e_column, deleted);
    //track undo with position after and character deleted
    
    return;

}

void StudentTextEditor::backspace() {
    if (e_column == 0 && e_row == 0){
        return;
    }
    if (e_column >0){ // valid position not first letter
        e_column--;
        del(); //submitted to undo in del()
        
        //delete time complexity is O(L)
        
        return;
    }
    if (e_column == 0 || (*lineIt) == ""){ // first letter or empty line
        lineIt--; //move line up
        e_row--; 
        e_column = (int)(*lineIt).length();
        del();//submitted to undo in delete
        
        //delete time complexity for line merge is O(L1+L2)
    }
    
}






void StudentTextEditor::insert(char ch) {
    
    string insertion = "";
    if (ch == '\t'){
        insertion += "    "; //replace tabs with 4 spaces
        (*lineIt).insert(e_column, insertion); // insert function is max O(L)
        
        for (int i = 0; i < 4; i++){
            e_column ++; //move position by 4
            getUndo()->submit(Undo::Action::INSERT, e_row, e_column, ch);// submit space 4 times
        }
    }
    
    else { // insert function is O(L) where L is length of line
        insertion += ch;
        (*lineIt).insert(e_column, insertion); //insert character into string on current line
        e_column++; // move column right
        getUndo()->submit(Undo::Action::INSERT, e_row, e_column, ch);// row and column after insertion
    }
       
}

void StudentTextEditor::enter() {
    
    string lastLine = (*lineIt).substr(0, e_column); //current line in front of split
    string newLine = (*lineIt).substr(e_column, (*lineIt).size()); // line behind split
    // substr is O(L) where L is length
    
    *lineIt = lastLine; // set current line to string in front of enter
    
    lineIt++; //move row down
    lineIt = editFile.insert(lineIt, newLine); // insert new line O(1)
    getUndo()->submit(Undo::Action::SPLIT, e_row, e_column); // submit to UNDO
    
    //insertion is O(1)
    e_row++; // move the row position down
    e_column = 0;
    
}

void StudentTextEditor::getPos(int& row, int& col) const {
    row = e_row;
    col = e_column; // O(1) because it's a constant runtime regardless
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
    
    if (startRow < 0 || numRows < 0 || startRow > editFile.size()) return -1;
    
    lines.clear(); // clear all previous data
    
    // O (oldR) to clear lines
    list <string> :: const_iterator it = lineIt;
    
    int counter = 0;
    for ( int i = 0; i < abs(e_row - startRow); i++){
        if (e_row < startRow) it++; // move iterator to correct row
        else it--;
    }
    
    //time complexity for above loop is abs(current row - startRow)
    
    
    for (int i = startRow; i < startRow + numRows; i++){
        if (it == editFile.end()) break;
        lines.push_back(*it); // push lines onto lines vector
        it++;
        counter++;
    }
    
    return counter;

}

void StudentTextEditor::undo() {
    int row;
    int col;
    int count;
    string text;
    Undo:: Action x = getUndo()->get(row, col, count, text);

    
    
    if (x == Undo::Action::ERROR){
        return;
    }
    
    e_row = row; // reposition cursor
    e_column = col;
    
    
    if (x == Undo::Action:: INSERT){
        (*lineIt).insert(e_column, text);
        
    }
    
    
    else if (x == Undo::Action:: DELETE){
        (*lineIt).erase(e_column, count); //delete character at current position
    }
    
    else if (x == Undo::Action::SPLIT){
        
        string lastLine = (*lineIt).substr(0, e_column);
        string newLine = (*lineIt).substr(e_column, (*lineIt).size());
        *lineIt = lastLine;
        
        lineIt++; //move row down
        lineIt = editFile.insert(lineIt, newLine);
        lineIt--;
        e_row++; // move the row position down
        e_column = 0;
        
 
    }
    
    else if (x == Undo::Action::JOIN){
        string newLine = (*lineIt); //text on next line
        lineIt = editFile.erase(lineIt); //delete next line
        lineIt--; //move iterator back
        (*lineIt) += newLine; //connect next line's text
        
    
    }
    e_row = row;
    e_column = col;
  

}

