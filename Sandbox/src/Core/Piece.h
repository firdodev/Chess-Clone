#pragma once

enum class ChessPiece {
    EMPTY = 0,
    WHITE_PAWN = 1,
    WHITE_ROOK = 2,
    WHITE_KNIGHT = 3,
    WHITE_BISHOP = 4,
    WHITE_QUEEN = 5,
    WHITE_KING = 6,
    BLACK_PAWN = -1,
    BLACK_ROOK = -2,
    BLACK_KNIGHT = -3,
    BLACK_BISHOP = -4,
    BLACK_QUEEN = -5,
    BLACK_KING = -6,
};

class Piece
{
private:
    ChessPiece m_type;
    bool m_hasMoved;
    bool m_isSelected;

public:
    Piece(ChessPiece type = ChessPiece::EMPTY);

    ChessPiece getType() const;
    bool hasMoved() const; // Return whether the piece has moved or not
    void setMoved(bool moved); // Set whether the piece has moved or not

    void setType(ChessPiece type);

    void setSelected(bool isSelected);
    bool isSelected() const;
    void deselect();

};
