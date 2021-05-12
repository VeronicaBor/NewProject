#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <string>
#include <iostream>
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

class Player {
private: 
    float x, y;
    int maxHP;
    int HP;
    float damage, damageRaduase, AttackSpeed;
    

public:
    float w, h, dx, dy, speed;

    int dir = 0;
    String file;
    Image image;
    Texture texture;
    Sprite sprite;
    //Конструктор
    Player(String F, float X, float Y, float W, float H) {
        dx = 0; dy = 0; speed = 0;

        file = "character/" + F;
        w = W; h = H;
        image.loadFromFile(file);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X; y = Y;
        HP = 8;
        maxHP = 8;

        damage = 1;
        damageRaduase = 20;
        AttackSpeed = 0.5;

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
        point twoD_vector = Isoto2D(dx, dy);
        for (int i = twoD.y / 50; i < (twoD.y + w) / 50; i++)
            for (int j = twoD.x / 50; j < (twoD.x + w) / 50; j++){
                if (TileMap[j][i] == 'W' || TileMap[j][i] == 'H' || TileMap[j][i] == 'B' || TileMap[j][i] == 'b')
                {
                    if (twoD_vector.y < 0 && twoD_vector.x < 0)//если мы шли вниз,
                    {
                        twoD.x = j * 50 + w;
                        twoD.y = i * 50 + w;
                    }
                    if (twoD_vector.y < 0 && twoD_vector.x > 0)
                    {
                        twoD.x = j*50 + w;
                        twoD.y = i * 50 + w;
                    }
                    if (twoD_vector.y > 0 && twoD_vector.x < 0)
                    {
                        twoD.y = i * 50 + 50;
                        twoD.x = j * 50 - w;
                    }
                    if (twoD_vector.y > 0 && twoD_vector.x > 0)
                    {
                        twoD.y = i * 50 + 50;
                        twoD.x = j * 50 + 50;
                    }

                }

                if (TileMap[j][i] == 'f') 
                { 
                    if (Keyboard::isKeyPressed(Keyboard::R)) {
                        
                        TileMap[j][i] = '.';

                    }
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
    
    //изменения максимального ХП
    void increaseMaxHP(const int& plus) {
        maxHP += plus;
    }
    void decreaseMaxHP(const int& minus) {
        maxHP -= minus;
    }
    int getMaxHP() { return maxHP; }
    int getHP() { return HP; }

    void takeDamage(const int& damage) {
        if (HP - damage > 0) HP -= damage;
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

    void attack() {

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
        image.createMaskFromColor(Color(198,198,198));
        texture.loadFromImage(image);
        sprite.setTexture(texture);
    }

    void GetCordinate(float x, float y) {
        Health.x = x + 20, Health.y = 520 + y;
        Controler.x = x + 580, Controler.y = 310 + y;
        potion.x = x + 640, potion.y = y + 20;
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
};

class Inventary {
public:
    float x;
    float y;
    Texture texture;
    Sprite sprite;

    float w, h;

    point FlowerInventary;
    short flowerIn[2] = { 5, 3};

    point PotionInventary;
    short PotionIn[2] = { 2, 2};
    
    point ActivePotion[3];
    point Healmet;
    point Armor;
    point Pants;
    point Socks;
    point Sword;

    point rings;
    short NumOfRings;

    point Crafting;
    short Craft[2] = { 2, 2};

    point AfterCraftPotions;

    Inventary(String path) {

        texture.loadFromFile(path);
        sprite.setTexture(texture);
        x = 229; y = 69.5;
        sprite.setPosition(x, y);
        w = 36; h = 36;

        FlowerInventary.x = 14, FlowerInventary.y = 277;
        PotionInventary.x = 237, PotionInventary.y = 277;

        Healmet.x = 14, Healmet.y = 13;
        Armor.x = 14, Armor.y = 49;
        Pants.x = 14, Pants.y = 85;
        Socks.x = 14; Socks.y = 121;

        Sword.x = 73, Sword.y = 13;
        rings.x = 73, rings.y = 49;
        NumOfRings = 3;

        ActivePotion[0].x = 194, ActivePotion[0].y = 41;
        ActivePotion[1].x = 194, ActivePotion[1].y = 89;
        ActivePotion[2].x = 194, ActivePotion[2].y = 138;

        Crafting.x = 25,Crafting.y = 182;
        AfterCraftPotions.x = 137, AfterCraftPotions.y = 197;
    }

    void CorrectInventary(const float& dx,const float& dy) {
        x = 229 + dx; y = 69.5 + dy;
        sprite.setPosition(x, y);

        FlowerInventary.x = 14 + dx, FlowerInventary.y = 277 + dy;
        PotionInventary.x = 237 + dx, PotionInventary.y = 277 + dy;

        Healmet.x = 14 + dx, Healmet.y = 13 + dy;
        Armor.x = 14 + dx, Armor.y = 49 + dy;
        Pants.x = 14 + dx, Pants.y = 85 + dy;
        Socks.x = 14 + dx; Socks.y = 121 + dy;

        Sword.x = 73 + dx, Sword.y = 13 + dy;
        rings.x = 73 + dx, rings.y = 49 + dy;
        NumOfRings = 3;

        ActivePotion[0].x = 194 + dx, ActivePotion[0].y = 41 + dy;
        ActivePotion[1].x = 194 + dx, ActivePotion[1].y = 89 + dy;
        ActivePotion[2].x = 194 + dx, ActivePotion[2].y = 138 + dy;

        Crafting.x = 25 + dx, Crafting.y = 182 + dy;
        AfterCraftPotions.x = 137 + dx, AfterCraftPotions.y = 197 + dy;
    }

};

//функция движения игрового персонажа


int main()
{
    RenderWindow window(sf::VideoMode(810, 540), "SFML works!");

    Clock clock;
    point isoMain(405, 270);

    float speed = 0.1;

    Player p("ch.png", 405, 270, 42, 62.5);

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
    float CurrentFrame = 0;

    bool OpenInventary = true;
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();

        flower_growing_timer += 0.0005 * time;
        ui.GetCordinate(p.getplayercoordinateX(), p.getplayercoordinateY());

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (event.type == Event::KeyPressed)//событие нажатия клавиши
            if ((event.key.code == Keyboard::Tab)) {//если клавиша ТАБ


                switch (OpenInventary) {//переключатель, реагирующий на логическую переменную showMissionText

                case true: {
                    window.clear();
                    inventary_menu.CorrectInventary(p.getplayercoordinateX(), p.getplayercoordinateY());
                    window.draw(inventary_menu.sprite);
                    OpenInventary = false;
                    time = 0;
                    break;//выходим , чтобы не выполнить условие "false" (которое ниже)
                }
                case false: {
                    OpenInventary = true;// а эта строка позволяет снова нажать клавишу таб и получить вывод на экран
                    break;
                }
                }
            }

        String ForDrawUI = "CP";
        int HP = p.getHP();
        for (int i = 0; i < p.getMaxHP() / 2; i++) {
            if (HP > 0) {
                HP -= 2;
                ForDrawUI += 'F';
            }
            else if (HP == -1) {
                HP -= 10;
                ForDrawUI += 'H';
            }
            else ForDrawUI += 'E';
        }

        
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            p.dir = 1; p.speed = speed;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.sprite.setTextureRect(IntRect(42 * int(CurrentFrame), 62.5, 42, 62.5));
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            p.dir = 0; p.speed = speed;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.sprite.setTextureRect(IntRect(42 * int(CurrentFrame), 125, 42, 62.5));
        }

        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            p.dir = 3; p.speed = speed;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.sprite.setTextureRect(IntRect(42 * int(CurrentFrame), 187.5 + 3, 42, 62.5));

        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            p.dir = 2; p.speed = speed;
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

            //Рисуем Карту
         for (int i = 0; i < HEIGHT_MAP; i++)
            {
                for (int j = 0; j < WIDTH_MAP; j++)
                {
                    point iso = twoDtoIso(50 * i, 50 * j);
                    if (TileMap[i][j] == 'W') DrawingPicture = water.print(iso.x, iso.y);

                    else if (TileMap[i][j] == '.' || TileMap[i][j] == 'f') DrawingPicture = grass.print(iso.x, iso.y);
                    else if (TileMap[i][j] == 'H') DrawingPicture = grass.print(iso.x, iso.y);

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
         for (int i = 0; i < ForDrawUI.getSize(); i++) {
                ui.Get_Texture(ForDrawUI[i]);
                if (ForDrawUI[i] == 'F' || ForDrawUI[i] == 'H' || ForDrawUI[i] == 'E') {
                    ui.sprite.setPosition(ui.Health.x, ui.Health.y + 37 * (i - 2) + 10);
                }
                window.draw(ui.sprite);
            }

            
         
         if (flower_growing_timer == 30) {
             TileMap[rand() % 26 + 12][rand() % 38 + 1] = 'f';
             flower_growing_timer = 0;
         }
         window.draw(p.sprite);
        
        

        
        window.display();
        }

    return 0;
}
