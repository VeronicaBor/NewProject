#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include "View.h"
#include "map.h"

using namespace sf;

class field
{
public:
    String file;

    Image image;
    Texture texture;
    Sprite sprite;


    field(String F) {
        file = "rhombus/" + F;
        image.loadFromFile(file);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
    }

    Sprite print(float x, float y) {
        sprite.setPosition(x, y);
        return sprite;
    }
};

class Assets {
public:
    String file;
    float dx, dy;
    Image image;
    Texture texture;
    Sprite sprite;


    Assets(String F, float x, float y) {
        file = "assets/" + F;
        image.loadFromFile(file);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        dx = x;
        dy = y;
    }

    Sprite print(float x, float y) {
        sprite.setPosition(x - dx, y - dy);
        return sprite;
    }
};

class Player {
private: float x, y;
public:
    float w, h, dx, dy, speed;
    int dir = 0;
    String file;
    Image image;
    Texture texture;
    Sprite sprite;
    Player(String F, float X, float Y, float W, float H) {
        dx = 0; dy = 0; speed = 0;

        file = "character/" + F;
        w = W; h = H;
        image.loadFromFile(file);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X; y = Y;
        sprite.setTextureRect(IntRect(0, 2, w, h));
    }
    void update(float time)
    {
        switch (dir)
        {
        case 0: dx = speed; dy = 0; break;
        case 1: dx = -speed; dy = 0; break;
        case 2: dx = 0; dy = speed; break;
        case 3: dx = 0; dy = -speed; break;
        }

        x += dx * time;
        y += dy * time;

        speed = 0;
        sprite.setPosition(x, y);
    }

    float getplayercoordinateX() {	//этим методом будем забирать координату Х	
        return x;
    }
    float getplayercoordinateY() {	//этим методом будем забирать координату Y 	
        return y;
    }

};

struct point {
    float x;
    float y;

    point(float x1, float y1) {
        x = x1;
        y = y1;
    }
    friend point operator +(const point& a, const point& b) {
        point c(a.x + b.x, a.y + b.y);
        return c;
    }

    friend point operator -(const point& a, const point& b) {
        point c(a.x - b.x, a.y - b.y);
        return c;
    }
};
//функция изменения декортавой системы координат на изометрическую
point twoDtoIso(const point& twoDpoint) {
    point isoPoint(0, 0);
    isoPoint.x = twoDpoint.x - twoDpoint.y;
    isoPoint.y = (twoDpoint.x + twoDpoint.y) /2;
    return isoPoint;
}
point twoDtoIso(const float& x, const float& y) {
    point isoPoint(0, 0);
    isoPoint.x = x - y;
    isoPoint.y = (x + y) /2;
    return isoPoint;
}

//функция движения игрового персонажа


int main()
{
    RenderWindow window(sf::VideoMode(810, 540), "SFML works!");

    Clock clock;

    Player p("ch.png", 405, 270, 42, 62.5);


    field water("water.png");

    field grass("grass.png");
    field roadEast("roadEast.png");
    field roadNorth("roadNorth.png");
    field roadCornerWS("roadCornerWS.png");
    field roadCornerNW("roadCornerNW.png");
    field roadCornerES("roadCornerES.png");
    field roadCornerNE("roadCornerNE.png");

    Assets house("house.png", 22, 90);

    float CurrentFrame = 0;
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            p.dir = 1; p.speed = 0.1;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.sprite.setTextureRect(IntRect(42 * int(CurrentFrame), 62.5, 42, 62.5));
            getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            p.dir = 0; p.speed = 0.1;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.sprite.setTextureRect(IntRect(42 * int(CurrentFrame), 125, 42, 62.5));
            getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой
        }

        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            p.dir = 3; p.speed = 0.1;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.sprite.setTextureRect(IntRect(42 * int(CurrentFrame), 187.5+3, 42, 62.5));
            getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой

        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            p.dir = 2; p.speed = 0.1;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.sprite.setTextureRect(IntRect(42 * int(CurrentFrame), 2, 42, 62.5));
            getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());//передаем координаты игрока в функцию управления камерой

        }
        p.update(time);
        window.setView(view);//"оживляем" камеру в окне sfml
        window.clear();
        Sprite DrawingPicture;
        for (int i = 0; i < HEIGHT_MAP; i++)
        {
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                point iso = twoDtoIso(50 * i, 50 * j);
                if (TileMap[i][j] == 'W') DrawingPicture = water.print(iso.x, iso.y);

                if (TileMap[i][j] == '.' || TileMap[i][j] == 'f') DrawingPicture = grass.print(iso.x, iso.y);
                if (TileMap[i][j] == 'H') DrawingPicture = grass.print(iso.x, iso.y);

                if (TileMap[i][j] == '1') DrawingPicture = roadEast.print(iso.x, iso.y);;
                if (TileMap[i][j] == '2') DrawingPicture = roadNorth.print(iso.x, iso.y);
                if (TileMap[i][j] == '3') DrawingPicture = roadCornerWS.print(iso.x, iso.y);
                if (TileMap[i][j] == '4') DrawingPicture = roadCornerNW.print(iso.x, iso.y);
                if (TileMap[i][j] == '5') DrawingPicture = roadCornerES.print(iso.x, iso.y);
                if (TileMap[i][j] == '6') DrawingPicture = roadCornerNE.print(iso.x, iso.y);

                window.draw(DrawingPicture);
                if (TileMap[i][j] == 'H') {
                    DrawingPicture = house.print(iso.x, iso.y);
                    window.draw(DrawingPicture);
                }
            }
        }


        window.draw(p.sprite);
        window.display();
    }

    return 0;
}