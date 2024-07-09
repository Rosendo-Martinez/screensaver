#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

// centers the text on the shape
void centerText(const std::shared_ptr<sf::Shape> shape, std::shared_ptr<sf::Text> text)
{
    float left_s = shape->getGlobalBounds().left;
    float top_s = shape->getGlobalBounds().top;
    float width_s = shape->getGlobalBounds().width;
    float height_s = shape->getGlobalBounds().height;
    float width_t = text->getGlobalBounds().width;
    float height_t = text->getGlobalBounds().height;

    text->setPosition(left_s + width_s/2 - width_t/2, top_s + height_s/2 - height_t/2);
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

        std::cout << dummy << " " << windowWidth << " " << windowHeight << std::endl;
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

        std::cout << dummy << " " << path << " " << fontSize << " " << r << " " << g << " " << b << std::endl;
    }

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

            std::cout << type << " " << name << " " << posX << " " << posY << " " << sX << " " << R << " " << G << " " << B << " ";

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
                std::cout << w << " " << h << std::endl;
                auto rectangle = std::make_shared<sf::RectangleShape>(sf::Vector2f(w,h));
                rectangle->setPosition(posX, posY);
                rectangle->setFillColor(sf::Color(R, G, B));
                shapes.push_back(rectangle);
            }
            else if (type == "Circle")
            {
                float r;
                config >> r;
                std::cout << r << std::endl;
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

        window.clear();
        
        // iterate through shapes
        for (int i = 0; i < shapes.size(); i++) {
            centerText(shapes[i], texts[i]);
            window.draw(*shapes[i]);
            window.draw(*texts[i]);
        }
        window.display();
    }

    return 0;
}