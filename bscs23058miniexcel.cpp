#include <iostream>
#include <iomanip>
using namespace std;

class MiniExcel {
private:
    class Cell {
    public:
        char data;   // Content of the cell
        Cell* up;    // Link to the cell above
        Cell* down;  // Link to the cell below
        Cell* left;  // Link to the cell to the left
        Cell* right; // Link to the cell to the right

        Cell(char value = ' ') : data(value), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
    };
    Cell* current;
    Cell* root;      // Root cell (top-left cell)
    int rows;        // Number of rows in the grid
    int cols;        // Number of columns in the grid

public:
    MiniExcel() {
        rows = 5; 
        cols = 5;

      
        root = new Cell();
        Cell* rowPtr = root;

        for (int i = 0; i < rows; i++) {
            Cell* colPtr = rowPtr;
            for (int j = 0; j < cols; j++) {
                if (i < rows - 1) {
                    colPtr->down = new Cell();
                    colPtr->down->up = colPtr;  
                }
                if (j < cols - 1) {
                    colPtr->right = new Cell();  
                    colPtr->right->left = colPtr; 
                    colPtr = colPtr->right;      
                }
            }
            if (i < rows - 1) {
                rowPtr = rowPtr->down;  
            }
        }

       
        root->data = 'A';
        root->right->data = 'B';
        root->down->data = 'C';
        root->down->right->data = 'D';
    }

    
    Cell* insertRowAbove(Cell* current) {
        Cell* newRowHead = new Cell(); 
        Cell* newRowCurrent = newRowHead;

       
        for (int i = 0; i < cols; i++) {
            if (i > 0) {
                newRowCurrent->right = new Cell(); 
                newRowCurrent->right->left = newRowCurrent;
                newRowCurrent = newRowCurrent->right; 
            }
          
            newRowCurrent->data = ' '; 
        }

        
        newRowHead->down = current; 

        if (current->up) { 
            newRowHead->up = current->up;
            current->up->down = newRowHead; 
        }
        else {
            newRowHead->up = nullptr; 
        }

        current->up = newRowHead; 

       
        rows++;
        return newRowHead; 
    }

  

    Cell* insertRowBelow(Cell* current) {
        Cell* newRowHead = new Cell(); 
        Cell* newRowCurrent = newRowHead;

        for (int i = 0; i < cols; i++) {
            if (i > 0) {
                newRowCurrent->right = new Cell(); 
                newRowCurrent->right->left = newRowCurrent;
                newRowCurrent = newRowCurrent->right; 
            }
        }

      
        newRowHead->up = current; 
        newRowHead->down = current->down; 

        if (current->down) { 
            current->down->up = newRowHead;
        }

        current->down = newRowHead; 

      
        rows++;
        return newRowHead; 
    }

    
    void insertColumnToRight(Cell* current) {
        
        Cell* rowPtr = root; 

        while (rowPtr != nullptr) {
            Cell* newCell = new Cell(); 

           
            newCell->left = current; 
            newCell->data = ' '; 

            if (current->right) {
                newCell->right = current->right; 
                current->right->left = newCell; 
            }
            current->right = newCell; 
 
            rowPtr = rowPtr->down;
            current = current->down; 
        }
    }

    void insertColumnToLeft1(Cell* current) {
        // Create the new column head
        Cell* newColHead = new Cell();
        newColHead->data = ' '; // Initialize the new column cell with a space

        // Link the new column head with the current column
        newColHead->right = current; // The new column's right points to the current column
        newColHead->left = current->left; // The new column's left points to the left of the current column

        // Update the current column's left pointer to the new column
        current->left = newColHead;

        // If there was an existing column to the left, update its right pointer
        if (newColHead->left) {
            newColHead->left->right = newColHead; // Link the old left column to the new column
        }

        // Now we need to iterate through each row and add the new column cell
        Cell* rowPtr = root; // Start from the root

        while (rowPtr != nullptr) {
            // Create a new cell for the new column in the current row
            Cell* newCell = new Cell();
            newCell->data = ' '; // Initialize the new cell with a space

            // Link the new cell with the left cell in the current row
            newCell->right = rowPtr->left; // Link the new cell's right to the old left cell
            if (newCell->right) {
                newCell->right->left = newCell; // If there's an existing left cell, update its left pointer
            }

            // Update the left pointer of the current row to the new cell
            rowPtr->left = newCell; // Set the current row's left pointer to the new cell

            // Move to the next row downwards
            rowPtr = rowPtr->down;
        }

        cols++; // Increment the column count
    }
    void insertColumnToLeft(Cell* current) {
        
        Cell* newColHead = new Cell();
        newColHead->data = ' ';

      
        newColHead->right = current; 
        newColHead->left = current->left;
        current->left = newColHead;

  
        if (newColHead->left) {
            newColHead->left->right = newColHead;
        }

      
        Cell* rowPtr = root; 

        while (rowPtr != nullptr) {
            
            Cell* newCell = new Cell();
            newCell->data = ' ';

           
            newCell->right = rowPtr->left; 
            if (newCell->right) {
                newCell->right->left = newCell; 
            }

          
            rowPtr->left = newCell; 

            rowPtr = rowPtr->down;
        }

        cols++;
    }



