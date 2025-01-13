#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <cstring>
using namespace sf;
using namespace std;

#define WindowHeight 550 // 420    //450 //200          //300//550  //Max is 650
#define WindowWidth 900  // 650    //770  //350           //500//900  //Max is 1115

#define PaddleWidth WindowWidth * 0.012
#define PaddleHeight WindowHeight * 0.15
#define BallRadius WindowWidth * 0.01

int Player1Score = 0;
int Player2Score = 0;

int Paddle1_X = WindowWidth * 0.018;
int Paddle1_Y = WindowHeight * 0.417;
int Paddle2_X = WindowWidth - PaddleWidth - Paddle1_X;
int Paddle2_Y = WindowHeight * 0.416;
int Ball_X = WindowWidth / 2;
int Ball_Y = WindowHeight / 2;

double XVILOCITY = 0.0037 * WindowWidth;    // 0.37% of total width
double YVILOCITY = 0.002618 * WindowHeight; // 0.26% of total hight

double SpeedBoostX = 0.02 * XVILOCITY; // 2% of actual speed
double SpeedBoostY = 0.02 * YVILOCITY;
int hitCount = 0; // How many times Ball has collided with any of the paddles
                  // till no player gets out.

int Paddle1Distance = 0; // Distance from y-coordinate of initial position of paddle
int Paddle2Distance = 0;

bool Paddle1_UP = false;
bool Paddle1_DOWN = false;
bool Paddle2_UP = false;
bool Paddle2_DOWN = false;

bool GameEnded = false;

RenderWindow GameWindow(VideoMode(WindowWidth, WindowHeight), "BroxGaming"); // Gaming window
RectangleShape Paddle1(Vector2f(PaddleWidth, PaddleHeight));                 // Player1's Paddle i.e Red Paddle
RectangleShape Paddle2(Vector2f(PaddleWidth, PaddleHeight));                 // Player2's Paddle i.e Blue Paddle
CircleShape Ball(BallRadius);                                                // Ball
Text Score1, Score2, GameResult;

