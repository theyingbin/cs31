// zurts.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <cassert>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;            // max number of rows in the arena
const int MAXCOLS = 20;            // max number of columns in the arena
const int MAXZURTS = 100;          // max number of zurts allowed
const int MAXCOLORS = 3;           // max number of colors
const double WALL_DENSITY = 0.13;  // density of walls

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int NUMDIRS = 4;

const int EMPTY = 0;
const int WALL = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
// type name, since it's mentioned in the Zurt declaration.

class Zurt
{
public:
	// Constructor
	Zurt(Arena* ap, int r, int c, char color);

	// Accessors
	int  row() const;
	int  col() const;
	char color() const;
	bool isDead() const;

	// Mutators
	void forceMove(int dir);
	void move();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	char   m_color;
	int	   m_health;
};

class Player
{
public:
	// Constructor
	Player(Arena *ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	string stand();
	string move(int dir);
	void   setDead();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	bool   m_dead;
};

class Arena
{
public:
	// Constructor/destructor
	Arena(int nRows, int nCols);
	~Arena();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	int     zurtCount() const;
	int     getCellStatus(int r, int c) const;
	int     numberOfZurtsAt(int r, int c) const;
	void    display(string msg) const;

	// Mutators
	void   setCellStatus(int r, int c, int status);
	bool   addZurt(int r, int c, char color);
	bool   addPlayer(int r, int c);
	string moveZurts(char color, int dir);

private:
	int     m_grid[MAXROWS][MAXCOLS];
	int     m_rows;
	int     m_cols;
	Player* m_player;
	Zurt*   m_zurts[MAXZURTS];
	int     m_nZurts;

