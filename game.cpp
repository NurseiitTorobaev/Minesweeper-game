#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime>   

using namespace std;

// printing board with rows and cols
void printBoard(const vector<vector<string>>& board) 
{
    int rows = board.size();
    int cols = board[0].size();
    cout << "   ";
    for (int j = 0; j < cols; ++j) 
    {
        cout << j + 1 << "  ";
    }
    cout << endl;
    for (int i = 0; i < rows; ++i) 
    {
        
        cout << i + 1 << " ";
        for (int j = 0; j < cols; ++j) 
        {
            cout << " " << board[i][j] << " ";
        }
        cout << endl;
    }
}

// creating board
vector<vector<string>> createHiddenBoard(int rows, int cols) 
{
    return vector<vector<string>>(rows, vector<string>(cols, "#"));
}

// placing mines randomly on another board
void placeMines(vector<vector<string>>& board, int numMines) 
{
    int rows = board.size();
    int cols = board[0].size();

    srand(time(NULL));

    for (int i = 0; i < numMines; ++i) 
    {
        int randRow = rand() % rows;
        int randCol = rand() % cols;
        board[randRow][randCol] = "*";
    }
}

//numbering the cells on the board depending on how much mines are around each cell
void numberingGameBoard(vector<vector<string>>& gameBoard) 
{
    int rows = gameBoard.size();
    int cols = gameBoard[0].size();

    for (int i = 0; i < rows; ++i) 
    {
        for (int j = 0; j < cols; ++j) 
        {
            // If the cell is not a mine, count nearby mines
            if (gameBoard[i][j] != "*") 
            {
                int count = 0;
                // Check neighboring cells for mines
                for (int k = -1; k <= 1; ++k) 
                {
                    for (int l = -1; l <= 1; ++l) 
                    {
                        int newRow = i + k;
                        int newCol = j + l;
                        // checking bounds
                        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) 
                        {
                            if (gameBoard[newRow][newCol] == "*") 
                            {
                                count++;
                            }
                        }
                    }
                }
                gameBoard[i][j] = to_string(count);
            }
        }
    }
}

// function for uncovering all nearby cells with "0"
void uncoverCells(vector<vector<string>>& hiddenBoard, const vector<vector<string>>& gameBoard, int row, int col) 
{
    int rows = hiddenBoard.size();
    int cols = hiddenBoard[0].size();

    // Base case: if the cell is not hidden or if it has a number, return
    if (row < 0 || row >= rows || col < 0 || col >= cols || hiddenBoard[row][col] != "#")
        return;

    // Uncovering the cell
    hiddenBoard[row][col] = gameBoard[row][col];

    // If the cell is "0", uncover its neighbors recursively
    if (gameBoard[row][col] == "0") 
    {
        for (int i = -1; i <= 1; ++i) 
        {
            for (int j = -1; j <= 1; ++j) 
            {
                uncoverCells(hiddenBoard, gameBoard, row + i, col + j);
            }
        }
    }
}

int main() 
{
    int rows, cols, numMines;
    //Beginning of the game, choosing the difficulty
    cout <<"Hi! Welcome to the Minesweeper game!\n";
    cout <<"Choose a difficulty level:\n";
    cout <<"1. Beginner (9x9 board with 10 mines)\n";
    cout <<"2. Intermediate (16x16 board with 40 mines)\n";
    cout <<"3. Advanced (16x30 board with 99 mines)\n";
    cout <<"Enter your choice: ";
    int difficulty;
    cin>>difficulty;
    cout << endl;
    switch (difficulty)
    {
    case 1:
        rows=9;
        cols=9;
        numMines=10;
        break;
    case 2:
        rows=16;
        cols=16;
        numMines=40;    
        break;
    case 3:
        rows=16;
        cols=30;
        numMines=99;
        break;
    default:
        break;
    }
    
    vector<vector<string>> hiddenBoard = createHiddenBoard(rows, cols);
    
    int countMines=numMines;
    
    vector<vector<string>> gameBoard(rows, vector<string>(cols, "0"));
    
    placeMines(gameBoard, numMines);

    numberingGameBoard(gameBoard);

    cout << "Hidden board:" << endl;
    printBoard(hiddenBoard);

    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "Game starts!\n";

    //the game process
    int gameEnd = 0;
    int x, y;
    string choice;
    while (gameEnd != 1) 
    {
        cout<<"Write 'u' to undercover cell, 'f' to flag it: ";
        cin>>choice;
        if(choice=="u")
        {
            cin >> x >> y;
            x--;
            y--;
            if (gameBoard[x][y] == "*") 
            {
                gameEnd = 1;
                cout << "You've been blown up on mine\n";
            } 
            else if (gameBoard[x][y] == "0") 
            {
                uncoverCells(hiddenBoard, gameBoard, x, y);
                cout << "Hidden Board:" << endl;
                printBoard(hiddenBoard);
            }
            else
            {
                hiddenBoard[x][y]=gameBoard[x][y];
                printBoard(hiddenBoard);
            }
        }
        else if(choice=="f")
        {
            cin>>x>>y;
            x--;
            y--;
            if(gameBoard[x][y]=="*")
            {
                countMines--;
            }
            hiddenBoard[x][y]="F";
            printBoard(hiddenBoard);
        }
        else
        {
            cout<<"Invalid option! Try again\n";
            printBoard(hiddenBoard);
        }
        //if the player flags all mines, he wins
        if(countMines==0)
        {
            cout<<"You've won! Congratulations!\n";
            gameEnd=1;
        }
        int remainingCells = 0;
        for (int i = 0; i < rows; ++i) 
        {
            for (int j = 0; j < cols; ++j) 
            {
                if (hiddenBoard[i][j] == "#" || hiddenBoard[i][j] == "F") 
                {
                    remainingCells++;
                }
            }
        }
        //if the player uncovers all safe cells, he wins
        if (remainingCells == numMines) 
        {
            cout << "You've won! Congratulations!\n";
            gameEnd = 1;
        }
        cout << endl;
    }
    //If player wants, the game can be restarted
    int restart;
    cout<<"Do you want to restart? Type 1 for 'Yes'.\n";
    cin>>restart;
    if(restart==1)
    {
        main();
    }
    return 0;
}