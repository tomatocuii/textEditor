#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>
using namespace std;

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
    
    struct undoInfo { // struct with info needed for undo 
        Action move;
        int u_row;
        int u_col;
        string u_ch;
    };
    
    stack <undoInfo> changes;
    
    
};

#endif // STUDENTUNDO_H_