	// Helper functions
	void checkPos(int r, int c) const;
};

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nZurts);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	string takePlayerTurn();
	string takeZurtsTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool charToDir(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();
int findRisk(const Arena& a, int r, int c);


///////////////////////////////////////////////////////////////////////////
//  Zurt implementation
///////////////////////////////////////////////////////////////////////////

Zurt::Zurt(Arena* ap, int r, int c, char color)
{
	if (ap == nullptr)
	{
		cout << "***** A zurt must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "***** Zurt created with invalid coordinates (" << r << ","
			<< c << ")!" << endl;
		exit(1);
	}
	if (color != 'R'  &&  color != 'Y'  &&  color != 'B')
	{
		cout << "***** Zurt created with invalid color " << color << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_color = color;
	m_health = 3;
}

int Zurt::row() const
{
	return m_row;
}

int Zurt::col() const
{
	// TODO: TRIVIAL:  return what column the zurt is at
	return m_col; //  *PROVISIONALLY DONE*
}

char Zurt::color() const
{
	// TODO: TRIVIAL:  return what color the zurt follows
	return m_color;  //  * PROBATIONARILY DONE *
}

bool Zurt::isDead() const
{
	// TODO: TRIVIAL:  return whether the zurt is dead
	if (m_health == 0)
		return true;
	else
		return false;// *PROVISIONALLY DONE*
}

void Zurt::forceMove(int dir)
{
	// TODO:  move the zurt in the indicated direction, if possible.
	// Suffer one unit of damage if moving fails
	switch (dir)
	{
	case 0: //North
		if (m_row - 1 == 0) // checks for out of bounds
			m_health--;
		else if (m_arena->getCellStatus(m_row - 1, m_col) == WALL)
			m_health--;
		else
			m_row--;
		break;
	case 1: // East
		if (m_col + 1 == 0)
			m_health--;
		else if (m_arena->getCellStatus(m_row, m_col + 1) == WALL)
			m_health--;
		else
			m_col++;
		break;
	case 2: // South
		if (m_row + 1 == 0)
			m_health--;
		else if (m_arena->getCellStatus(m_row + 1, m_col) == WALL)
			m_health--;
		else
			m_row++;
		break;
	case 3: // West
		if (m_col - 1 == 0)
			m_health--;
		else if (m_arena->getCellStatus(m_row, m_col - 1) == WALL)
			m_health--;
		else
			m_col--;
		break;
	default:
		break;
	}

}

void Zurt::move()
{
	// Attempt to move in a random direction; if we can't move, don't move
	if (!isDead())
		attemptMove(*m_arena, randInt(0, NUMDIRS - 1), m_row, m_col);
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** The player must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_dead = false;
}

int Player::row() const
{
	// TODO: TRIVIAL:  return what row the player is at
	return m_row;  // *PROVISONALLY DONE*
}

int Player::col() const
{
	// TODO: TRIVIAL:  return what column the player is at
	// Delete the following line and replace it with the correct code.
	return m_col;  //  * PROVISIONALLY DONE*
}

string Player::stand()
{
	return "Player stands.";
}

string Player::move(int dir)
{
	// TODO:  Attempt to move the player one step in the indicated *PROVISIONALLY DONE*
	//        direction.  If this fails,
	//        return "Player couldn't move; player stands."
	//        A player who moves onto a zurt dies, and this
	//        returns "Player walked into a zurt and died."
	//        Otherwise, return one of "Player moved north.",
	//        "Player moved east.", "Player moved south.", or
	//        "Player moved west."

	switch (dir)
	{
	case 0:
		if (m_arena->numberOfZurtsAt(m_row - 1, m_col) > 0)
		{
			setDead();
			return "Player walked into a zurt and died.";
		}
		else if (m_arena->getCellStatus(m_row - 1, m_col) == WALL || m_row - 1 == 0)
			return "Player couldn't move; player stands.";
		else
		{
			m_row--;
			return "Player moved north.";
		}
		break;
	case 1:
		if (m_arena->numberOfZurtsAt(m_row - 1, m_col) > 0)
		{
			setDead();
			return "Player walked into a zurt and died.";
		}
		else if (m_arena->getCellStatus(m_row, m_col + 1) == WALL || m_col + 1 > m_arena->cols())
			return "Player couldn't move; player stands.";
		else
		{
			m_col++;
			return "Player moved east.";
		}
		break;
	case 2:
		if (m_arena->numberOfZurtsAt(m_row - 1, m_col) > 0)
		{
			setDead();
			return "Player walked into a zurt and died.";
		}
		else if (m_arena->getCellStatus(m_row + 1, m_col) == WALL || m_row + 1 > m_arena->rows())
			return "Player couldn't move; player stands.";
		else
		{
			m_row++;
			return "Player moved south.";
		}
		break;
	case 3:
		if (m_arena->numberOfZurtsAt(m_row - 1, m_col) > 0)
		{
			setDead();
			return "Player walked into a zurt and died.";
		}
		else if (m_arena->getCellStatus(m_row, m_col - 1) == WALL || m_col - 1 == 0)
			return "Player couldn't move; player stands.";
		else
		{
			m_col--;
			return "Player moved west.";
		}
		break;
	}
	return "I hope I never get this message :(";
}

bool Player::isDead() const
{
	// TODO: TRIVIAL:  return whether the player is dead
	// Delete the following line and replace it with the correct code.
	return this->m_dead; // *PROVISONALLY DONE*
}

void Player::setDead()
{
	m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Arena created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nZurts = 0;
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
	// TODO:  release the player and all remaining dynamically allocated robots *PROVISIONALLY DONE*
	for (int k = 0; k < zurtCount(); k++)
		delete m_zurts[k];
	delete m_player;
}

int Arena::rows() const
{
	// TODO: TRIVIAL:  return the number of rows in the arena
	return m_rows;  // *PROVISIONALLY DONE*
}

int Arena::cols() const
{
	// TODO: TRIVIAL:  return the number of columns in the arena
	return m_cols;  //  *PROVISIONALLY EDONE*
}

Player* Arena::player() const
{
	return m_player;
}

int Arena::zurtCount() const
{
	// TODO: TRIVIAL:  return the number of zurts in the arena
	return m_nZurts; // **PROVISIONALLY DONE**
}

int Arena::getCellStatus(int r, int c) const
{
	checkPos(r, c);
	return m_grid[r - 1][c - 1];
}

int Arena::numberOfZurtsAt(int r, int c) const
{
	// TODO:  return the number of zurts at row r, column c
	int count = 0;
	for (int j = 0; j < zurtCount(); j++)
		if (m_zurts[j]->row() == r && m_zurts[j]->col() == c)
			count++;
	return count; // PROVISIONALLY DONE
}

void Arena::display(string msg) const
{
	char displayGrid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill displayGrid with dots (empty) and stars (wall)
	for (r = 1; r <= rows(); r++)
		for (c = 1; c <= cols(); c++)
			displayGrid[r - 1][c - 1] = (getCellStatus(r, c) == EMPTY ? '.' : '*');

	// Indicate zurt positions by their colors.  If more than one zurt
	// occupies a cell, show just one (any one will do).

	// TODO:  For each zurt, set the cell to that zurt's color character. *PROVISIONALLY DONE*
	for (int k = 0; k < m_nZurts; k++)
	{
		if (!m_zurts[k]->isDead())
			displayGrid[m_zurts[k]->row() - 1][m_zurts[k]->col() - 1] = m_zurts[k]->color();

	}

	// Indicate player's position
	if (m_player != nullptr)
		displayGrid[m_player->row() - 1][m_player->col() - 1] = (m_player->isDead() ? 'X' : '@');

	// Draw the grid
	clearScreen();
	for (r = 1; r <= rows(); r++)
	{
		for (c = 1; c <= cols(); c++)
			cout << displayGrid[r - 1][c - 1];
		cout << endl;
	}
	cout << endl;

	// Write message, zurt, and player info
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << zurtCount() << " zurts remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player!" << endl;
	else if (m_player->isDead())
		cout << "The player is dead." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
	checkPos(r, c);
	m_grid[r - 1][c - 1] = status;
}

bool Arena::addZurt(int r, int c, char color)
{
	if (m_nZurts == MAXZURTS)
		return false;
	m_zurts[m_nZurts] = new Zurt(this, r, c, color);
	m_nZurts++;
	return true;
}

bool Arena::addPlayer(int r, int c)
{
	if (m_player != nullptr)
		return false;
	m_player = new Player(this, r, c);
	return true;
}

string Arena::moveZurts(char color, int dir)
{
	// Zurts of the indicated color will follow that color with probability 1/2
	bool willFollow = (randInt(0, 1) == 0);

	// Move all zurts
	int nZurtsOriginally = m_nZurts;

	// TODO:  Move each zurt.  Force zurts of the indicated color to move
	//        in the indicated direction if willFollow is true.  If
	//        willFollow is false, or if the zurt is of a different color,
	//        it just moves.  Mark the player as dead necessary.  Release
	//        any dead dynamically allocated zurt.
	for (int k = 0; k < m_nZurts; k++)
	{
		if (m_zurts[k]->color() == color && willFollow == true)
		{
			m_zurts[k]->forceMove(dir);
			if (m_zurts[k]->isDead())
			{
				delete m_zurts[k];
				for (int l = k; l < m_nZurts; l++)
				{
					m_zurts[l] = m_zurts[l + 1];
				}
				m_nZurts--;
			}
		}
		else
			m_zurts[k]->move();
	}

	if (m_nZurts < nZurtsOriginally)
		return "Some zurts have been destroyed.";
	else
		return "No zurts were destroyed.";
}

void Arena::checkPos(int r, int c) const
{
	if (r < 1 || r > m_rows || c < 1 || c > m_cols)
	{
		cout << "***** " << "Invalid arena position (" << r << ","
			<< c << ")" << endl;
		exit(1);
	}
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nZurts)
{
	if (nZurts < 0 || nZurts > MAXZURTS)
	{
		cout << "***** Game created with invalid number of zurts:  "
			<< nZurts << endl;
		exit(1);
	}
	int nEmpty = rows * cols - nZurts - 1;  // 1 for Player
	if (nEmpty < 0)
	{
		cout << "***** Game created with a " << rows << " by "
			<< cols << " arena, which is too small too hold a player and "
			<< nZurts << " zurts!" << endl;
		exit(1);
	}

	// Create arena
	m_arena = new Arena(rows, cols);

	// Add some walls in WALL_DENSITY of the empty spots
	assert(WALL_DENSITY >= 0 && WALL_DENSITY <= 1);
	int nWalls = static_cast<int>(WALL_DENSITY * nEmpty);
	while (nWalls > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		if (m_arena->getCellStatus(r, c) == WALL)
			continue;
		m_arena->setCellStatus(r, c, WALL);
		nWalls--;
	}

	// Add player
	int rPlayer;
	int cPlayer;
	do
	{
		rPlayer = randInt(1, rows);
		cPlayer = randInt(1, cols);
	} while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
	m_arena->addPlayer(rPlayer, cPlayer);

	// Populate with zurts
	while (nZurts > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		if (m_arena->getCellStatus(r, c) != EMPTY || (r == rPlayer && c == cPlayer))
			continue;
		const char colors[MAXCOLORS] = { 'R', 'Y', 'B' };
		m_arena->addZurt(r, c, colors[randInt(1, MAXCOLORS) - 1]);
		nZurts--;
	}
}

Game::~Game()
{
	delete m_arena;
}

string Game::takePlayerTurn()
{
	for (;;)
	{
		cout << "Your move (n/e/s/w/x or nothing): ";
		string playerMove;
		getline(cin, playerMove);

		Player* player = m_arena->player();
		int dir;

		if (playerMove.size() == 0)
		{
			if (recommendMove(*m_arena, player->row(), player->col(), dir))
				return player->move(dir);
			else
				return player->stand();
		}
		else if (playerMove.size() == 1)
		{
			if (tolower(playerMove[0]) == 'x')
				return player->stand();
			else if (charToDir(playerMove[0], dir))
				return player->move(dir);
		}
		cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
	}
}

string Game::takeZurtsTurn()
{
	for (;;)
	{
		cout << "Color thrown and direction (e.g., Rn or bw): ";
		string colorAndDir;
		getline(cin, colorAndDir);
		if (colorAndDir.size() != 2)
		{
			cout << "You must specify a color followed by a direction." << endl;
			continue;
		}
		char color = toupper(colorAndDir[0]);
		if (color != 'R'  &&  color != 'Y'  &&  color != 'B')
		{
			cout << "Color must be upper or lower R, Y, or B." << endl;
			continue;
		}
		int dir;
		if (charToDir(colorAndDir[1], dir))
			return m_arena->moveZurts(color, dir);
		else
			cout << "Direction must be n, e, s, or w." << endl;
	}
}

void Game::play()
{
	m_arena->display("");
	Player* player = m_arena->player();
	while (!player->isDead() && m_arena->zurtCount() > 0)
	{
		string msg = takePlayerTurn();
		m_arena->display(msg);
		if (player->isDead())
			break;
		msg = takeZurtsTurn();
		m_arena->display(msg);
	}
	if (player->isDead())
		cout << "You lose." << endl;
	else
		cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations (except clearScreen)
///////////////////////////////////////////////////////////////////////////

// Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
	if (highest < lowest)
		swap(highest, lowest);
	return lowest + (rand() % (highest - lowest + 1));
}

bool charToDir(char ch, int& dir)
{
	switch (tolower(ch))
	{
	default:  return false;
	case 'n': dir = NORTH; break;
	case 'e': dir = EAST;  break;
	case 's': dir = SOUTH; break;
	case 'w': dir = WEST;  break;
	}
	return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would hit a wall or run off the edge of the
// arena.  

//Otherwise, update r and c to the position resulting from the
// move and return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
	// TODO:  implement this function
	int rr = r;
	int cc = c;
	switch (dir)
	{
	case NORTH:
		if (r <= 1)
			return false;
		else
			rr--;
		break;
	case EAST:
		if (c >= a.cols())
			return false;
		else
			cc++;
		break;
	case SOUTH:
		if (r >= a.cols())
			return false;
		else
			rr++;
		break;
	case WEST:
		if (c <= 1)
			return false;
		else
			cc--;
		break;
	}
	r = rr;
	c = cc;
	return true;
	// PROVISIONALLY DONE
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should stand; otherwise, bestDir is
// set to the recommended direction to move.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
	// TODO:  replace this implementation:
	// Delete the following line and replace it with the correct code.
	int doNothingRisk = findRisk(a, r, c);

	if (doNothingRisk > 0) // check whether moving will be safer
	{
		int recomendedMoveRisk = doNothingRisk;
		int recommendedDir = NORTH;  // initialization

		for (int dir = 0; dir < NUMDIRS; dir++) // check n, e, s, w to see if anything is better than x
		{
			int rr = r;
			int cc = c;
			if (attemptMove(a, dir, rr, cc))
			{
				int danger = findRisk(a, rr, cc);
				if (danger < recomendedMoveRisk)
				{
					recomendedMoveRisk = danger;
					recommendedDir = dir;

				}

			}

		}
		if (recomendedMoveRisk < doNothingRisk) // if standing proves to be more dangerous than moving, reccomend move to be the best direction
		{
			bestDir = recommendedDir;
			return true;
		}
		// Your replacement implementation should do something intelligent.
		// For example, if you're standing next to four zurts, and moving
		// north would put you next to two zurts, but moving east would put
		// you next to none, moving east is a safer choice than standing or
		// moving north.
	}
	return false; // just standd!
}

int findRisk(const Arena& a, int r, int c) // check the # of zurts that might enter position r,c.
{

	if (a.numberOfZurtsAt(r, c) > 0)
		return MAXZURTS + 1;

	int risk = 0;
	if (r > 1)
		risk = risk + a.numberOfZurtsAt(r - 1, c);
	if (r < a.rows())
		risk = risk + a.numberOfZurtsAt(r + 1, c);
	if (c > 1)
		risk = risk + a.numberOfZurtsAt(r, c - 1);
	if (c < a.cols())
		risk = risk + a.numberOfZurtsAt(r, c + 1);

	return risk;
}


// Your replacement implementation should do something intelligent.
// For example, if you're standing next to four zurts, and moving
// north would put you next to two zurts, but moving east would put
// you next to none, moving east is a safer choice than standing or
// moving north.


///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

void doBasicTests();
int main()
{

	// Initialize the random number generator
	srand(static_cast<unsigned int>(time(0)));

	// Create a game
	// Use this instead to create a mini-game:   Game g(3, 5, 2);
	Game g(10, 12, 50);
	// doBasicTests();
	// Play the game
	g.play();
}
/* void thisFunctionWillNeverBeCalled()
{
// If the student deleted or changed the manifest constants, this
// won't compile.

const bool b1 = (MAXROWS == 20 && MAXCOLS == 20 && MAXZURTS == 100 &&
MAXCOLORS == 3 && EMPTY == 0 && WALL == 1 &&
NORTH == 0 && EAST == 1 && SOUTH == 2 && WEST == 3);
char a1[b1 ? 1 : -1];  // illegal negative size array if b1 is false

// If the student deleted or changed the interfaces to the public
// functions, this won't compile.  (This uses magic beyond the scope
// of CS 31.)

Zurt z(static_cast<Arena*>(0), 1, 1, 1);
int  (Zurt::*pz1)() const = &Zurt::row;
int  (Zurt::*pz2)() const = &Zurt::col;
char (Zurt::*pz3)() const = &Zurt::color;
bool (Zurt::*pz4)() const = &Zurt::isDead;
void (Zurt::*pz5)(int) = &Zurt::forceMove;
void (Zurt::*pz6)() = &Zurt::move;

Player p(static_cast<Arena*>(0), 1, 1);
int  (Player::*pp1)() const = &Player::row;
int  (Player::*pp2)() const = &Player::col;
bool (Player::*pp3)() const = &Player::isDead;
string(Player::*pp4)() = &Player::stand;
string(Player::*pp5)(int) = &Player::move;
void (Player::*pp6)() = &Player::setDead;

Arena a(1, 1);
int  (Arena::*pa1)() const = &Arena::rows;
int  (Arena::*pa2)() const = &Arena::cols;
Player* (Arena::*pa3)() const = &Arena::player;
int (Arena::*pa4)() const = &Arena::zurtCount;
int (Arena::*pa5)(int, int) const = &Arena::getCellStatus;
int (Arena::*pa6)(int, int) const = &Arena::numberOfZurtsAt;
void (Arena::*pa7)(string) const = &Arena::display;
void (Arena::*pa8)(int, int, int) = &Arena::setCellStatus;
bool (Arena::*pa9)(int, int, char) = &Arena::addZurt;
bool (Arena::*pa10)(int, int) = &Arena::addPlayer;
string(Arena::*pa11)(char, int) = &Arena::moveZurts;

Game g(1, 1, 1);
void (Game::*pg1)() = &Game::play;
}

void doBasicTests()
{
{
Arena a(10, 20);
a.addPlayer(2, 5);
Player* pp = a.player();
assert(pp->row() == 2 && pp->col() == 5 && !pp->isDead());
assert(pp->move(NORTH) == "Player moved north.");
assert(pp->row() == 1 && pp->col() == 5 && !pp->isDead());
assert(pp->move(NORTH) == "Player couldn't move; player stands.");
assert(pp->row() == 1 && pp->col() == 5 && !pp->isDead());
pp->setDead();
assert(pp->row() == 1 && pp->col() == 5 && pp->isDead());
}
{
Arena a(10, 20);
a.setCellStatus(1, 4, WALL);
a.setCellStatus(1, 5, WALL);
a.setCellStatus(3, 4, WALL);
a.setCellStatus(3, 5, WALL);
a.setCellStatus(2, 3, WALL);
a.setCellStatus(2, 6, WALL);
a.addZurt(2, 4, 'R');
a.addPlayer(7, 7);
assert(a.zurtCount() == 1 && a.numberOfZurtsAt(2, 4) == 1);
for (int k = 0; k < 100 && a.numberOfZurtsAt(2, 4) == 1; k++)
a.moveZurts('R', EAST);
assert(a.numberOfZurtsAt(2, 4) == 0 && a.numberOfZurtsAt(2, 5) == 1);
for (int k = 0; k < 100 && a.zurtCount() == 1; k++)
a.moveZurts('Y', EAST);
assert(a.zurtCount() == 1);
assert(a.numberOfZurtsAt(2, 4) == 1 || a.numberOfZurtsAt(2, 5) == 1);
for (int k = 0; k < 100 && a.zurtCount() == 1; k++)
a.moveZurts('R', EAST);
assert(a.zurtCount() == 0);
assert(a.numberOfZurtsAt(2, 4) == 0 && a.numberOfZurtsAt(2, 5) == 0);
for (int k = 0; k < MAXZURTS / 4; k++)
{
a.addZurt(6, 7, 'B');
a.addZurt(8, 7, 'B');
a.addZurt(7, 6, 'B');
a.addZurt(7, 8, 'B');
}
assert(!a.player()->isDead());
a.moveZurts('Y', NORTH);
assert(a.player()->isDead());
}
cout << "Passed all basic tests" << endl;
exit(0);
}
*/


///////////////////////////////////////////////////////////////////////////
//  clearScreen implementations
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <cstring>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif

