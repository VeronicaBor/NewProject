#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <map>
#include <iostream>
#include <math.h>
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
    point() {
        x = 0;
        y = 0;
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



class Entity {
public:
    float x, y;
    int maxHP;
    int HP;
    float damage;
    float w, h, dx, dy, speed, MoveTimer;
    bool life, isMove, damaged;

    String file;
    Image image;
    Texture texture;
    Sprite sprite;

    Entity(String text, float X, float Y, int W, int H) {
        x = X; y = Y; h = H; w = W; MoveTimer = 0;
        speed = 0; maxHP = 16; dx = 0; dy = 0;
        HP = 16;
        life = true; isMove = false; damaged = false;
        image.loadFromFile("character/" + text);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
    }
};

class Player :public Entity {
public:
    enum { walk, attack, damaged1 } state;
    float Frame = 0;
    int floversPosition[30] = {};
    int dir = 0;

    //Конструктор
    Player(String text, float X, float Y, int W, int H) :Entity(text, X, Y, W, H) {

        floversPosition[0] = 1;
        state = walk;
        sprite.setTextureRect(IntRect(0, 2, w, h));
    }

    void WhatToDO(float time) {
        switch (state) {
        case walk:
            Muving(time);
            if (Keyboard::isKeyPressed(Keyboard::E)) {
                state = attack;
            }
            break;
        case attack:
            Frame += time;
            speed = 0;
            attacking(time);
            if (Frame > 800) {
                std::cout << "attack" << std::endl;
                state = walk;
                Frame = 0;
                sprite.setColor(Color::White);
            }
            break;
        case damaged1:
            Frame += time;
            if (Frame > 400) {
                sprite.setColor(Color::White);
                Frame = 0;
                damaged = false;
                state = walk;
            }
            break;

        }

    }

    void Muving(float& time) {
        float speed1 = 0.3;
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            dir = 1; speed = speed1;
            Frame += 0.005 * time;
            if (Frame > 3) Frame -= 3;
            sprite.setTextureRect(IntRect(42 * int(Frame), 62.5, 42, 62.5));
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            dir = 0; speed = speed1;
            Frame += 0.005 * time;
            if (Frame > 3) Frame -= 3;
            sprite.setTextureRect(IntRect(42 * int(Frame), 125, 42, 62.5));
        }

        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            dir = 3; speed = speed1;
            Frame += 0.005 * time;
            if (Frame > 3) Frame -= 3;
            sprite.setTextureRect(IntRect(42 * int(Frame), 187.5 + 3, 42, 62.5));

        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            dir = 2; speed = speed1;
            Frame += 0.005 * time;
            if (Frame > 3) Frame -= 3;
            sprite.setTextureRect(IntRect(42 * int(Frame), 2, 42, 62.5));
        }

        update(time);
    }

    void update(float time)
    {
        switch (dir)
        {
        case 0: dx = speed; dy = speed / 2; break;
        case 1: dx = -speed; dy = -speed / 2; break;
        case 2: dx = -speed; dy = speed / 2; break;
        case 3: dx = speed; dy = -speed / 2; break;
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
        point twoD_vector = Isoto2D(dx, dy);



        for (int i = (twoD.y + h) / 50; i < (twoD.y + h) / 50; i++)
            for (int j = (twoD.x + w) / 50; j < (twoD.x + w) / 50; j++) {
                if (TileMap[j][i] == 'W' || TileMap[j][i] == 'H' || TileMap[j][i] == 'B' || TileMap[j][i] == 'd' || TileMap[j][i] == 'b')
                {
                    if (twoD_vector.y < 0)//если мы шли вниз,
                    {
                        twoD.y = i * 50;
                    }
                    if (twoD_vector.y > 0)
                    {

                        twoD.y = i * 50 - 50 - 15;
                    }
                    if (twoD_vector.x > 0)
                    {

                        twoD.x = j * 50 - 50;
                    }
                    if (twoD_vector.x < 0)
                    {

                        twoD.x = j * 50 + 15;
                    }

                }

                if (TileMap[j][i] == 'f') {

                    if (Keyboard::isKeyPressed(Keyboard::R)) {

                        InteractionWithFlowers(j, i);

                    }
                }
            }
        point iso = twoDtoIso(twoD);
        x = iso.x;
        y = iso.y;
    }

    void takeDamage(const int& damage) {
        if (HP - damage > 0) {
            HP -= damage;
            sprite.setColor(Color::Red);
            damaged = true;
            state = damaged1;
        }
        else HP = 0;
    }
    void healing(const int& heal) {
        if (maxHP < heal + HP) HP = maxHP;
        else HP += heal;
    }

    void increaseDamage(const int& damageBuf) {
        damage += damageBuf;
    }
    void DecreaseDamage(const int& demageDeBuf) {
        damage -= demageDeBuf;
    }

    void attacking(float time) {
        sprite.setColor(Color::Blue);

    }

    void InteractionWithFlowers(int i, int j) {
        for (int i = 0; i < 15; i++) {
            if (floversPosition[2 * i] != 0 && floversPosition[2 * i + 1] < 5) {
                floversPosition[2 * i + 1]++;
                std::cout << floversPosition[2 * i + 1] << std::endl;
            }
            if (floversPosition[2 * i + 1] == 4) floversPosition[2 * (i + 1)] = 1;
            if (floversPosition[2 * i] == 0) break;
        }
        TileMap[i][j] = '.';
    }

};

