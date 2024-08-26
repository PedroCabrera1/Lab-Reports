#include <iostream>
#include <fstream>
#include <string>
#include <vector>//Ch7
#include <cstdlib>//Ch 8
#include <ctime>//Ch 5 
#include <iomanip>//CH 3
#include <set>

using namespace std;

//Class to keep track of the scores from each Player including AI.
class ScoreBoard {
public:
    int player1Wins;
    int player2Wins;
    int ties;
    int aiWins;
    string fileName;

    
    ScoreBoard() {
        player1Wins = 0;
        player2Wins = 0;
        ties = 0;
        aiWins = 0;
        fileName = "default_scores.txt";//in case player chooses list but does not enter one from the list
    }

    //This will add 1 to the winning players score
    void recordWin(int player) {
        if (player == 1) player1Wins++;
        else if (player == 2) player2Wins++;
        else if (player == 0) aiWins++;  
    }
    //In case of a tie those are also recorded
    void recordTie() {//I tried to keep everything separate in methods for easier later access, hints this very short one
        ties++;
    }
    //In general computer terminology, functions defined in a class (discussed in Part II) are referred to as “methods.” pg 433
    // This method displays the score of each player.
    void displayScore() {
        cout << "Scoreboard:" << endl;
        cout << "Player 1 Wins: " << player1Wins << endl;
        cout << "Player 2 Wins: " << player2Wins << endl;
        cout << "AI Wins: " << aiWins << endl;
        cout << "Ties: " << ties << endl;
    }
    //This saves the score of the game to the file name of choice by the player
    //this allows to quit and use tha same file later to continue with the same score.
    void saveScores() {
        ofstream outFile("C:\\test\\files\\" + fileName);
        outFile << player1Wins << '\n';
        outFile << player2Wins << '\n';
        outFile << aiWins << '\n';
        outFile << ties << '\n';
        outFile.close();
    }
    // Method for loading saved data from old files and the current file.
    void loadScores(const string& file) {
        ifstream inFile("C:\\test\\files\\" + file); 
        if (inFile.is_open()) {
            inFile >> player1Wins;
            inFile >> player2Wins;
            inFile >> aiWins;
            inFile >> ties;
            inFile.close();
            fileName = file; //Update the current file name so new data cn be saved
        }
        else {
            cout << "Unable to open file for reading scores." << endl;
        }
    }
};
//This class will set up the board and hold the methods for all the moves
class TicTacToe {
public:
    TicTacToe() {
        //initializing a 3x3 empty board
        //Using Vectors over the conventional array proved more useful
        board = vector<vector<char>>(3, vector<char>(3, ' '));
        //Calling reset board to makle sure board is ready for the game
        resetBoard();
        srand(time(0));  // Initialize random seed
    }
    //Method to start a game in either single or 2 player modes
    void playGame(int mode);
    //For recording the results
    void recordScore(const string& result);
    ScoreBoard scores;

private:
    //Private member to hold the current state of the game board
    vector<vector<char>> board;
    //Methods in TicTacToe private class
    void twoPlayerGame();
    void singlePlayerGame();
    bool checkWin();
    void resetBoard();
    bool makeMove(int player, int x, int y);
    void printBoard();
    bool checkForWin(char playerSymbol, int& x, int& y);
    bool checkForBlock(int& x, int& y, char playerSymbol);
    void makeRandomMove(char symbol);
    void aiMakeMove();
};
//this was made to better enhance the performance of the AI
// With Thios method Dr. Strange is able to test all scenarios of victory before execution
bool TicTacToe::checkForWin(char playerSymbol, int& x, int& y) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = playerSymbol; // Temporarily make the move
                if (checkWin()) {
                    board[i][j] = ' '; // Undo the move
                    x = i;
                    y = j;
                    return true;
                }
                board[i][j] = ' '; // Undo the move
            }
        }
    }
    return false;
    }
//This method takes the player selection and connects it to their menu choice
//It then calls the desired method 
void TicTacToe::playGame(int mode) {
    switch (mode) {
    case 1:
        singlePlayerGame();
        break;
    case 2:
        twoPlayerGame();
        break;
    default:
        cout << "Invalid mode selected. Please try again." << endl;
        break;
    }
}

