#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

enum class Tile : char {
    empty ='e', 
    white ='w', 
    black = 'b', 
    illegal ='i'
};

// A Board class that represents the current state of the game board.
class Board
{
private:
	// a board is a 2-dimensional vector of Tiles
    vector<vector<Tile>> board;

    struct Location 
    {
        int row;
        int col;
    };

public:
	Board(int width)
	: board(width)
	{
		assert(width > 1);


		// initialize all tiles to empty
        for(int i = 0; i < board.size(); ++i) 
        {
            board[i] = vector<Tile>(width, Tile::empty);
        }

        // centre of the tiles to be:
        // black	white
        // white	black

        // int bLeft = width / 2;
        // int bRight = (width / 2) - 1;

        // board[bLeft] = vector<Tile> (bLeft, Tile::black);
        // board[bRight] = vector<Tile> (bRight, Tile::black);

        

	}

	~Board()
	{}

	// get the width and height of the board
    int width() const { return board[0].size(); }
    int height() const { return board.size(); }

	// print the board to the screen
    void print() const 
    {
        for(int i = 0; i < height(); ++i) 
        {
            for(int j = 0; j < width(); ++j) 
            {
                cout << char(board[i][j]) << "";  // char(board[i][j]) converts
            }                                     // board[i][j] (which is of type Tile)
            cout << "\n";                         // to its underlying char
        }
    }

    void println() const {
        print();
        cout << "\n";
    }	
	
};

// An abstract base class called Player that represents a generic player, either a human or a computer. 
// Player must be abstract: the only use of Player is to be a base class for the two classes mentioned below.

class Player
{
public:
	Player();
	~Player();
	
};


// A class called Human_player that represents a human player. This should derive from the Player class.
class Human_player : public Player
{
public:
	Human_player();
	~Human_player();
	
};

// A class called Computer_player that represents a computer player. This should derive from the Player class.
class Computer_player : public Player
{
public:
	Computer_player();
	~Computer_player();
	
};

void test_board() {
    Board board(4);
    board.println();
}


int main()
{
	test_board();
}