class Enemy :public Entity {
public:
    enum { sit, walk, attack, agressive, damaged1 } state;
    float OrkFrame = 0;
    bool atttacks = false;
    bool InRage = false;
    int x1, y1;
    Enemy(String text, float X, float Y, int W, int H) :Entity(text, X, Y, W, H) {
        state = sit;
        damage = 2;
        sprite.setTextureRect(IntRect(25, 565, 65, 78));
        sprite.setPosition(X, Y);
        sprite.setOrigin(w / 2, h / 2);
        speed = 0.05;
    }

    void WhatToDo(Player& p, float time) {
        point twoD(x, y);
        switch (state) {
        case sit:
            siting();

            OrkFrame += time;

            if (OrkFrame > 4000) {
                x1 = (rand() % 19) + 20;
                y1 = rand() % 36 + 2;
                std::cout << x1 << " " << y1 << std::endl;
                state = walk;
                OrkFrame = 0;

            }
            break;
        case walk: WalkTo(x1, y1, time);
            if (abs(x - p.x) < 100 && abs(y - p.y) < 100) {
                if ((p.x + 2 * p.y) / 100 > 20)
                    state = agressive;
            }
            break;
        case agressive: GoToAttack(time, p.x, p.y);
            sprite.setColor(Color::White);
            if (abs(x - p.x) < 20 || abs(y - p.y) < 20) {
                state = attack;
            }
            if (abs(x - p.x) > 100 && abs(y - p.y) > 100) {
                state = walk;
                break;
            }

            break;
        case attack: attacking(p, time);
            sprite.setColor(Color::Blue);
            if (abs(x - p.x) > 20 && abs(y - p.y) > 20) {
                state = agressive;
            }
            break;
        case damaged1:
            OrkFrame += time;
            if (OrkFrame > 400) {
                sprite.setColor(Color::White);
                OrkFrame = 0;
                damaged = false;
                state = walk;
            }
            break;
        }
    }

    void siting() {
        sprite.setTextureRect(IntRect(762, 282, 62, 48));
    }

    void attacking(Player& p, float time) {
        OrkFrame += time;
        int damageIm = 0;
        if (OrkFrame > 800) damageIm = 1;
        if (dx > 0 && dy > 0) {
            sprite.setTextureRect(IntRect(345 + 105 * int(damageIm), 565, 75, 80));
        }
        if (dx < 0 && dy > 0) {
            sprite.setTextureRect(IntRect(345 + 90 * int(damageIm), 775, 70, 80));
        }
        if (dx > 0 && dy < 0) {
            sprite.setTextureRect(IntRect(345 + 105 * int(damageIm), 365, 75, 70));
        }
        if (dx < 0 && dy < 0) {
            sprite.setTextureRect(IntRect(350 + 90 * int(damageIm), 155, 70, 70));
        }
        if (OrkFrame > 1000) {
            p.takeDamage(damage);
            OrkFrame = 0;
        }
    }

    void WalkTo(int x1, int y1, const float& time) {

        point in = Isoto2D(x, y);
        point vec(-in.x + x1 * 50, -in.y + y1 * 50);
        float xl = vec.x; float yl = vec.y;
        if (abs(xl) > 1) {
            walking('y', yl, time);
        }
        if (abs(yl) < 1)
            walking('x', xl, time);
        if (abs(xl) < 1)
            walking('y', yl, time);

        if (abs(vec.x) < 20 && abs(vec.y) < 20) state = sit;
    }

