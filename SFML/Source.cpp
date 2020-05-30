#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;

int size = 75;

int Board [8][8] = {
    1, 0, 1, 0, 1, 0, 1, 0,
    0, 1, 0, 1, 0, 1, 0, 1,
    1, 0, 1, 0, 1, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 2, 0, 2, 0, 2,
    2, 0, 2, 0, 2, 0, 2, 0,
    0, 2, 0, 2, 0, 2, 0, 2
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "Checkers");
    sf::CircleShape shape(100.f);

    Sprite WhiteCheckers[12];
    Sprite BlackCheckers[12];

    Texture t1, t2, t3;
    t1.loadFromFile("sprites/board.jpg");
    t2.loadFromFile("sprites/checkerBlack.png");
    t3.loadFromFile("sprites/checkerWhite.png");
    
    Sprite board(t1);
    
    int b = 0;
    int w = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int n;
            n = Board[j][i];
            if (!n) {
                continue;
            }
            int x = i;
            int y = j;
            if (n == 1) {
                WhiteCheckers[w].setTexture(t3);
                WhiteCheckers[w].setPosition(size*x,size*y);
                w++;
            }
            if (n == 2) {
                BlackCheckers[b].setTexture(t2);
                BlackCheckers[b].setPosition(size * x, size * y);
                b++;
            }
        }
    }
   

    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);
        Event event;
        
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }

            if (event.type == sf::Event::Closed)
                window.close();



            
        }

        window.clear();
        window.draw(board);
        for (int i = 0; i < 12; i++) {
            window.draw(WhiteCheckers[i]);
        } for (int i = 0; i < 12; i++) {
            window.draw(BlackCheckers[i]);
        }
        window.display();
    }

    return 0;
}