//These are the contents of the 2 player method 
void TicTacToe::twoPlayerGame() {
    int currentPlayer = 1;// This means player one will begin
    bool gameRunning = true;
    int moves = 0;

    //This loop will run as long as game is still running and less than 9 moves have been made
    while (gameRunning && moves < 9) {
        printBoard();
        int x, y;
        cout << "Your turn. Enter row (0-2): ";
        cin >> x;
        cout << "\nYour turn. Enter Column (0-2): ";
        cin >> y;
        
        //This will call makeMove to make sure selection is valid
        if (!makeMove(currentPlayer, x, y)) {
            cout << "Invalid move. Try again." << endl;
            continue;
        }
        //Checking for a win
        if (checkWin()) {
            printBoard();
            cout << "Player " << currentPlayer << " wins!" << endl;
            recordScore("Player " + to_string(currentPlayer) + " wins");
            resetBoard();
            gameRunning = false;//once set top false while loop will end
        }
        else {
            currentPlayer = 3 - currentPlayer;
            moves++;
        }

        if (moves == 9 && gameRunning) {
            printBoard();
            cout << "It's a tie!" << endl;
            recordScore("Tie");
            resetBoard();
            gameRunning = false;
        }
    }
}
//This is the Mothod that runs the single player mode
void TicTacToe::singlePlayerGame() {
    int currentPlayer = 1;
    bool gameRunning = true;
    int moves = 0;

    while (gameRunning && moves < 9) {
        printBoard();
        int x, y;
        if (currentPlayer == 1) {
            cout << "Your turn. Enter row (0-2): ";
            cin >> x;
            cout << "\nYour turn. Enter Column (0-2): ";
            cin >> y;
            if (!makeMove(currentPlayer, x, y)) {
                cout << "Invalid move. Try again." << endl;
                continue;
            }
        }
        else {
            cout << "Dr. Strange is making his move...\n" << endl;
            aiMakeMove();
        }

        if (checkWin()) {
            printBoard();
            if (currentPlayer == 1) {
                cout << "You win!" << endl;
                recordScore("Player 1 wins");
            }
            else {
                cout << "Dr. Strange wins!" << endl;
                recordScore("AI wins");  
            }
            resetBoard();
            gameRunning = false;
        }
        else {
            currentPlayer = 3 - currentPlayer;
            moves++;
        }

        if (moves == 9 && gameRunning) {
            printBoard();
            cout << "It's a tie!" << endl;
            recordScore("Tie");
            resetBoard();
            gameRunning = false;
        }
    }
}
//Simple random move selection when no block or win is available
void TicTacToe::makeRandomMove(char symbol) {
    int x, y;
    do {
        x = rand() % 3;
        y = rand() % 3;
    } while (board[x][y] != ' ');
    board[x][y] = symbol;
}
//For AI to check if he can block opponent
bool TicTacToe::checkForBlock(int& x, int& y, char playerSymbol) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = playerSymbol;  // Temporarily make the move
                if (checkWin()) {
                    board[i][j] = 'O';  // Make the block move
                    return true;
                }
                board[i][j] = ' ';  // Undo the move
            }
        }
    }
    return false;
}
//This calls and checks the three methods used for AI selection making
void TicTacToe::aiMakeMove() {
    int x = -1, y = -1;
    if (checkForWin('O', x, y)) {//Ofc first it will check for victory
        if (x >= 0 && x < 3 && y >= 0 && y < 3) {
            board[x][y] = 'O';
            cout << "AI winning move at: " << x << ", " << y << endl;
        }
        else {
            cout << "... Still Thinking" << endl;// This was originally an out of bounds check
        }
    }
    else if (checkForBlock(x, y, 'X')) {
        if (x >= 0 && x < 3 && y >= 0 && y < 3) {
            board[x][y] = 'O';
            cout << "AI blocking move at: " << x << ", " << y << endl;
        }
        else {
            cout << "... Still Thinking" << endl;// This was originally an out of bounds check
        }
    }
    else {
        makeRandomMove('O');
        cout << "AI made a random move." << endl;
    }
}
//Simple method to reset board
void TicTacToe::resetBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

bool TicTacToe::makeMove(int player, int x, int y) {
    if (x >= 0 && x < 3 && y >= 0 and y < 3 && board[x][y] == ' ') {
        board[x][y] = (player == 1) ? 'X' : 'O';//Saw that the Conditional operator was barely tapped in the appendix A3 and it peaked my interest and
                                                // it proved quite useful, especially to save a few lines for a switch or if statement.
        return true;
    }
    return false;
}
//Method to check for victory
bool TicTacToe::checkWin() {
    for (int i = 0; i < 3; i++) {
        // Check horizontal and vertical lines
        if ((board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) ||
            (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])) {
            return true;
        }
    }
    // Check diagonal lines
    if ((board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
        (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])) {
        return true;
    }
    return false;
}

