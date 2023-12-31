#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Zombie.hpp"
#include "../CommonEnum.hpp"
#include "../Holders/TextureHolder.hpp"

using namespace std;

Zombie::Zombie(){}

void Zombie::spawn(Vector2f spawnLoaction, float spriteScaling, ZombieType type, ZombieMoveStyle moveStyle, int seed, FloatRect playArea) {

    m_SpriteScaling = spriteScaling;

    Texture &texture = TextureHolder::GetTexture(M_SPRITE_FILENAME[type]);
    Vector2f textureSize = (Vector2f)texture.getSize();
    m_Sprite = Sprite(texture, M_SPRITE_CROP[type]);
    m_Sprite.setScale(Vector2f(m_SpriteScaling, m_SpriteScaling));
    m_CenterOffset = Vector2f((m_Sprite.getLocalBounds().width * m_SpriteScaling) / 2, ((m_Sprite.getLocalBounds().height * m_SpriteScaling) / 2));
    m_Health = M_HEALTH_BASE[type];
    m_Sprite.setColor(M_COLOR_BASE[type]);

    // vary speed to make each one unique
    srand((int)(time(0)) * seed);
    // somewhere between 80-100
    float modifierX = (rand() % M_MAX_VARIANCE) + M_OFFSET;
    float modifierY = (rand() % M_MAX_VARIANCE) + M_OFFSET;
    // convert to a fraction of 1; range 0.7-1
    modifierX /= 100;
    modifierY /= 100;
    m_Speed = Vector2f(M_SPEED_BASE[type] * modifierX, M_SPEED_BASE[type] * modifierY);

    // initialize locatio
    m_Position = spawnLoaction;
    m_PlayArea = playArea;
    m_MoveStyle = moveStyle;
    // set origin to center
    m_Sprite.setOrigin(m_CenterOffset);
    // set position
    m_Sprite.setPosition(m_Position);
    // set bounds size
    m_Bounds = m_Sprite.getLocalBounds();

    // make alive
    m_Alive = true;
}

bool Zombie::hit() {
    m_Health--;
    if (m_Health < 0) {
        // dead
        m_Alive = false;
        m_Sprite.setTexture(TextureHolder::GetTexture("../assets/sprites/dungeon/pixel-poem/Dungeon_Tileset-x1.png"));

        return true;
    }
    else {
        return false;
    }
}

bool Zombie::isAlive() {
    return m_Alive;
}

FloatRect Zombie::getHitBox() {
    FloatRect bounds(
        m_Position.x - (m_CenterOffset.x * m_SpriteScaling),
        m_Position.y - (m_CenterOffset.y * m_SpriteScaling),
        m_Bounds.width * m_SpriteScaling,
        m_Bounds.height * m_SpriteScaling);
    return bounds;
}

RectangleShape Zombie::getDrawableHitbox() {
    FloatRect bounds = Zombie::getHitBox();
    RectangleShape drawableHitbox(sf::Vector2f(bounds.width, bounds.height));
    drawableHitbox.setPosition(bounds.left, bounds.top);

    // Set the fill color to transparent and set an outline color and thickness
    drawableHitbox.setFillColor(sf::Color::Transparent);
    drawableHitbox.setOutlineColor(sf::Color::Red);
    drawableHitbox.setOutlineThickness(1.f);

    return drawableHitbox;
}

Sprite Zombie::getSprite() {
    return m_Sprite;
}

Vector2f Zombie::getPosition() {
    return m_Position;
}

void Zombie::update(Time frameTime, Vector2f playerLocation) {
    if(m_Alive){
        float distanceX = (playerLocation.x - m_Position.x);
        float distanceY = (playerLocation.y - m_Position.y);

        bool moveX, moveY;

        Vector2f speed = m_Speed;

        Vector2i inverseChance = M_BASE_INVERSE_CHANCE;

        if (m_SpeedInverse.x == -1) {
            inverseChance.x /= 2;
        }
        if (m_SpeedInverse.y == -1) {
            inverseChance.y /= 2;
        }

        if ((rand() % inverseChance.x) == 0) m_SpeedInverse.x = -m_SpeedInverse.x;
        if ((rand() % inverseChance.y) == 0) m_SpeedInverse.y = -m_SpeedInverse.y;

        if (!m_PlayArea.contains(m_Position)) {
            if (m_Position.x < m_PlayArea.left)   m_SpeedInverse.x = 1;
            if (m_Position.x > m_PlayArea.width)  m_SpeedInverse.x = 1;
            if (m_Position.y < m_PlayArea.top)    m_SpeedInverse.y = 1;
            if (m_Position.y > m_PlayArea.height) m_SpeedInverse.y = 1;
        }

        if (distanceX != 0) {
            moveX = true;
        }
        if (distanceY != 0) {
            moveY = true;
        }


        if (m_MoveStyle == ZombieMoveStyle::ZOMBIE_MOVESTYLE_XFIRST) {
            if (abs(distanceX) >= 50) {
                moveX = true;
                moveY = false;
            }
        }
        else if (m_MoveStyle == ZombieMoveStyle::ZOMBIE_MOVESTYLE_YFIRST) {
            if (abs(distanceY) >= 50) {
                moveX = false;
                moveY = true;
            }
        }
        else if (m_MoveStyle == ZombieMoveStyle::ZOMBIE_MOVESTYLE_RANDOM) {
            moveX = (rand() % 2) == 0;
            moveY = (rand() % 2) == 0;
        }

        // check for moving in diagonal
        if (moveX && moveY) {
            speed = Vector2f(m_Speed.x * sqrt(0.5), m_Speed.y * sqrt(0.5));
        }


        if (moveX == true) {
            if (distanceX > 0) m_Position.x += (speed.x * m_SpeedInverse.x) * frameTime.asSeconds();
            if (distanceX < 0) m_Position.x -= (speed.x * m_SpeedInverse.x) * frameTime.asSeconds();
        }
        if (moveY == true) {
            if (distanceY > 0) m_Position.y += (speed.y * m_SpeedInverse.y) * frameTime.asSeconds();
            if (distanceY < 0) m_Position.y -= (speed.y * m_SpeedInverse.y) * frameTime.asSeconds();
        }

        // flip zombie if facing in -x direction
        // if ((playerLocation.x - m_Position.x ) < 0) {
        //     m_Sprite.setScale(Vector2f(-m_SpriteScaling, m_SpriteScaling));
        // }
        // else {
        //     m_Sprite.setScale(Vector2f(-m_SpriteScaling, m_SpriteScaling));
        // }

        // set new position
        m_Sprite.setPosition(m_Position);

        // // face zombie to player
        // float angle = (atan2(playerY - m_Position.y,
        //                     playerX - m_Position.x)
        //                * 180) / M_PI;
        // m_Sprite.setRotation(angle);
    }
}
