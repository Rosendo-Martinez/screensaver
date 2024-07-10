#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

void centerText(const std::shared_ptr<sf::Shape> shape, std::shared_ptr<sf::Text> text)
{
    sf::FloatRect textRect = text->getLocalBounds();
    sf::FloatRect shapeRect = shape->getGlobalBounds();
    text->setOrigin(sf::Vector2f(textRect.left + textRect.width/2, textRect.top + textRect.height/2));
    text->setPosition(sf::Vector2f(shapeRect.left + shapeRect.width/2, shapeRect.top + shapeRect.height/2));
}

int main(int argc, char* argv[]) 
{
    // open spec file
    std::ifstream config("./config.txt", std::ifstream::in);

    // read in window specs
    int windowWidth;
    int windowHeight;
    {
        std::string dummy;

        config >> dummy >> windowWidth >> windowHeight;
    }

    // read in font specs
    sf::Font font;
    int fontSize;
    sf::Color fontColor;
    {
        std::string dummy;
        std:: string path;
        int r;
        int g;
        int b;

        config >> dummy >> path >> fontSize >> r >> g >> b;

        fontColor.r = r;
        fontColor.g = g;
        fontColor.b = b;

        if (!font.loadFromFile(path)) 
        {
            std::cout << "Could not load font.\n";
            return 0;
        }
    }

    // read in shapes specs
    // shape_0, shape_1, ...
    std::vector<std::shared_ptr<sf::Shape>> shapes;
    // sX_0, sY_0, sX_1, sY_1, ...
    std::vector<float> speeds;
    // text_0, text_1, ...
    std::vector<std::shared_ptr<sf::Text>> texts;
    {
        std::string type;
        while (config >> type) 
        {
            std::string name;
            float posX, posY, sX, sY;
            int R, G, B;

            config >> name >> posX >> posY >> sX >> sY >> R >> G >> B;

            auto thisText = std::make_shared<sf::Text>();
            thisText->setFont(font);
            thisText->setCharacterSize(fontSize);
            thisText->setString(name);
            thisText->setFillColor(fontColor);
            
            texts.push_back(thisText);
            speeds.push_back(sX);
            speeds.push_back(sY);

            if (type == "Rectangle")
            {
                float w, h;
                config >> w >> h;
                auto rectangle = std::make_shared<sf::RectangleShape>(sf::Vector2f(w,h));
                rectangle->setPosition(posX, posY);
                rectangle->setFillColor(sf::Color(R, G, B));
                shapes.push_back(rectangle);
            }
            else if (type == "Circle")
            {
                float r;
                config >> r;
                auto circle = std::make_shared<sf::CircleShape>(r);
                circle->setPosition(posX,posY);
                circle->setFillColor(sf::Color(R,G,B));
                shapes.push_back(circle);
            }
            else
            {
                std::cout << "Invalid Shape type: " << type << "\n";
                return 0;
            }
        }
    }

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Assignment 1");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // move shapes
        for (int i = 0; i < shapes.size(); i++) 
        {
            float &sX = speeds[i*2];
            float &sY = speeds[i*2 + 1];
            sf::FloatRect rect = shapes[i]->getGlobalBounds();

            if (rect.left <= 0 || rect.left + rect.width >= window.getSize().x)
            {
                sX *= -1;
            }
            if (rect.top <= 0 || rect.top + rect.height >= window.getSize().y)
            {
                sY *= -1;
            }
            
            shapes[i]->move(sf::Vector2f(sX,sY));
        }

        // draw shapes
        window.clear();
        for (int i = 0; i < shapes.size(); i++) 
        {
            centerText(shapes[i], texts[i]);
            window.draw(*shapes[i]);
            window.draw(*texts[i]);
        }
        window.display();
    }

    return 0;
}