void TicTacToe::printBoard() {
    const int width = 7;  // Width for each cell to maintain spacing
    int centeringPad = 20;  // Adjust this to center the board in your console
    cout << setw(centeringPad) << "" << "Board:\n\n";  // Print the board title centered

    // Printing column labels
    cout << setw(centeringPad) << ""; // Pad for centering
    for (int col = 0; col < 3; col++) {
        cout << setw(width) << "Col (" << col << ")";
    }
    cout << endl;

    for (int i = 0; i < 3; i++) {
        // Printing row label
        cout << setw(centeringPad - 5) << "Row (" << i << ") ";  // Adjust -5 as needed to align the row labels

        // Printing the board cells
        for (int j = 0; j < 3; j++) {//Once again here the conditional operator proved useful
            cout << setw(width) << (board[i][j] == ' ' ? "_" : string(1, board[i][j]));  // Display '_' for empty spaces
            if (j < 2) cout << "|";  //only add vertical bars between columns
        }
        cout << endl;

        //adding horizontal dividers between rows, but not after the last row
        if (i < 2) {
            cout << setw(centeringPad) << "";  // Pad the line divider as well
            cout << setw(width * 3 + 2) << setfill('-') << "-" << endl;  //creates a horizontal line divider
            cout << setfill(' ');  // Reset fill character
        }
    }
    cout << endl;
}

void TicTacToe::recordScore(const std::string& result) {
    cout << "Result Recorded: " << result << endl; //Debugging line to show what result is being processed
                                                   //I kept having issues where AI wins Counted as ties

    if (result == "Player 1 wins") {
        scores.player1Wins++;
        cout << "Player 1 wins now totals: " << scores.player1Wins << endl; //These were also incorporated for debugging 
    }
    else if (result == "Player 2 wins") {
        scores.player2Wins++;
        cout << "Player 2 wins now totals: " << scores.player2Wins << endl; //But they made a nice addition to the code
    }
    else if (result == "AI wins") {
        scores.aiWins++;
        cout << "AI wins now totals: " << scores.aiWins << endl; 
    }
    else {
        scores.recordTie();
        cout << "Ties now totals: " << scores.ties << endl; 
    }

    scores.saveScores();
}

//This displays the list of saved files
void listSavedFiles() {
    cout << setw(40) << right << "Available Score Files:\n";
    ifstream inFile("C:\\test\\files\\savedFiles.txt");
    string file;
    while (getline(inFile, file)) {
        cout << setw(30) << right << file << endl;
    }
    inFile.close();
}
//Updating the saved file list
void updateSavedFilesList(const string& fileName) {
    ofstream outFile("C:\\test\\files\\savedFiles.txt", ios::app);//ios::app pg. 471 :Open a text file in append mode
    outFile << fileName << endl;
    outFile.close();
}

int main() {
    TicTacToe game;
    int choice;
    bool running = true;
    string fileName;

    cout << setw(60) << right << "Welcome To Dr.Strange's Tic Tac Toe\n\n"<< endl;

    cout << setw(50) << right << "Enter a filename for new score data or type 'list' to choose from existing files: ";
    cin >> fileName;
    if (fileName == "list") {
        listSavedFiles();
        cout << setw(50) << right << "Enter the filename from the list above: ";
        cin >> fileName;
        game.scores.loadScores(fileName);
    }
    else {
        game.scores.fileName = fileName;
        updateSavedFilesList(fileName);  // Update the list of saved score files
    }
    //Main menu
    while (running) {
        cout << setw(51) << right << "Welcome to Tic Tac Toe!\n\n";
        cout << setw(29) << right << "1: Single Player\n";
        cout << setw(26) << right << "2: Two Player\n";
        cout << setw(31) << right << "3: View Scoreboard\n";
        cout << setw(21) << right << "4: Quit\n\n";
        cout << setw(50) << right << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
        case 2:
            game.playGame(choice);
            break;
        case 3:
            game.scores.displayScore();
            break;
        case 4:
            running = false;
            cout << setw(30) << right << "Thanks for playing!\n";
            game.scores.saveScores();  // Save scores before exiting
            break;
        default:
            cout << setw(50) << right << "Invalid input. Please enter a number between 1 and 4.\n";
            break;
        }
    }
    system("pause");
    return 0;
}