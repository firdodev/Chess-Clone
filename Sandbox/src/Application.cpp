#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>

#include "Log/Logger.h"
#include "Core/Board.h"

int WIDTH = 825;
int HEIGHT = 768;

std::string PieceTypeToString(ChessPiece type) {
    switch (type) {
    case ChessPiece::EMPTY: return "Empty";
    case ChessPiece::WHITE_PAWN: return "White Pawn";
    case ChessPiece::BLACK_PAWN: return "Black Pawn";
    case ChessPiece::WHITE_QUEEN: return "White Queen";
    case ChessPiece::BLACK_QUEEN: return "Black Queen";
    case ChessPiece::WHITE_KING: return "White King";
    case ChessPiece::BLACK_KING: return "Black King";
    case ChessPiece::WHITE_ROOK: return "White Rook";
    case ChessPiece::BLACK_ROOK: return "Black Rook";
    case ChessPiece::WHITE_BISHOP: return "White Bishop";
    case ChessPiece::BLACK_BISHOP: return "Black Bishop";
    case ChessPiece::WHITE_KNIGHT: return "White Knight";
    case ChessPiece::BLACK_KNIGHT: return "Black Knight";
    default: return "Unknown";
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Chess Game", sf::Style::Close);
    ImGui::SFML::Init(window);

    window.setFramerateLimit(10);

    Board board(WIDTH, HEIGHT);

    float offset = board.getOffset();
    float pieceSize = board.getPieceSize();

    float horizontalSpacing = board.getHorizontalSpacing();
    float verticalSpacing = board.getVerticalSpacing();


    int srcX = -1, srcY = -1, destX = -1, destY = -1;

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    srcX = event.mouseButton.x / (WIDTH / 8);
                    srcY = event.mouseButton.y / (HEIGHT / 8);
                    board.selectPiece(srcX, srcY);
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    destX = event.mouseButton.x / (WIDTH / 8);
                    destY = event.mouseButton.y / (HEIGHT / 8);
                    if (srcX != -1 && srcY != -1) {
                        board.movePiece(srcX, srcY, destX, destY);
                        srcX = -1;
                        srcY = -1;
                    }
                }
                break;
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        // Set window size and position
        ImGui::SetNextWindowSize(ImVec2(200, 300));
        ImGui::SetNextWindowPos(ImVec2((float)WIDTH - 200, 0));

        // Start the ImGui frame with ImGuiWindowFlags_NoMove and ImGuiWindowFlags_NoResize
        ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        ImGui::Text("Board settings:");
        if (ImGui::SliderFloat("Offset", &offset, 0.0f, 50.0f)) {
            board.setOffset(offset);
        }

        if (ImGui::SliderFloat("Piece Size", &pieceSize, 0.0f, 200.0f)) {
            board.setPieceSize(pieceSize);
        }
        ImGui::Separator();

        ImGui::Text("\n Spacing between pieces");

        if (ImGui::SliderFloat("Horizontal Spacing", &horizontalSpacing, -50.0f, 50.0f)) {
            board.setHorizontalSpacing(horizontalSpacing);
        }

        if (ImGui::SliderFloat("Vertical Spacing", &verticalSpacing, -50.0f, 50.0f)) {
            board.setVerticalSpacing(verticalSpacing);
        }
        ImGui::Separator();

        Piece* selectedPiece = board.getSelectedPiece();
        if (selectedPiece != nullptr) {
            ImGui::Text("Selected Piece: %s", PieceTypeToString(selectedPiece->getType()).c_str());
        }
        else {
            ImGui::Text("No piece selected.");
        }

        float frameTime = ImGui::GetIO().DeltaTime;
        float FPS = 1.0f / frameTime;
        ImGui::Text("FPS: %.1f", FPS);

        ImGui::End();

        window.clear();
        
        board.drawBoard(window);
        board.drawPieces(window);

        ImGui::SFML::Render(window);
        window.display();

    }
    
    ImGui::SFML::Shutdown();

    return 0;
}