void *Paddle1Movement(void *params)
{
    RectangleShape *Paddle1 = reinterpret_cast<RectangleShape *>(params);
    Paddle1->setFillColor(Color(255, 0, 0, 60)); // < 20% opacity
    Paddle1->setOutlineColor(Color::Red);
    Paddle1->setOutlineThickness(2);
    Paddle1->setPosition(Paddle1_X, Paddle1_Y);
    while (!GameEnded)
    {
        if (Paddle1_UP && Paddle1->getPosition().y > 3)
        {
            Paddle1->move(0, -WindowHeight * 0.005454);
            Paddle1Distance -= WindowHeight * 0.005454;
        }
        if (Paddle1_DOWN && Paddle1->getPosition().y + Paddle1->getSize().y < WindowHeight - 3)
        {
            Paddle1->move(0, WindowHeight * 0.005454);
            Paddle1Distance += WindowHeight * 0.005454;
        }
        usleep(5000);
    }
    return NULL;
}
void *Paddle2Movement(void *params)
{
    RectangleShape *Paddle2 = reinterpret_cast<RectangleShape *>(params);
    Paddle2->setFillColor(Color(0, 0, 255, 60)); // < 20% opacity
    Paddle2->setOutlineColor(Color::Blue);
    Paddle2->setOutlineThickness(2);
    Paddle2->setPosition(Paddle2_X, Paddle2_Y);
    while (!GameEnded)
    {
        if (Paddle2_UP && Paddle2->getPosition().y > 3)
        {
            Paddle2->move(0, -WindowHeight * 0.005454);
            Paddle2Distance -= WindowHeight * 0.005454;
        }
        if (Paddle2_DOWN && Paddle2->getPosition().y + Paddle2->getSize().y < WindowHeight - 3)
        {
            Paddle2->move(0, WindowHeight * 0.005454);
            Paddle2Distance += WindowHeight * 0.005454;
        }
        usleep(5000);
    }
    return NULL;
}
void *BallMovement(void *params)
{
    double xVil = 0.00183 * WindowWidth;
    double yVil = 0.001182 * WindowHeight;
    Ball.setFillColor(Color(255, 255, 0, 200));
    Ball.setPosition(Ball_X, Ball_Y);

    while (!GameEnded)
    {
        Ball.move(xVil, yVil);

        // Collisions with top and bottom of GameWindow
        if (Ball.getPosition().y <= 0)
            yVil *= -1;
        else if (Ball.getPosition().y + Ball.getRadius() * 2 >= WindowHeight)
            yVil *= -1;

        // Collisions with Paddles
        if (Paddle1.getGlobalBounds().intersects(Ball.getGlobalBounds()))
        {
            if (yVil >= 0 && (Ball.getPosition().y >= Paddle1_Y + Paddle1Distance && Ball.getPosition().y < Paddle1_Y + Paddle1Distance + (2 * (PaddleHeight) / 5)))
            {
                xVil = XVILOCITY + hitCount * SpeedBoostX;
                yVil = -YVILOCITY - hitCount * SpeedBoostY;
            }
            else if (yVil <= 0 && (Ball.getPosition().y <= Paddle1_Y + Paddle1Distance + PaddleHeight && Ball.getPosition().y > Paddle1_Y + Paddle1Distance + (3 * (PaddleHeight) / 5)))
            {
                xVil = XVILOCITY + hitCount * SpeedBoostX;
                yVil = YVILOCITY + hitCount * SpeedBoostY;
            }
            else if (Ball.getPosition().y >= Paddle1_Y + Paddle1Distance + (2 * (PaddleHeight) / 5) && Ball.getPosition().y <= Paddle1_Y + Paddle1Distance + (3 * (PaddleHeight) / 5))
            {
                xVil = XVILOCITY + hitCount * SpeedBoostX;
                yVil = 0;
            }
            else
            {
                xVil = XVILOCITY + hitCount * SpeedBoostX;
                if (yVil < 0)
                    yVil = -YVILOCITY - hitCount * SpeedBoostY;
                else
                    yVil = YVILOCITY + hitCount * SpeedBoostX;
            }
            hitCount++;
        }
        else if (Paddle2.getGlobalBounds().intersects(Ball.getGlobalBounds()))
        {
            if (yVil >= 0 && (Ball.getPosition().y >= Paddle2_Y + Paddle2Distance && Ball.getPosition().y <= Paddle2_Y + Paddle2Distance + (2 * (PaddleHeight) / 5)))
            {
                xVil = -XVILOCITY - hitCount * SpeedBoostX;
                yVil = -YVILOCITY - hitCount * SpeedBoostY;
            }
            else if (yVil <= 0 && (Ball.getPosition().y <= Paddle2_Y + Paddle2Distance + PaddleHeight && Ball.getPosition().y >= Paddle2_Y + Paddle2Distance + (3 * (PaddleHeight) / 5)))
            {
                xVil = -XVILOCITY - hitCount * SpeedBoostX;
                yVil = YVILOCITY + hitCount * SpeedBoostY;
            }
            else if (Ball.getPosition().y >= Paddle2_Y + Paddle2Distance + (2 * (PaddleHeight) / 5) && Ball.getPosition().y <= Paddle1_Y + Paddle1Distance + (3 * (PaddleHeight) / 5))
            {
                xVil = -XVILOCITY - hitCount * SpeedBoostX;
                yVil = 0;
            }
            else
            {
                xVil = -XVILOCITY - hitCount * SpeedBoostX;
                if (yVil < 0)
                    yVil = -YVILOCITY - hitCount * SpeedBoostY;
                else
                    yVil = YVILOCITY + hitCount * SpeedBoostY;
            }
            hitCount++;
        }

        // Collison for right or left walls
        if (Ball.getPosition().x <= 0)
        {
            usleep(50000);
            xVil *= -1;
            Player2Score++;
            Score2.setString(to_string(Player2Score));

            // Reset ball and paddle positions
            Ball.setPosition(Ball_X, Ball_Y);
            Paddle1.setPosition(Paddle1_X, Paddle1_Y);
            Paddle2.setPosition(Paddle2_X, Paddle2_Y);
            Paddle1Distance = 0;
            Paddle2Distance = 0;
            xVil = 0.00183 * WindowWidth;
            if (yVil < 0)
                yVil = 0.001182 * WindowHeight;
            else
                yVil = -0.001182 * WindowHeight;
            ;
            hitCount = 0;

            if (Player2Score == 10)
            {
                Score2.setPosition((WindowWidth / 2) + 0.067 * WindowWidth, 25);
                sleep(1);
                GameEnded = true;
                GameResult.setFillColor(Color(0, 0, 255, 90));
                GameResult.setOutlineColor(Color::Blue);
                GameResult.setString("Player 2 Wins!");
                return NULL;
            }
            usleep(200000);
        }
        else if (Ball.getPosition().x + Ball.getRadius() * 2 >= WindowWidth)
        {
            usleep(50000);
            xVil *= -1;
            Player1Score++;
            Score1.setString(to_string(Player1Score));

            // Reset ball and paddle positions
            Ball.setPosition(Ball_X, Ball_Y);
            Paddle1.setPosition(Paddle1_X, Paddle1_Y);
            Paddle2.setPosition(Paddle2_X, Paddle2_Y);
            Paddle1Distance = 0;
            Paddle2Distance = 0;
            xVil = -0.00183 * WindowWidth;
            if (yVil < 0)
                yVil = 0.001182 * WindowHeight;
            else
                yVil = -0.001182 * WindowHeight;
            hitCount = 0;

            if (Player1Score == 10)
            {
                Score1.setPosition((WindowWidth / 2) - 0.123 * WindowWidth, 25);
                sleep(1);
                GameEnded = true;
                GameResult.setFillColor(Color(255, 0, 0, 90));
                GameResult.setOutlineColor(Color::Red);
                GameResult.setString("Player 1 Wins!");
                return NULL;
            }
            usleep(200000);
        }
        usleep(5000);
    }
    return NULL;
}