    void walking(char duration, float& a, float time) {

        if (duration == 'x') {
            if (a > 0) {
                dx = speed;
                dy = speed / 2;

            }
            if (a < 0) {
                dx = -speed;
                dy = -speed / 2;
            }
        }
        if (duration == 'y') {

            if (a > 0) {
                dy = speed / 2;
                dx = -speed;
            }

            if (a < 0) {
                dy = -speed / 2;
                dx = speed;
            }
        }
        x += dx * time;
        y += dy * time;
        muveSet(time);

        interactionWithMap();
        sprite.setPosition(x, y);

    }

    void interactionWithMap()//ф-ция взаимодействия с картой
    {
        point twoD = Isoto2D(x, y);
        point twoD_vector = Isoto2D(dx, dy);
        for (int i = (twoD.y + h) / 50; i < (twoD.y + h) / 50; i++)
            for (int j = (twoD.x + w) / 50; j < (twoD.x + w) / 50; j++) {
                if (TileMap[j][i] == 'W' || TileMap[j][i] == 'H' || TileMap[j][i] == 'B' || TileMap[j][i] == 'd' || TileMap[j][i] == 'b')
                {
                    if (twoD_vector.y < 0)//если мы шли вниз,
                    {
                        twoD.y = i * 50;
                    }
                    if (twoD_vector.y > 0)
                    {

                        twoD.y = i * 50 - 50 - 105;
                    }
                    if (twoD_vector.x > 0)
                    {

                        twoD.x = j * 50 - 100;
                    }
                    if (twoD_vector.x < 0)
                    {

                        twoD.x = j * 50 + 105;
                    }

                }
            }
        point iso = twoDtoIso(twoD);
        x = iso.x;
        y = iso.y;
    }

    void GoToAttack(float time, float x1, float y1) {
        point in = Isoto2D(x, y);
        point en = Isoto2D(x1, y1);
        point vec((-in.x + en.x) / 50, (-in.y + en.y) / 50);

        if (abs(vec.x) > 1) walking('y', vec.y, time);
        if (abs(vec.x) < 1) walking('y', vec.y, time);
        if (abs(vec.y) < 1) walking('x', vec.x, time);

    }

    void muveSet(float time) {
        OrkFrame += time / 400;
        if (dx > 0 && dy > 0) {
            if (OrkFrame > 4) OrkFrame = 0;
            sprite.setTextureRect(IntRect(25 + 105 * int(OrkFrame), 565, 65, 78));
        }
        if (dx < 0 && dy > 0) {
            if (OrkFrame > 4) OrkFrame = 0;
            sprite.setTextureRect(IntRect(25 + 105 * int(OrkFrame), 772, 65, 78));
        }
        if (dx > 0 && dy < 0) {
            if (OrkFrame > 4) OrkFrame = 0;
            sprite.setTextureRect(IntRect(25 + 105 * int(OrkFrame), 360, 65, 78));
        }
        if (dx < 0 && dy < 0) {
            if (OrkFrame > 4) OrkFrame = 0;
            sprite.setTextureRect(IntRect(25 + 105 * int(OrkFrame), 155, 65, 78));
        }
    }

    void TakeDamage(int damage) {
        if (HP - damage > 0) {
            HP -= damage;
            sprite.setColor(Color::Red);
            damaged = true;
            state = damaged1;
        }
        else HP = 0;
    }

};




class flower {
public:
    float x, y;
    float w, h;
    String name;

