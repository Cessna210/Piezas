/**
 * Unit Tests for Piezas
**/

#include <gtest/gtest.h>
#include "Piezas.h"
 
class PiezasTest : public ::testing::Test
{
	protected:
		PiezasTest(){} //constructor runs before each test
		virtual ~PiezasTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor) 
};

TEST(PiezasTest, ResetWorks)
{
  Piezas p;
  for(int i = 0; i < BOARD_COLS; i++)
    p.dropPiece(i);
    
  p.reset();
  
  // Prove that the board was reset
  for(int i = 0; i < BOARD_ROWS; i++)
  {
    for(int j = 0; j < BOARD_COLS; j++)
      ASSERT_EQ(Blank, p.pieceAt(i, j));
  }
}

TEST(PiezasTest, PieceAtNegativeRowIsInvalid)
{
  Piezas p;
  ASSERT_EQ(Invalid, p.pieceAt(-1, 0));
}

TEST(PiezasTest, PieceAtNegativeColIsInvalid)
{
  Piezas p;
  ASSERT_EQ(Invalid, p.pieceAt(0, -1));
}

TEST(PiezasTest, PieceAtOverflowRowIsInvalid)
{
  Piezas p;
  ASSERT_EQ(Invalid, p.pieceAt(BOARD_ROWS, 0));
}

TEST(PiezasTest, PieceAtOverflowColIsInvalid)
{
  Piezas p;
  ASSERT_EQ(Invalid, p.pieceAt(0, BOARD_COLS));
}

TEST(PiezasTest, PieceAtIsInitiallyBlank)
{
  Piezas p;
  for(int i = 0; i < BOARD_ROWS; i++)
  {
    for(int j = 0; j < BOARD_COLS; j++)
      ASSERT_EQ(Blank, p.pieceAt(i, j));
  }
}

TEST(PiezasTest, DropPieceWorks)
{
  Piezas p;
  p.dropPiece(0);
  ASSERT_EQ(X, p.pieceAt(0, 0));
  ASSERT_EQ(Blank, p.pieceAt(1, 0));//Make sure above spot remains blank
}

TEST(PiezasTest, DropPieceSwitchesTurnXO)
{
  Piezas p;
  ASSERT_EQ(X, p.dropPiece(0));
  ASSERT_EQ(O, p.dropPiece(1));
}

TEST(PiezasTest, DropPieceSwitchesTurnXOX)
{
  Piezas p;
  ASSERT_EQ(X, p.dropPiece(0));
  ASSERT_EQ(O, p.dropPiece(1));
  ASSERT_EQ(X, p.dropPiece(1));
}

TEST(PiezasTest, DropPieceAtNegativeColIsInvalid)
{
  Piezas p;
  ASSERT_EQ(Invalid, p.dropPiece(-1));
}

TEST(PiezasTest, DropPieceAtOverflowColIsInvalid)
{
  Piezas p;
  ASSERT_EQ(Invalid, p.dropPiece(BOARD_COLS));
}

TEST(PiezasTest, DropOutOfBoundsLosesTurn)
{
  Piezas p;
  ASSERT_EQ(Invalid, p.dropPiece(-1));//X loses their turn
  ASSERT_EQ(O, p.dropPiece(0));//O's turn next
}

TEST(PiezasTest, DropPieceFallsToBottom)
{
  Piezas p;
  ASSERT_EQ(X, p.dropPiece(0));
  ASSERT_EQ(X, p.pieceAt(0,0));
  ASSERT_EQ(Blank, p.pieceAt(1,0));//Make sure the spot above is still Blank
}

TEST(PiezasTest, DropPieceWillStack)
{
  Piezas p;

  //Drop two pieces in the same col
  ASSERT_EQ(X, p.dropPiece(0));
  ASSERT_EQ(O, p.dropPiece(0));
  
  //Ensure that they are stacked
  ASSERT_EQ(X, p.pieceAt(0, 0));
  ASSERT_EQ(O, p.pieceAt(1, 0));
  ASSERT_EQ(Blank, p.pieceAt(2, 0));//Make sure above is Blank
}