int main()
{
    GameWindow.setFramerateLimit(60);

    Texture backgroundTexture;
    string imagePath = "game_bg.png";
    if (!backgroundTexture.loadFromFile(imagePath))
    {
        cerr << "Error loading background image!" << std::endl;
        return 1;
    }
    Sprite BackGround;
    BackGround.setTexture(backgroundTexture);

    // Scalling of backgrounf
    Vector2f scale(
        static_cast<float>(GameWindow.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(GameWindow.getSize().y) / backgroundTexture.getSize().y);
    BackGround.setScale(scale);
    BackGround.setColor(Color(255, 255, 255, 150)); // 50% opacity

    Font font;
    if (!font.loadFromFile("Watanabe.ttf"))
    {
        cerr << "Error! Cany load Font\n";
        return -1;
    }
    Score1.setFont(font);
    Score1.setCharacterSize(0.11 * WindowHeight);
    Score1.setFillColor(Color(255, 0, 0, 110)); // < 50% opacity
    Score1.setOutlineColor(Color::Red);
    Score1.setOutlineThickness(0.0017 * WindowWidth);
    Score1.setPosition((WindowWidth / 2) - 0.11112 * WindowWidth, 25);
    Score1.setString("0");

    Score2.setFont(font);
    Score2.setCharacterSize(0.11 * WindowHeight);
    Score2.setFillColor(Color(0, 0, 255, 110)); // < 50% opacity
    Score2.setOutlineColor(Color::Blue);
    Score2.setOutlineThickness(0.0017 * WindowWidth);
    Score2.setPosition((WindowWidth / 2) + 0.077778 * WindowWidth, 25);
    Score2.setString("0");

    GameResult.setFont(font);
    GameResult.setCharacterSize(.062 * WindowWidth);
    GameResult.setOutlineThickness(0.0036 * WindowHeight);
    GameResult.setPosition(0.2834 * WindowWidth, 0.427 * WindowHeight);

    RectangleShape BlurOverlay(Vector2f(WindowWidth / 2, WindowHeight / 2));
    BlurOverlay.setFillColor(Color(0, 0, 0, 200)); // Adjust alpha value for the desired level of opacity
    BlurOverlay.setPosition(WindowWidth / 4, WindowHeight / 4);

    pthread_t TID[4];

    pthread_create(&TID[0], NULL, &Paddle1Movement, &Paddle1);
    pthread_create(&TID[1], NULL, &Paddle2Movement, &Paddle2);
    pthread_create(&TID[2], NULL, &BallMovement, NULL);

    while (GameWindow.isOpen())
    {
        while (!GameEnded)
        {
            Event KeyPressingEvents;
            while (GameWindow.pollEvent(KeyPressingEvents))
            {
                // In case if escape key is pressed OR gaming window is being closed by pressing CROSS
                if (KeyPressingEvents.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    GameWindow.close();
                }
                // Checking if any of the player moving paddle or not.
                if (KeyPressingEvents.type == Event::KeyPressed)
                {
                    if (KeyPressingEvents.key.code == Keyboard::W)
                        Paddle1_UP = true;

                    if (KeyPressingEvents.key.code == Keyboard::S)
                        Paddle1_DOWN = true;

                    if (KeyPressingEvents.key.code == Keyboard::Up)
                        Paddle2_UP = true;

                    if (KeyPressingEvents.key.code == Keyboard::Down)
                        Paddle2_DOWN = true;

                    if (KeyPressingEvents.key.code == Keyboard::P)
                    {
                    }
                }
                if (KeyPressingEvents.type == Event::KeyReleased)
                {
                    if (KeyPressingEvents.key.code == Keyboard::W)
                        Paddle1_UP = false;

                    if (KeyPressingEvents.key.code == Keyboard::S)
                        Paddle1_DOWN = false;

                    if (KeyPressingEvents.key.code == Keyboard::Up)
                        Paddle2_UP = false;

                    if (KeyPressingEvents.key.code == Keyboard::Down)
                        Paddle2_DOWN = false;
                }
            }
            GameWindow.clear();
            GameWindow.draw(BackGround);
            GameWindow.draw(Paddle1);
            GameWindow.draw(Paddle2);
            GameWindow.draw(Ball);
            GameWindow.draw(Score1);
            GameWindow.draw(Score2);
            GameWindow.display();
            usleep(5000);
        }
        usleep(10000);
        GameWindow.clear();
        GameWindow.draw(BackGround);
        GameWindow.draw(BlurOverlay);
        GameWindow.draw(GameResult);
        GameWindow.display();

        sleep(5);
        GameWindow.close();
    }

    return 0;
}