    void insertColumnToCurrentPosition(bool toRight) {
        auto currentCell = getCurrentCell(); 
        if (toRight) {
            insertColumnToRight(currentCell);
        }
        else {
            insertColumnToLeft(currentCell);
        }
    }

   
    Cell* getCurrentCell() {
        return root; 
    }
 


    class Iterator {
    private:
        Cell* currentCell;

    public:
        // Constructor initializing the iterator to a given cell
        Iterator(Cell* startCell) : currentCell(startCell) {}

        // Move the iterator up
        Iterator& operator--() {  // Pre-decrement (move up)
            if (currentCell->up) {
                currentCell = currentCell->up;
            }
            return *this;
        }

        // Move the iterator down
        Iterator& operator++() {  // Pre-increment (move down)
            if (currentCell->down) currentCell = currentCell->down;
            return *this;
        }

        // Move the iterator left
        Iterator operator--(int) {  // Post-decrement (move left)
            Iterator temp = *this;
            if (currentCell->left) currentCell = currentCell->left;
            return temp;
        }

        // Move the iterator right
        Iterator operator++(int) {  // Post-increment (move right)
            Iterator temp = *this;
            if (currentCell->right) currentCell = currentCell->right;
            return temp;
        }
    };

    Iterator getIterator() {
        return Iterator(current);
    }


    void printSheet() {
        Cell* rowPtr = root;
        while (rowPtr != nullptr) {
            Cell* colPtr = rowPtr;
            cout << "    |";
            while (colPtr != nullptr) {
                cout << setw(3) << colPtr->data << " | "; 
                colPtr = colPtr->right; 
            }
            cout << endl;

          
            cout << "    ------------------------------" << endl;
            rowPtr = rowPtr->down; 
        }
    }
  

    void deleteRow(Cell* rowToDelete) {
        if (!rowToDelete) return;
       
        if (rowToDelete->up) {
            rowToDelete->up->down = rowToDelete->down;
        }
        if (rowToDelete->down) {
            rowToDelete->down->up = rowToDelete->up;
        }

        
        Cell* colPtr = rowToDelete;
        while (colPtr) {
            Cell* temp = colPtr;
            colPtr = colPtr->right; 
            delete temp; 
        }

       
        rows--;
    }

 

    void deleteColumn(int columnIndex) {
        if (columnIndex <= 0 || columnIndex >= cols) return; 

       
        Cell* rowPtr = root; 

        while (rowPtr != nullptr) {
            Cell* colPtr = rowPtr; 
            for (int j = 0; j < columnIndex; j++) {
                if (colPtr == nullptr) return; 
                colPtr = colPtr->right;
            }

           
            if (colPtr != nullptr) {
               
                if (colPtr->left) {
                    colPtr->left->right = colPtr->right; 
                }
                if (colPtr->right) {
                    colPtr->right->left = colPtr->left; 
                }

               
                delete colPtr;
            }

           
            rowPtr = rowPtr->down;
        }

       
        cols--;
    }
   

    void ClearColumn(Cell* colPtr) {
      
        while (colPtr->up) {
            colPtr = colPtr->up;
        }
        while (colPtr) {
            colPtr->data = ' '; 
            colPtr = colPtr->down;
        }
    }

   
    void ClearRow(Cell* rowPtr) {
       
        while (rowPtr->left) {
            rowPtr = rowPtr->left;
        }
        while (rowPtr) {
            rowPtr->data = ' '; 
            rowPtr = rowPtr->right;
        }
    }

   
    ~MiniExcel() {
        while (root) {
            Cell* rowPtr = root;
            while (rowPtr) {
                Cell* temp = rowPtr;
                rowPtr = rowPtr->right; 
                delete temp; 
            }
            root = root->down; 
        }
    }


};

int main() {
    MiniExcel sheet;

    cout << "Initial Sheet:" << endl;
    sheet.printSheet();

    
    auto currentCell = sheet.getCurrentCell(); 

    /*    currentCell = sheet.insertRowBelow(currentCell);
     cout << "After inserting row below:" << endl;
     sheet.printSheet();*/

    
 /*     currentCell = sheet.insertRowAbove(currentCell->down->down); 
      cout << "after inserting row above:" << endl;
      sheet.printSheet();*/


    /*  sheet.insertColumnToCurrentPosition(true); 
      cout << "After inserting column to the right of A:" << endl;
      sheet.printSheet();*/

   
     /* sheet.insertColumnToCurrentPosition(false); 
      cout << "After inserting column to the left of A:" << endl;
      sheet.printSheet();*/

     // currentCell = sheet.getCurrentCell();
     // sheet.deleteRow(currentCell->down); 
     // cout << "After deleting the row:" << endl;
     // sheet.printSheet();

     // 
     // sheet.deleteColumn(2); 
     // cout << "After deleting the column:" << endl;
     // sheet.printSheet();

    
     //sheet.ClearColumn(currentCell); 
     // cout << "After clearing current column:" << endl;
     // sheet.printSheet();

     // sheet.ClearRow(currentCell);
     // cout << "After clearing current row:" << endl;
     // sheet.printSheet();


    return 0;
}

