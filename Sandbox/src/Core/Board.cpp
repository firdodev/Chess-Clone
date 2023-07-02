#include "Board.h"
#include "../Log/Logger.h"


Board::Board(uint32_t width, uint32_t height): m_width(width), m_height(height), m_isWhiteTurn(true), m_end(false) {
    ChessPiece initialBoard[8][8] = {
        { ChessPiece::BLACK_ROOK, ChessPiece::BLACK_KNIGHT, ChessPiece::BLACK_BISHOP, ChessPiece::BLACK_QUEEN, ChessPiece::BLACK_KING, ChessPiece::BLACK_BISHOP, ChessPiece::BLACK_KNIGHT, ChessPiece::BLACK_ROOK },
        { ChessPiece::BLACK_PAWN, ChessPiece::BLACK_PAWN, ChessPiece::BLACK_PAWN, ChessPiece::BLACK_PAWN, ChessPiece::BLACK_PAWN, ChessPiece::BLACK_PAWN, ChessPiece::BLACK_PAWN, ChessPiece::BLACK_PAWN },
        { ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY },
        { ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY },
        { ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY },
        { ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY, ChessPiece::EMPTY },
        { ChessPiece::WHITE_PAWN, ChessPiece::WHITE_PAWN, ChessPiece::WHITE_PAWN, ChessPiece::WHITE_PAWN, ChessPiece::WHITE_PAWN, ChessPiece::WHITE_PAWN, ChessPiece::WHITE_PAWN, ChessPiece::WHITE_PAWN },
        { ChessPiece::WHITE_ROOK, ChessPiece::WHITE_KNIGHT, ChessPiece::WHITE_BISHOP, ChessPiece::WHITE_QUEEN, ChessPiece::WHITE_KING, ChessPiece::WHITE_BISHOP, ChessPiece::WHITE_KNIGHT, ChessPiece::WHITE_ROOK }
    };

    if (!m_pieceTexture.loadFromFile("./assets/pieces_spritesheet.png")) {
        // Handle error
        LOG_ERROR("Failed to load sprite sheet.");
    }
    else {
        LOG_INFO("Loaded spritesheet correctly.");
    }
    m_pieceSprite.setTexture(m_pieceTexture);


    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            m_boardState[i][j] = Piece(initialBoard[i][j]);

            sf::RectangleShape square(sf::Vector2f((float)m_width / 8 * 50, (float)m_height / 8 * 50));
            square.setPosition((float)j * (m_width / 8), (float)i * (m_height / 8));
            
            if ((i + j) % 2 == 0) {
                square.setFillColor(sf::Color(240, 217, 181));
            }
            else {
                square.setFillColor(sf::Color(181, 136, 99));
            }
            m_squares.push_back(square);
        }
    }

    setOffset(0.1f);
    setPieceSize(88.0f);
    setHorizontalSpacing(19.0f);
    setVerticalSpacing(9.5f);
}

Board::~Board() {
}

void Board::drawBoard(sf::RenderWindow &window) {
    for (size_t i = 0; i < m_squares.size(); ++i) {
        if (i == m_selectedSquareIndex) {
            sf::RectangleShape highlight = m_squares[i];
            highlight.setFillColor(sf::Color(7, 54, 66, 100));
            window.draw(highlight);
        }
        else {
            window.draw(m_squares[i]);
        }
    }

    // Draw a circle for each valid move
    if (m_selectedPiece != nullptr) {

        int x = -1;
        int y = -1;

        for (size_t i = 0; i < 8; i++) {
            for (size_t j = 0; j < 8; j++) {
                if (&m_boardState[i][j] == m_selectedPiece) {
                    x = j;
                    y = i;
                    break;
                }
            }
        }
        
        std::vector<std::pair<int, int>> validMoves = getAllValidMoves(x, y);

        for (const auto& move : validMoves) {
            sf::CircleShape circle(7);
            circle.setFillColor(sf::Color(7, 54, 66, 100));
            circle.setPosition(move.first * (m_width / 8) + (m_width / 8) / 2, move.second * (m_height / 8) + (m_height / 8) / 2);
            window.draw(circle);
        }
    }
}

