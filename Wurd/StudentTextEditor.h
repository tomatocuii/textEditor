#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_
#include "TextEditor.h"
#include <string>
#include <list>

using namespace std;

class Undo;

class StudentTextEditor : public TextEditor {
public:

    StudentTextEditor(Undo* undo);
    ~StudentTextEditor();
    bool load(std::string file);
    bool save(std::string file);
    void reset();
    void move(Dir dir);
    void del();
    void backspace();
    void insert(char ch);
    void enter();
    void getPos(int& row, int& col) const;
    int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
    void undo();

private:
    int e_row; // current editing position
    int e_column;
    list <string> :: iterator lineIt;
    list <string> editFile;
    
};

#endif // STUDENTTEXTEDITOR_H_
