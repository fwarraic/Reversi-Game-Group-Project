#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <map>

using namespace std;

//color of piece. None means that board cell is empty
enum class Color : char {
    white = 'o', black = 'x', none = '.'
};

//cell of board
struct Cell {
    //row number
    int row;
    //column number
    int col;

    //default constructor
    Cell();
    //constructor
    Cell(int x, int y);
    //check that cell is in square [mn..mx] x [mn..mx]
    bool inSquare(int mn, int mx);
};

//game board
class Board {
    //colors of each cell of board
    vector<vector<Color>> board;
    //counts number of pieces for given color
    map<Color, int> scores;

    //return vector of flips that would be done after placing piece of color color on cell cell
    vector<Cell> flips(Cell cell, Color color);

public:
    //board size
    static const int size = 8;

    //constructor. allocate memory for board
    Board();
    //make Othello board configuration
    void initOthello();
    //print board to stdout
    void print();
    //getter for scores
    map<Color, int> getScores();
    //check that it is possible to place piece of color color to cell cell
    bool validMove(Cell cell, Color color);
    //place piece of color color to cell cell
    void makeMove(Cell cell, Color color);
    //return count of flips that would be done after placing piece of color color on cell cell
    int countFlips(Cell cell, Color color);
    //check that player with color color has valid moves
    bool canMove(Color color);
};

// base class for game player
class Player {
protected:
    //color of player\s pieces. Black or white
    Color color;

public:
    //virtual destructor
    virtual ~Player();
    //make move on given board
    virtual void makeMove(Board& board) = 0;
    //setter for color
    void setColor(Color _color);
    //getter for color
    Color getColor();
};

// human player
class Human_player: public Player {
    // check that string suits to turn format
    bool validTurnString(string turn, Cell& cell);
    //check that turn can be made on given board
    bool validTurn(Board& board, Cell cell);

public:
    //make move
    void makeMove(Board& board);
};

//computer player
class Computer_player: public Player {
    //estimator for given move.  Estimation depends on flips count and corner occupation
    int getPriority(Cell cell, Board& board);

public:
    //make move
    void makeMove(Board& board);
};

class Game {
    Board board;
    Human_player human;
    Computer_player computer;
    bool finished;

    //initialize players
    void createPlayers();
    //print current state of the game
    void printState();
    //print game over message
    void over();

public:
    //play the game
    void play();
};

//default constructor
Cell::Cell() {}

//constructor
Cell::Cell(int x, int y) : row(x), col(y) {}

//check that cell is in square [mn..mx] x [mn..mx]
bool Cell::inSquare(int mn, int mx) {
    return row >= mn && row <= mx && col >= mn && col <= mx;
}

//return vector of flips that would be done after placing piece of color color on cell cell
vector<Cell> Board::flips(Cell cell, Color color) {
    //result vector with flips
    vector<Cell> result;

    //sort through all possible directions in which pieces can flip
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            //(0, 0) is not a direction
            if (x == 0 && y == 0) {
                continue;
            }

            //cell current moves along the direction while pieces have different color
            Cell current(cell.row + x, cell.col + y);
            //vector to remember pieces which probably would be flipped
            vector<Cell> tempFlips;
            //while cell current is on the board and there is a piece of required color on current cell
            while (current.inSquare(0, size - 1) && board[current.row][current.col] != Color::none && board[current.row][current.col] != color) {
                //add current to possible flips
                tempFlips.push_back(current);
                //continue move current  along the direction
                current.row += x;
                current.col += y;
            }
            //if we have met a piece of same color, than add possible flips to actual flips
            if (current.inSquare(0, size - 1) && board[current.row][current.col] == color) {
                result.insert(result.end(), tempFlips.begin(), tempFlips.end());
            }
        }
    }

    return result;
}

//constructor. allocate memory for board
Board::Board() : board(size) {
    for(int i = 0; i < size; ++i) {
        board[i] = vector<Color>(size);
    }
}

