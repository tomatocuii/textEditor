#include "StudentUndo.h"



Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
    undoInfo temp;
    
    
    string line = "";
    line += ch;
    

   if (!changes.empty()){ // if stack isnt empty
    
      if (action == DELETE){ // consecutive backspaces or deletions
           if (changes.top().move == DELETE){
               if (changes.top().u_row == row){ // same row
                   
                   
                   if (changes.top().u_col == col+1|| changes.top().u_col == col + changes.top().u_ch.size() + 1) // same position or consecutive positions
                   {
                       col = changes.top().u_col;
                       row = changes.top().u_row;
                       line = ch + changes.top().u_ch; // O(line length)
                        col--;
                       
                       changes.pop(); // set string to consecutive deletions and pop off top of stack
                   }
                   
                   else if (changes.top().u_col == col){ // if it's deletions in same position
                       col = changes.top().u_col;
                       row = changes.top().u_row;
                       line =  changes.top().u_ch + ch;

                       
                       changes.pop();
                   }
               }
           }
       }
       
   
       if (action == INSERT){ // consecutive insertions
           if (changes.top().move == INSERT){ // check top of stack is an insertion
               if (changes.top().u_row == row && changes.top().u_col == col - changes.top().u_ch.size() - 1){ // check position for consecutives & same row
                        col = changes.top().u_col;
                        row = changes.top().u_row;
                        line = changes.top().u_ch + ch;
                        col++;
                   
                   changes.pop(); // pop last insertion off stack
               }
            }
       }
   }

   
    if (action == INSERT) col--;
    
    undoInfo toPush;
    toPush.move = action;
    toPush.u_row = row;
    toPush.u_col = col;
    toPush.u_ch = line;
    
    changes.push (toPush); //push action with info for undo onto stack
    
    //O(1) to push onto stack

}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    
    if (changes.empty()) return Action::ERROR;
    
    undoInfo top = changes.top();
    
    row = top.u_row;
    col = top.u_col;
    // O(1) constant time complexity
    
    
    if (top.move == INSERT){
        changes.pop();
        count = (int)top.u_ch.size(); //# of characters involved
        text = "";
        return DELETE;
    }
    else if (top.move == DELETE){
        changes.pop();
        count = 1;
        text = top.u_ch; //string to insert back
        //O(length of deleted characters that need to be restored)
        return INSERT;
    }
    else if (top.move == SPLIT){
        changes.pop();
        count = 1;
        text = "";
        return JOIN;
    }
    else if (top.move == JOIN){
        changes.pop();
        count = 1;
        text = "";
        return SPLIT;
    }
    
    return Action::ERROR;

 
}

void StudentUndo::clear() {
    while (! changes.empty()){
        changes.pop(); //clear all submissions of undo
    } // loop will be O(N)
}