    flower(String name1, float x1, float y1) {
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

class UI {
public:
    Image image;
    Texture texture;
    Sprite sprite;

    point Health;
    point Controler;
    point potion;

    UI(String Path) {
        image.loadFromFile(Path);
        image.createMaskFromColor(Color(198, 198, 198));
        texture.loadFromImage(image);
        sprite.setTexture(texture);
    }

    void GetCordinate(float x, float y) {
        Health.x = x - 475, Health.y = y + 410;
        Controler.x = x + 270, Controler.y = 230 + y;
        potion.x = x + 320, potion.y = y - 430;
    }

    void Get_Texture(const char& key) {
        if (key == 'C') {
            sprite.setTextureRect(IntRect(0, 0, 230, 230));
            sprite.setPosition(Controler.x, Controler.y);
        }
        if (key == 'F') sprite.setTextureRect(IntRect(17, 252, 37, 32));
        if (key == 'H') sprite.setTextureRect(IntRect(17, 289, 37, 32));
        if (key == 'E') sprite.setTextureRect(IntRect(17, 325, 37, 32));

        if (key == 'P') {
            sprite.setTextureRect(IntRect(0, 370, 172, 45));
            sprite.setPosition(potion.x, potion.y);
        }
    }
    String ForDrawUI(int HP, const int& maxHP) {
        String s = "CP";
        for (int i = 0; i < maxHP / 2; i++) {
            if (HP > 1) {
                HP -= 2;
                s += 'F';
            }
            else if (HP == 1) {
                HP -= 10;
                s += 'H';
            }
            else s += 'E';
        }
        return s;
    }
};

class Inventary {
public:
    float x;
    float y;
    Texture texture;
    Sprite sprite;

    float w, h;

    point FlowerInventary;
    short flowerIn[2] = { 5, 3 };

    point PotionInventary;
    short PotionIn[2] = { 2, 2 };

    point ActivePotion[3];
    point Healmet;
    point Armor;
    point Pants;
    point Socks;
    point Sword;

    point rings;
    short NumOfRings;

    point Crafting;
    short Craft[2] = { 2, 2 };

    point AfterCraftPotions;

    Inventary(String path) {

        texture.loadFromFile(path);
        sprite.setTexture(texture);
        x = 229; y = 69.5;
        sprite.setPosition(x, y);
        w = 36; h = 36;
        NumOfRings = 3;
    }

    void CorrectInventary(const float& dx, const float& dy) {
        x = -229 + dx; y = -269.5 + dy;
        sprite.setPosition(x, y);

        FlowerInventary.x = 14 + x, FlowerInventary.y = 277 + y;
        PotionInventary.x = 237 + x, PotionInventary.y = 277 + y;

        Healmet.x = 14 + x, Healmet.y = 13 + y;
        Armor.x = 14 + x, Armor.y = 49 + y;
        Pants.x = 14 + x, Pants.y = 85 + y;
        Socks.x = 14 + x; Socks.y = 121 + y;

        Sword.x = 73 + x, Sword.y = 13 + y;
        rings.x = 73 + x, rings.y = 49 + y;
        NumOfRings = 3;

        ActivePotion[0].x = 194 + x, ActivePotion[0].y = 41 + y;
        ActivePotion[1].x = 194 + x, ActivePotion[1].y = 89 + y;
        ActivePotion[2].x = 194 + x, ActivePotion[2].y = 138 + y;

        Crafting.x = 25 + x, Crafting.y = 182 + y;
        AfterCraftPotions.x = 137 + x, AfterCraftPotions.y = 197 + y;
    }

    Sprite drawFlowerInPosition(int i, int id, Flowers& flow) {
        flow.GetRect(id);
        int posY = 0;
        if (i > 9) {
            posY++; i -= 10;
            posY++;
        }
        else if (i > 4) {
            posY++; i -= 5;
        }
        flow.sprite.setPosition(FlowerInventary.x + i * w - 5, FlowerInventary.y + posY * h + 2);
        return flow.sprite;
    }
};

int main()
{
    RenderWindow window(sf::VideoMode(810, 540), "SFML works!");

    Clock clock;
    point isoMain(405, 270);

    float speed = 0.3;



    Player p("ch.png", 405, 270, 42, 62.5);

    Enemy ork1("ork.png", 1000, 1000, 65, 78);




    Flowers firts_loc({ "желтый", 287, 220 },
        { "красный", 287 + 51, 220 },
        { "зеленый", 287 + 51 * 2, 220 },
        { "синий", 287 + 51 * 3, 220 },
        { "розовый", 287 + 51 * 4, 220 },
        { "белый", 287 + 51 * 5, 220 });

    field water("water.png");

    field grass("grass.png");
    field roadEast("roadEast.png");
    field roadNorth("roadNorth.png");
    field roadCornerWS("roadCornerWS.png");
    field roadCornerNW("roadCornerNW.png");
    field roadCornerES("roadCornerES.png");
    field roadCornerNE("roadCornerNE.png");
    field lot("lot.png");

    Assets house("house.png", 22, 90);
    Assets wal0001("wal0001.png", -11, 65);
    Assets watchtower("watchtower.png", 14, 129);

    Inventary inventary_menu("assets/Inventory.png");
    UI ui("assets/UI.png");


    float flower_growing_timer = 0;
    bool OpenInventary = true;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();
        Vector2f a = view.getCenter();


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)//событие нажатия клавиши
                if ((event.key.code == Keyboard::M)) {//если клавиша ТАБ

                    switch (OpenInventary) {//переключатель, реагирующий на логическую переменную showMissionText

                    case true: {
                        std::cout << "Inventary is open" << std::endl;
                        OpenInventary = false;

                        break;//выходим , чтобы не выполнить условие "false" (которое ниже)
                    }
                    case false: {
                        std::cout << "Inventary is close" << std::endl;

                        OpenInventary = true;// а эта строка позволяет снова нажать клавишу таб и получить вывод на экран
                        break;
                    }
                    }
                }
        }

