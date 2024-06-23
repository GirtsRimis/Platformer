#include <iostream>
#include <SFML/Graphics.hpp>

const int windowHeight = 600;
const int windowWidth = 800;

class Platform {
public:
    sf::RectangleShape shape;

    Platform(float startX, float startY, float width, float height) {
        shape.setPosition(startX, startY);
        shape.setSize({width, height});
        shape.setFillColor(sf::Color::Red);
    }
};

class GameCharacter {
public:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool playerJump = false;

    GameCharacter(float startX, float startY) {
        shape.setPosition(startX, startY);
        shape.setSize({15.f, 15.f});
        shape.setFillColor(sf::Color::Green);
        velocity = sf::Vector2f(0, 0);
    }

    std::string collisionCheck(const sf::RectangleShape& firstObject, const sf::RectangleShape& secondObject) {
        float deltaX = (firstObject.getPosition().x + firstObject.getSize().x / 2) - (secondObject.getPosition().x + secondObject.getSize().x / 2);
        float deltaY = (firstObject.getPosition().y + firstObject.getSize().y / 2) - (secondObject.getPosition().y + secondObject.getSize().y / 2);

        float width = (firstObject.getSize().x + secondObject.getSize().x) / 2;
        float height = (firstObject.getSize().y + secondObject.getSize().y) / 2;

        float crossWidth = width * deltaY;
        float crossHeight = height * deltaX;

        std::string collisionDirection = "";

        if (std::abs(deltaX) <= width && std::abs(deltaY) <= height) {
            if (crossWidth > crossHeight) {
                collisionDirection = (crossWidth > -crossHeight) ? "Bottom" : "Left";
            } else {
                collisionDirection = (crossWidth > -crossHeight) ? "Right" : "Top";
            }
        }

        std::cout << collisionDirection << "\n";
        return collisionDirection;
    }

    void collideMovement(sf::RectangleShape& firstObject, const sf::RectangleShape& secondObject) {
        std::string collisionDirection = collisionCheck(firstObject, secondObject);

        if (collisionDirection == "Top") {
            firstObject.setPosition(firstObject.getPosition().x, secondObject.getPosition().y - firstObject.getSize().y);
            velocity.y = 0;
        } else if (collisionDirection == "Bottom") {
            firstObject.setPosition(firstObject.getPosition().x, secondObject.getPosition().y + secondObject.getSize().y + firstObject.getSize().y);
            velocity.y = 0;
        } else if (collisionDirection == "Left") {
            firstObject.setPosition(secondObject.getPosition().x - firstObject.getSize().x, firstObject.getPosition().y);
            velocity.x = 0;
        } else if (collisionDirection == "Right") {
            firstObject.setPosition(secondObject.getPosition().x + secondObject.getSize().x, firstObject.getPosition().y);
            velocity.x = 0;
        }
    }

    void update(const std::vector<Platform>& platforms, const std::vector<sf::RectangleShape> tileVector) {
        if (shape.getPosition().y < windowHeight - shape.getSize().y) {
            velocity.y += 0.5f;
        } else {
            shape.setPosition(shape.getPosition().x, windowHeight - shape.getSize().y);
            velocity.y = 0;
        }

        for (auto& platform : platforms) {
            if (shape.getGlobalBounds().intersects(platform.shape.getGlobalBounds())) {
                collideMovement(shape, platform.shape);
            }
        }

        for (auto& tilePiece : tileVector) {
            if (shape.getGlobalBounds().intersects(tilePiece.getGlobalBounds())) {
                collideMovement(shape, tilePiece);
            }
        }

        velocity.x *= 0.9f;

        if (playerJump) {
            velocity.y = -10.f;
            playerJump = false;
        }

        shape.move(velocity);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");
    window.setVerticalSyncEnabled(true);

    const int tileSize = 32;
    const int levelWidth = 25;
    const int levelHeight = 19;

    int level[levelHeight][levelWidth] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
    std::vector<sf::RectangleShape> tileVector;

    for (int y = 0; y < levelHeight; ++y) {
        for (int x = 0; x < levelWidth; ++x) {
            tile.setPosition(x * tileSize, y * tileSize);
            if (level[y][x] == 1) {
                tile.setFillColor(sf::Color::White);
                tileVector.push_back(tile);
            } 
        }
    }

    GameCharacter character(400, 300);

    std::vector<Platform> platforms;

    platforms.push_back(Platform(500, 500, 150, 10));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            character.velocity.x += 0.5f;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            character.velocity.x -= 0.5f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            character.playerJump = true;
        }

        window.clear();
        
        for (auto& tilePiece : tileVector) {
            window.draw(tilePiece);
        }

        for (auto& platform : platforms) {
            window.draw(platform.shape);
        }

        character.update(platforms, tileVector);
        window.draw(character.shape);

        window.display();
    }

    return 0;
}