//make Othello board configuration
void Board::initOthello() {
    //make board empty
    for(int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            board[i][j] = Color::none;
        }
    }

    //place initial pieces
    board[3][3] = board[4][4] = Color::white;
    board[3][4] = board[4][3] = Color::black;

    //initialize scores
    scores[Color::white] = 2;
    scores[Color::black] = 2;
}

//print board to stdout
void Board::print() {
    //print upper border
    cout << " ";
    for (int i = 0; i < 2 * size + 1; ++i) {
        cout << "_";
    }
    cout << endl;
    //print board itself
    for (int i = 0; i < size; ++i) {
        cout << i + 1 << "|";
        for (int j = 0; j < size; ++j) {
            if (j != 0) {
                cout << " ";
            }
            cout << char(board[i][j]);
        }
        cout << "|" << endl;
    }
    //print lower border
    cout << " |";
    for (int i = 0; i < 2 * size - 1; ++i) {
        cout << "_";
    }
    cout << "|" << endl;
    cout << " ";
    for (int i = 0; i < size; ++i) {
        cout << " " << char('a' + i);
    }
    cout << endl;
}

//getter for scores
map<Color, int> Board::getScores() {
    return scores;
}

//check that it is possible to place piece of color color to cell cell
bool Board::validMove(Cell cell, Color color) {
    //if cell is already occupied return false
    if (board[cell.row][cell.col] != Color::none) {
        return false;
    }

    //if this placement lead to no flips return false
    if (countFlips(cell, color) == 0) {
        return false;
    }

    //otherwise such placement is possible
    return true;
}

//place piece of color color to cell cell
void Board::makeMove(Cell cell, Color color) {
    //find all flips
    auto cells = flips(cell, color);
    //update scores
    scores[color] += 1 + cells.size();
    scores[board[cells[0].row][cells[0].col]] -= cells.size();

    //place the piece
    board[cell.row][cell.col] = color;
    //flip
    for (int i = 0; i < cells.size(); i++) {
        board[cells[i].row][cells[i].col] = color;
    }
}

//return count of flips that would be done after placing piece of color color on cell cell
int Board::countFlips(Cell cell, Color color) {
    return flips(cell, color).size();
}

//check that player with color color has valid moves
bool Board::canMove(Color color) {
    //sort through all cells
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (validMove(Cell(i, j), color)) {
                return true;
            }
        }
    }

    return false;
}

//destructor
Player::~Player(){}

//setter for color
void Player::setColor(Color _color) {
    color = _color;
}

//getter for color
Color Player::getColor() {
    return color;
}

bool Human_player::validTurnString(string turn, Cell& cell) {
    //check that turn has 2 characters: digit 1-8 and letter a-h
    if (turn.length() != 2 || turn[0] < '1' || turn[0] > '8' || tolower(turn[1]) < 'a' || tolower(turn[1]) > 'h') {
        cout << "Wrong turn format." << endl;
        return false;
    }
    //create cell for the turn
    cell.row = turn[0] - '1';
    cell.col = turn[1] - 'a';
    return true;
}

//check that turn can be made on given board
bool Human_player::validTurn(Board& board, Cell cell) {
    if (!board.validMove(cell, color)) {
        cout << "You can't place a piece on this cell." << endl;
        return false;
    }
    return true;
}

//make move
void Human_player::makeMove(Board& board) {
    string turn;
    Cell cell;
    //ask user to enter the turn
    cout << "Your turn [1h, 5a, etc]: ";
    cin >> turn;
    //ask again until we have a valid turn
    while (!validTurnString(turn, cell) || !validTurn(board, cell)) {
        cout << "Your turn [1h, 5a, etc]: ";
        cin >> turn;
    }

    //make move
    board.makeMove(cell, color);
}


//estimator for given move.  Estimation depends on flips count and corner occupation
int Computer_player::getPriority(Cell cell, Board& board) {
    int result = board.countFlips(cell, color);
    //cell is in corner
    if ((cell.row == 0 || cell.row == board.size - 1) && (cell.col == 0 || cell.col == board.size - 1)) {
        result += 10;
    }

    return result;
}

