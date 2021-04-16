#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <iostream>
#include <string>
#include "View.h"
#include "map.h"

using namespace sf;
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
    isoPoint.y = (twoDpoint.x + twoDpoint.y) / 2;
    return isoPoint;
}
point twoDtoIso(const float& x, const float& y) {
    point isoPoint(0, 0);
    isoPoint.x = x - y;
    isoPoint.y = (x + y) / 2;
    return isoPoint;
}
point Isoto2D(const point& IsoPoint) {
    float  x = (2 * IsoPoint.y + IsoPoint.x) / 2;
    float  y = (2 * IsoPoint.y - IsoPoint.x) / 2;
    point twoD(x, y);
    return twoD;
}
point Isoto2D(const float& IsoX, const float& IsoY) {
    float x = (2 * IsoY + IsoX) / 2;
    float y = (2 * IsoY - IsoX) / 2;
    point twoD(x, y);
    return twoD;
}

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
        interactionWithMap();//вызываем функцию, отвечающую за взаимодействие с картой
    }

    

    void interactionWithMap()//ф-ция взаимодействия с картой
    {
        point twoD = Isoto2D(x, y);
        for (int i = twoD.y / 50; i < (twoD.y + h) / 50; i++)
            for (int j = twoD.x / 50; j < (twoD.x + w) / 50; j++){
                if (TileMap[j][i] == 'W' || TileMap[j][i] == 'H')
                {
                    if (dy > 0)//если мы шли вниз,
                    {
                        twoD.y = i * 50 - h;
                    }
                    if (dy < 0)
                    {
                        twoD.y = i * 50 + 50;
                    }
                    if (dx > 0)
                    {
                        twoD.x = j * 50 - w;
                    }
                    if (dx < 0)
                    {
                        twoD.x = j * 50 + 50;
                    }

                }

                if (TileMap[j][i] == 'f') { 

                    TileMap[j][i] = ' ';
                }
            }
        point iso = twoDtoIso(twoD);
        x = iso.x;
        y = iso.y;
    }

    float getplayercoordinateX() {//этим методом будем забирать координату Х	
        return x;
    }
    float getplayercoordinateY() {	//этим методом будем забирать координату Y 	
        return y;
    }
    
};

class flower {
public:
    float x, y;
    float w, h;
    String name;

    flower(String name1 ,float x1, float y1) {
        w = 51;
        h = 20;
        x = x1;
        y = y1;
        name = name1;
    }
};

class Flowers {
public:
    String File;

    Image image;
    Texture texture;
    Sprite sprite;

    std::vector<flower> tipes;
    Flowers(flower a, flower b, flower c, flower d, flower e, flower f) {
        File = "assets/flowers.png";
        image.loadFromFile(File);
        texture.loadFromImage(image);
        sprite.setTexture(texture);

        tipes.push_back(a);
        tipes.push_back(b);
        tipes.push_back(c);
        tipes.push_back(d);
        tipes.push_back(e);
        tipes.push_back(f);

    }

    void GetRect(int i) {
        sprite.setTextureRect(IntRect(tipes[i].x, tipes[i].y, tipes[i].w, tipes[i].h));
    }
};



//функция движения игрового персонажа


int main()
{
    RenderWindow window(sf::VideoMode(810, 540), "SFML works!");

    Clock clock;
    point isoMain(405, 270);

    Player p("ch.png", 405, 270, 42, 62.5);

    Flowers firts_loc({ "желтый", 287, 220 },
                      { "красный", 287+51, 220 },
                      { "зеленый", 287 + 51*2, 220 },
                      { "синий", 287 + 51*3, 220 },
                      { "розовый", 287 + 51*4, 220},
                      { "белый", 287 + 51*5, 220 });

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

    char WindowMod = 'G';
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

        if (WindowMod == 'G') {
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                p.dir = 1; p.speed = 0.1;
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                p.sprite.setTextureRect(IntRect(42 * int(CurrentFrame), 62.5, 42, 62.5));
            }

            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                p.dir = 0; p.speed = 0.1;
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                p.sprite.setTextureRect(IntRect(42 * int(CurrentFrame), 125, 42, 62.5));
            }

            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                p.dir = 3; p.speed = 0.1;
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                p.sprite.setTextureRect(IntRect(42 * int(CurrentFrame), 187.5 + 3, 42, 62.5));

            }

            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                p.dir = 2; p.speed = 0.1;
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                p.sprite.setTextureRect(IntRect(42 * int(CurrentFrame), 2, 42, 62.5));

            }
            getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
            p.update(time);
            window.setView(view);//"оживляем" камеру в окне sfml
            window.clear();
            Sprite DrawingPicture;
            int random = 0;
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
                    if (TileMap[i][j] == 'f') {

                        firts_loc.GetRect(random);
                        DrawingPicture = firts_loc.sprite;
                        DrawingPicture.setPosition(iso.x + 25, iso.y + 10);
                        window.draw(DrawingPicture);
                        random++;
                        if (random == 6) random = 0;
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::M)) {
                    WindowMod = 'C';
                }
            }
            window.draw(p.sprite);
        }
        if (WindowMod == 'C') {
            window.clear();
            if (Keyboard::isKeyPressed(Keyboard::Escape) || Keyboard::isKeyPressed(Keyboard::M)) {
                WindowMod = 'G';
            }

        }
        window.display();
    }

    return 0;
}