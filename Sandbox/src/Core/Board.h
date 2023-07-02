#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"

#include <iostream>
#include <vector>

class Board
{
public:
	Board(uint32_t width, uint32_t height);
	~Board();


public:
	void drawBoard(sf::RenderWindow& window);
	void drawPieces(sf::RenderWindow& window);

	void setPiece(int x, int y, Piece piece);
	Piece getPiece(int x, int y);

	void selectPiece(int x, int y);
	Piece* getSelectedPiece() const;

	bool isValidMove(int srcX, int srcY, int destX, int destY);
	bool movePiece(int srcX, int srcY, int destX, int destY);
	bool isPathClear(int srcX, int srcY, int destX, int destY);
	bool isKingCheck(ChessPiece kingColor);
	bool isCheckMate(ChessPiece kingColor);

	void setPieceSize(float pieceSize);
	float getPieceSize() const;

	void setOffset(float offset);
	float getOffset() const;

	void setHorizontalSpacing(float spacing);
	float getHorizontalSpacing() const;

	void setVerticalSpacing(float spacing);
	float getVerticalSpacing() const;

	void selectSquare(int x, int y);
	std::vector<std::pair<int, int>> getAllValidMoves(int srcX, int srcY);

private:
	std::vector<sf::RectangleShape> m_squares;
	Piece m_boardState[8][8];

	Piece* m_selectedPiece = nullptr;

	sf::Texture m_pieceTexture;
	sf::Sprite m_pieceSprite;

	uint32_t m_width;
	uint32_t m_height;

	float m_pieceSize;
	float m_offset;

	float m_horizontalSpacing;
	float m_verticalSpacing;

	bool m_isWhiteTurn;

	int m_selectedSquareIndex;

	bool m_end;
};

