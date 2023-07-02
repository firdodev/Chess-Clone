#include "Piece.h"

Piece::Piece(ChessPiece type) :m_type(type), m_hasMoved(false) {
}

ChessPiece Piece::getType() const {
    return m_type;
}

bool Piece::hasMoved() const {
    return m_hasMoved;
}

void Piece::setMoved(bool moved) {
    m_hasMoved = moved;
}

void Piece::setType(ChessPiece type) {
    m_type = type;
}

void Piece::setSelected(bool isSelected) {
    m_isSelected = isSelected;
}

bool Piece::isSelected() const {
    return m_isSelected;
}

void Piece::deselect() {
    m_isSelected = false;
}