void Board::drawPieces(sf::RenderWindow& window) {

    float pieceSize = this->getPieceSize();
    float offset = this->getOffset();
    float horizontalSpacing = this->getHorizontalSpacing();
    float verticalSpacing = this->getVerticalSpacing();

    float squareSize = (float)m_width / 8; // computed the size of a square
    float pieceOffset = (squareSize - pieceSize) / 2; // computed the offset needed to center a piece in a square

    int pieceTextureSize = m_pieceTexture.getSize().x / 6;

    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            Piece piece = m_boardState[i][j];

            if (piece.getType() != ChessPiece::EMPTY) {
                int pieceIndex = static_cast<int>(std::abs(static_cast<int>(piece.getType()))) - 1;
                bool isWhite = static_cast<int>(piece.getType()) > 0;

                m_pieceSprite.setTextureRect(sf::IntRect(pieceIndex * pieceTextureSize, isWhite ? 0 : pieceTextureSize, pieceTextureSize, pieceTextureSize));

                // Adjust position if the piece is white.
                float adjustedVerticalSpacing = isWhite ? verticalSpacing - 0.1 : verticalSpacing;

                m_pieceSprite.setPosition(j * pieceSize + offset + j * horizontalSpacing, i * pieceSize + offset + i * adjustedVerticalSpacing);
                m_pieceSprite.setScale(pieceSize / pieceTextureSize, pieceSize / pieceTextureSize);

                window.draw(m_pieceSprite);
            }

        }
    }

}

void Board::selectPiece(int x, int y) {
    if (m_selectedPiece != nullptr) {
        m_selectedPiece->deselect();
    }

    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        m_selectedPiece = &m_boardState[y][x];
        m_selectedPiece->setSelected(true);
        selectSquare(x, y);
    }
    else {
        m_selectedPiece = nullptr;
        selectSquare(-1, -1);
    }

    LOG_INFO("Selected piece: ", x, ":", y);
}

Piece* Board::getSelectedPiece() const {
    return m_selectedPiece;
}

void Board::setPiece(int x, int y, Piece piece) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        m_boardState[x][y] = piece;
    }
}

Piece Board::getPiece(int x, int y) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        return m_boardState[x][y];
    }
    return Piece();
}

bool Board::isValidMove(int srcX, int srcY, int destX, int destY) {
    // Basic boundary check
    if (srcX < 0 || srcY < 0 || destX < 0 || destY < 0 || srcX >= 8 || srcY >= 8 || destX >= 8 || destY >= 8)
        return false;

    Piece srcPiece = getPiece(srcY, srcX);
    Piece destPiece = getPiece(destY, destX);

    if (srcX == destX && srcY == destY)
        return false;

    // Check if destination square is occupied by a piece of the same color
    if (destPiece.getType() != ChessPiece::EMPTY && ((int)srcPiece.getType() * (int)destPiece.getType() > 0))
        return false;

    int deltaX = abs(destX - srcX);
    int deltaY = abs(destY - srcY);

    // For pawn
    if (srcPiece.getType() == ChessPiece::WHITE_PAWN || srcPiece.getType() == ChessPiece::BLACK_PAWN) {
        // Check if the pawn is moving backwards
        if ((srcPiece.getType() == ChessPiece::WHITE_PAWN && destY > srcY) ||
            (srcPiece.getType() == ChessPiece::BLACK_PAWN && destY < srcY)) {
            return false;
        }

        // Pawn can move forward one square if it's not blocked
        if (deltaX == 0 && deltaY == 1 && destPiece.getType() == ChessPiece::EMPTY)
            return true;
        // Pawn can capture one square diagonally forward
        else if (deltaX == 1 && deltaY == 1 && destPiece.getType() != ChessPiece::EMPTY)
            return true;
        else if (!srcPiece.hasMoved() && deltaX == 0 && deltaY == 2 && destPiece.getType() == ChessPiece::EMPTY)
            return true;
        else
            return false;
    }

    // For rook
    else if (srcPiece.getType() == ChessPiece::WHITE_ROOK || srcPiece.getType() == ChessPiece::BLACK_ROOK) {
        return (deltaX == 0 || deltaY == 0) && isPathClear(srcX, srcY, destX, destY);
    }

    // For knight
    else if (srcPiece.getType() == ChessPiece::WHITE_KNIGHT || srcPiece.getType() == ChessPiece::BLACK_KNIGHT) {
        // Knight can move to a square that is two squares away horizontally and one square vertically, or two squares vertically and one square horizontally
        return ((deltaX == 1 && deltaY == 2) || (deltaX == 2 && deltaY == 1));
    }

    // For bishop
    else if (srcPiece.getType() == ChessPiece::WHITE_BISHOP || srcPiece.getType() == ChessPiece::BLACK_BISHOP) {
        return deltaX == deltaY && isPathClear(srcX, srcY, destX, destY);
    }

    // For queen
    else if (srcPiece.getType() == ChessPiece::WHITE_QUEEN || srcPiece.getType() == ChessPiece::BLACK_QUEEN) {
        return (deltaX == 0 || deltaY == 0 || deltaX == deltaY) && isPathClear(srcX, srcY, destX, destY);
    }

    // For king
    else if (srcPiece.getType() == ChessPiece::WHITE_KING || srcPiece.getType() == ChessPiece::BLACK_KING) {
        return (deltaX <= 1 && deltaY <= 1);
    }

    else {
        return false;
    }

    return true;
}