        if (!OpenInventary) time = 0;

        flower_growing_timer += 0.005 * time;



        p.WhatToDO(time);
        ork1.WhatToDo(p, time);


        getplayercoordinateforview(p.x, p.y);
        window.setView(view);//"оживляем" камеру в окне sfml
        window.clear();
        Sprite DrawingPicture;
        int random = 0;

        //Рисуем Карту
        for (int i = 0; i < HEIGHT_MAP; i++)
        {
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                point iso = twoDtoIso(50 * i, 50 * j);
                if (TileMap[i][j] == 'W' || TileMap[i][j] == 'B' || TileMap[i][j] == 'b') DrawingPicture = water.print(iso.x, iso.y);

                else if (TileMap[i][j] == '.' || TileMap[i][j] == 'f') DrawingPicture = grass.print(iso.x, iso.y);
                else if (TileMap[i][j] == 'H' || TileMap[i][j] == 'd') DrawingPicture = grass.print(iso.x, iso.y);

                else if (TileMap[i][j] == '1') DrawingPicture = roadEast.print(iso.x, iso.y);;
                if (TileMap[i][j] == '2') DrawingPicture = roadNorth.print(iso.x, iso.y);
                else if (TileMap[i][j] == '3') DrawingPicture = roadCornerWS.print(iso.x, iso.y);
                else if (TileMap[i][j] == '4') DrawingPicture = roadCornerNW.print(iso.x, iso.y);
                if (TileMap[i][j] == '5') DrawingPicture = roadCornerES.print(iso.x, iso.y);
                else if (TileMap[i][j] == '6') DrawingPicture = roadCornerNE.print(iso.x, iso.y);
                else if (TileMap[i][j] == 'B') DrawingPicture = wal0001.print(iso.x, iso.y);
                if (TileMap[i][j] == 'b') DrawingPicture = watchtower.print(iso.x, iso.y);
                else if (TileMap[i][j] == 'R') DrawingPicture = lot.print(iso.x, iso.y);


                window.draw(DrawingPicture);
                if (TileMap[i][j] == 'H') {
                    DrawingPicture = house.print(iso.x, iso.y);
                    window.draw(DrawingPicture);
                }
                else if (TileMap[i][j] == 'f') {

                    firts_loc.GetRect(random);
                    DrawingPicture = firts_loc.sprite;
                    DrawingPicture.setPosition(iso.x + 25, iso.y + 10);
                    window.draw(DrawingPicture);
                    random++;
                    if (random == 6) random = 0;
                }
            }


        }

        //Рисуем UI
        window.draw(ork1.sprite);
        window.draw(p.sprite);

        ui.GetCordinate(a.x, a.y);
        String ForDrawUI = ui.ForDrawUI(p.HP, p.maxHP);
        for (int i = 0; i < ForDrawUI.getSize(); i++) {
            ui.Get_Texture(ForDrawUI[i]);
            if (ForDrawUI[i] == 'F' || ForDrawUI[i] == 'H' || ForDrawUI[i] == 'E') {
                ui.sprite.setPosition(ui.Health.x + 37 * (i - 2) + 10, ui.Health.y);
            }
            window.draw(ui.sprite);
        }


        if (flower_growing_timer > 30) {
            int xCor = rand() % 26 + 12;
            int Ycor = rand() % 38 + 1;
            TileMap[xCor][Ycor] = 'f';
            flower_growing_timer = 0;
        }




        if (!OpenInventary) {
            inventary_menu.CorrectInventary(a.x, a.y);

            window.draw(inventary_menu.sprite);
            for (int i = 0; i < 15; i++) {
                if (p.floversPosition[2 * i + 1] != 0) window.draw(inventary_menu.drawFlowerInPosition(i, (i + 1) % 7, firts_loc));
            }
        }


        window.display();
    }

    return 0;
}
