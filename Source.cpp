#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

using namespace sf;
using namespace std;

// ���������� ����������
int amount[2] = { 12, 12 };
int LEN = 0, turn = 0;
int turn_block = 0;
bool gameover = false;
bool is_move = false;
bool attack_turn = false;
int piece_size = 75;
int board[8][8] = { {0, 2, 0, 2, 0, 2, 0, 2},
                    {2, 0, 2, 0, 2, 0, 2, 0},
                    {0, 2, 0, 2, 0, 2, 0, 2},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {1, 0, 1, 0, 1, 0, 1, 0},
                    {0, 1, 0, 1, 0, 1, 0, 1},
                    {1, 0, 1, 0, 1, 0, 1, 0},
};

// ����������� ����� ������
class Piece {
public:
    bool is_change = false, is_necessary = true;
    int x, y, side;
    Texture texture;
    Sprite image;

    // ��������� ������� �������� ������
    void SetPosition(int x, int y) {
        this->x = x;
        this->y = y;
        image.setPosition(x * piece_size + piece_size / 2, y * piece_size + piece_size / 2);
    }

    virtual bool is_possible(int x, int y) = 0;

    virtual bool continue_attack() = 0;

    // ���������� ����
    bool move(int x, int y) {
        board[y][x] = side;
        bool attack = false;
        int x_move = (x - this->x) / abs(x - this->x), y_move = (y - this->y) / abs(y - this->y);

        for (int i = this->x, j = this->y; i != x; i += x_move, j += y_move) {
            if (board[j][i] && board[j][i] != side) {
                attack = true;
            }
            board[j][i] = 0;
        }

        SetPosition(x, y);

        if (side == 2 && y == 7 || side == 1 && y == 0) {
            is_change = true;
        }

        return attack;
    }

};

// ����� �����
class Queen : public Piece {
public:
    Queen() {
        is_necessary = false;
    }

    // �������� �����
    Queen (int x, int y, int side) {
        is_necessary = false;

        if (side == 1) {
            texture.loadFromFile("sprites/queenWhite.png");
            image.setTexture(texture);
        }
        else if (side == 2) {
            texture.loadFromFile("sprites/queenBlack.png");
            image.setTexture(texture);
        }

        this->side = side;
        SetPosition(x, y);
        image.setOrigin(piece_size / 2, piece_size / 2);
        image.setPosition(x * piece_size + piece_size / 2, y * piece_size + piece_size / 2);
    }

    // �������� ����������� ����
    bool is_possible(int x, int y) override {
        if (x < 0 || x > 7 || y < 0 || y > 7) {
            return false;
        }
        if (abs(y - this->y) == abs(x - this->x) && !board[y][x]) {
            int x_move = (x - this->x) / abs(x - this->x), y_move = (y - this->y) / abs(y - this->y);

            for (int i = this->x + x_move, j = this->y + y_move; i != x; i += x_move, j += y_move) {
                if (board[j][i] == side || board[j][i] != side && board[j - y_move][i - x_move] != side && board[j][i] && board[j - y_move][i - x_move]) {
                    return false;
                }
            }
            return true;
        }
        else {
            return false;
        }
    }

    bool check(int dx, int dy) {
        for (int i = x + dx, j = y + dy; i > 0 && i < 7 && j > 0 && j < 7; i += dx, j += dy) {
            if (board[j][i] == side || (board[j][i] != side && board[j + dy][i + dx] != side && board[j][i] && board[j + dy][i + dx])) {
                return false;
            }
            else if (board[j][i] && board[j][i] != side && !board[j + dy][i + dx]) {
                return true;
            }
        }

        return false;
    }

    bool continue_attack() override {
        if (check(1, 1) || check(-1, 1) || check(1, -1) || check(-1, -1)) {
            return true;
        }
        else {
            return false;
        }
    }
};

// ����� �����
class Checker : public Piece {
public:

    Checker() {

    }

    // �������� �����
    Checker(int x, int y, int side) {

        if (side == 1) {
            texture.loadFromFile("sprites/checkerWhite.png");
            image.setTexture(texture);
        }
        else if (side == 2) {
            texture.loadFromFile("sprites/checkerBlack.png");
            image.setTexture(texture);
        }

        this->side = side;
        SetPosition(x, y);
        image.setOrigin(piece_size / 2, piece_size / 2);
        image.setPosition(x * piece_size + piece_size / 2, y * piece_size + piece_size / 2);
    }

    // �������� ����������� ����
    bool is_possible(int x, int y) override {
        if (x < 0 || x > 7 || y < 0 || y > 7) {
            return false;
        }
        if (side == 1 && y == this->y - 1 && abs(x - this->x) == 1 && !board[y][x]) {
            return true;
        }
        else if (side == 2 && y == this->y + 1 && abs(x - this->x) == 1 && !board[y][x]) {
            return true;
        }
        else if (abs(y - this->y) == 2 && abs(x - this->x) == 2 && !board[y][x] && board[(y + this->y) / 2][(x + this->x) / 2] != 0 && board[(y + this->y) / 2][(x + this->x) / 2] != board[this->y][this->x]) {
            return true;
        }
        else {
            return false;
        }
    }

    bool continue_attack() override {
        if (is_possible(x + 2, y + 2) || is_possible(x - 2, y - 2)) {
            return true;
        }
        else if (is_possible(x - 2, y + 2) || is_possible(x + 2, y - 2)) {
            return true;
        }
        else {
            return false;
        }
    }

    friend const Checker operator + (const Checker left, const Checker right);
};

const Checker operator + (const Checker left, const Checker rigth) {
    Checker rez;
    rez.x = left.x + rigth.x;
    rez.y = left.y + rigth.y;
    return rez;

}