bool Board::isKingCheck(ChessPiece kingColor) {
    int kingX, kingY;

    // Find the king's position
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (m_boardState[i][j].getType() == kingColor) {
                kingX = j;
                kingY = i;
            }
        }
    }

    // Now, check if any opposing piece can move to the King's position
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // Ignore the empty squares and pieces of the same color as the king
            if (m_boardState[i][j].getType() == ChessPiece::EMPTY || (int)m_boardState[i][j].getType() * (int)kingColor > 0)
                continue;

            // If an opposing piece can move to the King's position, then the King is in check
            if (isValidMove(j, i, kingX, kingY))
                return true;
        }
    }

    return false;
}

bool Board::movePiece(int srcX, int srcY, int destX, int destY) {
    if (!isValidMove(srcX, srcY, destX, destY) || m_end) {
        return false;
    }

    // Check if the King will be in check after this move
    Piece srcPiece = m_boardState[srcY][srcX];

    

    bool isWhitePiece = (int)srcPiece.getType() > 0;
    if (isWhitePiece != m_isWhiteTurn) {
        LOG_INFO("Not the turn of the selected piece.");
        return false;
    }

    ChessPiece kingColor = (int)srcPiece.getType() > 0 ? ChessPiece::WHITE_KING : ChessPiece::BLACK_KING;

    // Temporary make the move
    Piece destPiece = m_boardState[destY][destX];
    m_boardState[destY][destX] = srcPiece;
    m_boardState[srcY][srcX] = Piece(ChessPiece::EMPTY);

    bool kingInCheck = isKingCheck(kingColor);
    bool kingInCheckMate = false;

    if (kingInCheck) {
        kingInCheckMate = isCheckMate(kingColor);
    }

    // Undo the move
    m_boardState[srcY][srcX] = srcPiece;
    m_boardState[destY][destX] = destPiece;

    // If the King will be in check after this move, the move is invalid
    if (kingInCheck) {
        if (kingInCheckMate) {
            std::string winningColor = isWhitePiece ? "Black" : "White";
            LOG_WARN("King " , winningColor , " won.");
        }
        return false;
    }

    // If the King will be in check after this move, the move is invalid
    if (kingInCheck) {
        return false;
    }



    // Make the move for real
    m_boardState[destY][destX] = srcPiece;
    m_boardState[destY][destX].setMoved(true);
    m_boardState[srcY][srcX] = Piece(ChessPiece::EMPTY);

    // Switch turns only if the move was successful
    m_isWhiteTurn = !m_isWhiteTurn;

    return true;
}

bool Board::isPathClear(int srcX, int srcY, int destX, int destY) {
    int xStep = (destX > srcX) - (srcX > destX);
    int yStep = (destY > srcY) - (srcY > destY);

    srcX += xStep;
    srcY += yStep;

    while (srcX != destX || srcY != destY) {
        if (m_boardState[srcY][srcX].getType() != ChessPiece::EMPTY)
            return false;

        srcX += xStep;
        srcY += yStep;
    }

    return true;
}

void Board::setPieceSize(float pieceSize) {
    this->m_pieceSize = pieceSize;
}

float Board::getPieceSize() const {
    return this->m_pieceSize;
}

void Board::setOffset(float offset) {
    this->m_offset = offset;
}

float Board::getOffset() const {
    return this->m_offset;
}

void Board::setHorizontalSpacing(float spacing) {
    this->m_horizontalSpacing = spacing;
}

float Board::getHorizontalSpacing() const {
    return this->m_horizontalSpacing;
}

bool Board::isCheckMate(ChessPiece kingColor) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // Ignore the empty squares and pieces of the different color
            if (m_boardState[i][j].getType() == ChessPiece::EMPTY ||
                (int)m_boardState[i][j].getType() * (int)kingColor < 0)
                continue;

            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {
                    if (movePiece(j, i, l, k)) {
                        // undo the move
                        m_boardState[i][j] = m_boardState[k][l];
                        m_boardState[k][l] = Piece(ChessPiece::EMPTY);
                        m_end = false;
                        return m_end; // return false immediately if found a valid move
                    }
                }
            }
        }
    }
    m_end = true;
    return m_end;
}

void Board::setVerticalSpacing(float spacing) {
    this->m_verticalSpacing = spacing;
}

float Board::getVerticalSpacing() const {
    return this->m_verticalSpacing;
}

void Board::selectSquare(int x, int y) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        m_selectedSquareIndex = y * 8 + x;
    }
    else {
        m_selectedSquareIndex = -1;
    }

    LOG_INFO("Selected square: ", x, ":", y);
}

std::vector<std::pair<int, int>> Board::getAllValidMoves(int srcX, int srcY) {
    std::vector<std::pair<int, int>> validMoves;

    for (int destX = 0; destX < 8; ++destX) {
        for (int destY = 0; destY < 8; ++destY) {
            if (isValidMove(srcX, srcY, destX, destY)) {
                validMoves.push_back(std::make_pair(destX, destY));
            }
        }
    }

    return validMoves;
}