//make move
void Computer_player::makeMove(Board& board) {
    //let user to see board after previous move
    cout << "Press enter to continue";
    cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    cin.get();

    //find all possible moves
    vector<Cell> possibilities;
    for (int i = 0; i < board.size; ++i) {
        for (int j = 0; j < board.size; ++j) {
            if (board.validMove(Cell(i, j), color)) {
                possibilities.push_back(Cell(i, j));
            }
        }
    }

    //find move with maximal priority
    int maxIndex = 0;
    auto maxPriority = getPriority(possibilities[0], board);

    for (int i = 1; i < possibilities.size(); ++i) {
        auto priority = getPriority(possibilities[i], board);
        if (priority > maxPriority) {
            maxIndex = i;
            maxPriority = priority;
        }
    }

    auto cell = possibilities[maxIndex];
    //make move
    board.makeMove(cell, color);
    cout << "Computer place a piece on " << char('1' + cell.row) << char('a' + cell.col) << endl;
}

//initialize players
void Game::createPlayers() {
    //ask user about color
    cout << "White or black? [w/b] ";
    char option;
    cin >> option;
    option = tolower(option);
    //ask again until we have a valid color
    while (option != 'w' && option != 'b') {
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid option. White or black? [w/b] ";
        cin >> option;
        option = tolower(option);
    }

    //set colors to players
    if (option == 'w') {
        human.setColor(Color::white);
        computer.setColor(Color::black);
    } else {
        human.setColor(Color::black);
        computer.setColor(Color::white);
    }
}

//print current state of the game
void Game::printState() {
    cout << endl;
    cout << "************** Game state **************" << endl;
    //print piece counts for each color
    auto scores = board.getScores();
    cout << "You: " << scores[human.getColor()] << " " << char(human.getColor()) << endl;
    cout << "Computer: " << scores[computer.getColor()] << " " << char(computer.getColor()) << endl;
    //print board
    board.print();
    cout << "****************************************" << endl;
    cout << endl;
}

//print game over message
void Game::over() {
    //print board
    printState();
    //print scores
    auto scores = board.getScores();
    cout << "Game over. Scores: " << scores[human.getColor()] << ":" << scores[computer.getColor()] << endl;
    if (scores[human.getColor()] == scores[computer.getColor()]) {
        cout << "It is a tie." << endl;
    } else
    if (scores[human.getColor()] > scores[computer.getColor()]) {
        cout << "You win!" << endl;
    } else {
        cout << "Computer win!" << endl;
    }
    cout << endl;
}

//play the game
void Game::play() {
    finished = false;
    //initialize board
    board.initOthello();
    //initialize players
    createPlayers();
    //color of next turn. Black starts
    auto turn = Color::black;

    //print initial state
    printState();

    while (!finished) {
        Color next;
        //make move and define color of next move
        if (turn == human.getColor()) {
            human.makeMove(board);
            next = computer.getColor();
        } else {
            computer.makeMove(board);
            next = human.getColor();
        }

        //print board after move
        printState();

        //check that next player has valid moves
        if (!board.canMove(next)) {
            //if both players have no valid moves, game is over
            if (!board.canMove(turn)) {
                finished = true;
            } else {
                //if current player still have valid moves, continue with current player
                cout << "No valid turns. Game passes to " << char(turn) << "." << endl;
                next = turn;
            }
        }

        turn = next;
    }

    //print game results
    over();
}

int main() {
    Game game;
    char option = 'y';
    while (option == 'y') {
        //play game
        game.play();
        //ask if user wants to play again
        cout << "Do you want play again [y/n]? ";
        cin >> option;
        option = tolower(option);
         //ask again until we have a valid answer
        while (option != 'y' && option != 'n') {
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid option. Yes or no [y/n]? ";
            cin >> option;
            option = tolower(option);
        }
   }
}