// ���������� ������� ��� �������� �����
Piece* Pieces[24];
Piece* piece_link = nullptr;

// ����� ������ �� �����������
int Search(Piece* Pieces[24], int x, int y) {
    for (int i = 0; i < LEN; i++) {
        if (Pieces[i]->x == x && Pieces[i]->y == y) {
            return i;
        }
    }
    return -1;
}

// �������� ������ ����� ������
void delete_piece(Piece* pieces[24], int index) {
    LEN--;
    for (int i = index; i < LEN; i++) {
        pieces[i] = pieces[i + 1];
    }
}

int main()
{
    std::cout << "Hello";
    Checker c1, c2;
    c1.x = 1;
    c1.y = 1;
    c2.x = 2;
    c2.y = 2;
    Checker rez = c1 + c2;
    cout << rez.x;
    // �������� ���� ���� � ����������� ������� ������
    RenderWindow window(VideoMode(600, 600), "Checkers", Style::Close);
    window.setFramerateLimit(40);

    // ������ ����
    Image icon;
    icon.loadFromFile("sprites/icon.png");
    window.setIcon(32, 32, icon.getPixelsPtr());

    // �������� ������� �����
    Texture textures;
    textures.loadFromFile("sprites/board.jpg");
    Sprite chess_board(textures);

    // �������� �������� ���������
    Texture Wh_win, Bl_win;
    Wh_win.loadFromFile("sprites/WhiteWin.png");
    Bl_win.loadFromFile("sprites/BlackWin.png");
    Sprite WhiteWin(Wh_win), BlackWin(Bl_win);

    // �������� ����� �� ����
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[j][i]) {
                Pieces[LEN] = new Checker(i, j, board[j][i]);
                LEN++;
            }
        }
    }

    // �� ��� ���, ���� ���� �� �������
    while (window.isOpen())
    {
        Vector2i position = Mouse::getPosition(window);
        Event event;

        while (window.pollEvent(event))
        {
            // �������� ���� �� �������
            if (event.type == Event::Closed) {
                window.close();
            }

            if (!gameover) {
                // ��� ������� �� ����� ������ ����
                if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) {
                    // ���� �������� ������ ���, �� ������������ � �����
                    if (!piece_link) {
                        int index = Search(Pieces, position.x / 75, position.y / 75);
                        if (index != -1) {
                            if (Pieces[index]->side == turn % 2 + 1) {
                                swap(Pieces[index], Pieces[LEN - 1]);
                                piece_link = Pieces[LEN - 1];
                            }
                        }
                    }

                    // ���� � ��������� ������ ���� �������� ������, �� ����������� � �����������
                    if (piece_link) {
                        is_move = true;
                    }

                }
                // ���� �������� ����� ������ ����, ����� ������� �� �������� ������
                else if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && piece_link) {
                    // ���������� �������� ����������� ���� �� ��������� ������
                    if (piece_link->is_possible(position.x / 75, position.y / 75)) {
                        int old_x = piece_link->x, old_y = piece_link->y;
                        bool is_attack = piece_link->move(position.x / 75, position.y / 75);

                        if (attack_turn && !is_attack) {
                            piece_link->move(old_x, old_y);
                        }
                        else if (attack_turn && is_attack && piece_link->continue_attack()) {
                            attack_turn = true;
                        }
                        else {
                            attack_turn = false;
                            turn++;
                        }
                    }
                    // ���� �� ��������� ������ ������ ������, �� ������ ������������ �� �����
                    else {
                        piece_link->SetPosition(piece_link->x, piece_link->y);
                    }

                    is_move = false;
                    piece_link = nullptr;
                }
            }
        }

        // �������� �� ������� ���������� ������� ������
        if (is_move) {
            piece_link->image.setPosition(position.x, position.y);
        }

        // ����������� ������ �� �����
        if (piece_link) {
            if (piece_link->image.getPosition().x < piece_size / 2) {
                piece_link->image.setPosition(piece_size / 2, piece_link->image.getPosition().y);
            }
            if (piece_link->image.getPosition().x > 600 - piece_size / 2) {
                piece_link->image.setPosition(600 - piece_size / 2, piece_link->image.getPosition().y);
            }
            if (piece_link->image.getPosition().y < piece_size / 2) {
                piece_link->image.setPosition(piece_link->image.getPosition().x, piece_size / 2);
            }
            if (piece_link->image.getPosition().y > 600 - piece_size / 2) {
                piece_link->image.setPosition(piece_link->image.getPosition().x, 600 - piece_size / 2);
            }
        }

        // �������� ����� ����
        if (turn_block < turn) {
            turn_block++;
            for (int i = 0; i < LEN; i++) {
                if (Pieces[i]->side == turn % 2 + 1) {
                    // �������� ������ �����
                    if (!board[Pieces[i]->y][Pieces[i]->x]) {
                        amount[Pieces[i]->side - 1]--;
                        delete_piece(Pieces, i);
                        continue;
                    }
                    // �������� ����������� ������
                    else if (Pieces[i]->continue_attack()) {
                        attack_turn = true;
                    }
                }
                else {
                    // ����������� � �����
                    if (Pieces[i]->is_change && Pieces[i]->is_necessary) {
                        Pieces[i] = new Queen(Pieces[i]->x, Pieces[i]->y, Pieces[i]->side);
                    }
                }
            }
        }

        // ��������� ����� � �����
        window.clear();
        window.draw(chess_board);

        for (int i = 0; i < LEN; i++) {
            window.draw(Pieces[i]->image);
        }

        // �������� ��������� ����
        if (amount[0] == 0) {
            gameover = true;
            window.draw(BlackWin);
        }
        else if (amount[1] == 0) {
            gameover = true;
            window.draw(WhiteWin);
        }

        window.display();
    }

    return 0;
}