TEST(PiezasTest, DropPieceFullColFails)
{
  Piezas p;

  //Fill col 0
  for(int i = 0; i < BOARD_ROWS; i++)
    p.dropPiece(0);

  //Try to drop another piece
  ASSERT_EQ(Blank, p.dropPiece(0));
}

TEST(PiezasTest, GameStateInitiallyInvalid)
{
  Piezas p;
  ASSERT_EQ(Invalid, p.gameState());
}

TEST(PiezasTest, GameStateInvalidWhenIncomplete)
{
  Piezas p;
  p.dropPiece(0);
  ASSERT_EQ(Invalid, p.gameState());
}

TEST(PiezasTest, GameStateBlankOnCheckerTie)
{
  Piezas p;
  //Force a tie
  for(int i = 0; i < BOARD_COLS; i++)
  {
    for(int j = 0; j < BOARD_ROWS; j++)
      p.dropPiece(i);
  }

  ASSERT_EQ(Blank, p.gameState());
}

TEST(PiezasTest, GameStateBlankOnHorizontalTie)
{
  Piezas p;

  //Place two horizontal lines (tie)
  p.dropPiece(1);//X
  p.dropPiece(1);//O
  p.dropPiece(2);//X
  p.dropPiece(2);//O
  p.dropPiece(3);//X
  p.dropPiece(3);//O

  //Fill the rest with non-ties
  p.dropPiece(3);//X
  p.dropPiece(2);//O
  p.dropPiece(1);//X
  p.dropPiece(0);//O
  p.dropPiece(0);//X
  p.dropPiece(0);//O


  ASSERT_EQ(Blank, p.gameState());
}

TEST(PiezasTest, GameStateBlankOnVerticalTie)
{
  Piezas p;
  //Fill left two cols to tie
  for(int i = 0; i < 2; i++)
  {
    for(int j = 0; j < BOARD_ROWS; j++)
      p.dropPiece(i);
  }

  //Fill the remaining spots with non-tie
  for(int i = 2; i < BOARD_COLS; i++)
  {
    for(int j = 0; j < BOARD_ROWS; j++)
      p.dropPiece(i);
  }

  ASSERT_EQ(Blank, p.gameState());
}

TEST(PiezasTest, GameStateXWinVert)
{
  Piezas p;
  p.dropPiece(0);
  p.dropPiece(1);
  p.dropPiece(2);
  p.dropPiece(3);

  p.dropPiece(2);
  p.dropPiece(0);
  p.dropPiece(1);
  p.dropPiece(3);

  p.dropPiece(3);
  p.dropPiece(1);
  p.dropPiece(2);
  p.dropPiece(0);

  ASSERT_EQ(X, p.gameState());
}

TEST(PiezasTest, GameStateXWinHorizontal)
{
  Piezas p;
  for(int i = 0; i < 2; i++)
  {
    for(int j = 0; j < 2; j++)
      p.dropPiece(i);
  }

  p.dropPiece(2);//X
  p.dropPiece(0);//O
  p.dropPiece(3);//X

  for(int j = 0; j < BOARD_ROWS - 1; j++)
  {
    for(int i = 0; i < BOARD_COLS; i++)
    {
      if((i < 2 && j == 0))
        continue;//Already filled these locations
      if(i == 0 && j == 1)
        continue;//Already filled this location

      p.dropPiece(i);
    }
  }

  ASSERT_EQ(X, p.gameState());
}

TEST(PiezasTest, OCanWin)
{
  Piezas p;
  p.dropPiece(0);//X
  p.dropPiece(1);//O
  p.dropPiece(0);//X
  p.dropPiece(1);//O
  p.dropPiece(2);//X
  p.dropPiece(0);//O
  p.dropPiece(2);//X
  p.dropPiece(1);//O
  p.dropPiece(3);//X
  p.dropPiece(3);//O
  p.dropPiece(3);//X
  p.dropPiece(2);//O

  ASSERT_EQ(O, p.gameState());
}
