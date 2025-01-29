#include <SFML/Graphics.hpp>
#include "System/GameEngine.hpp"
int main()
{
    constexpr int wWidth = 1280;
    constexpr int wHeight = 720;
    auto window = sf::RenderWindow(sf::VideoMode({wWidth, wHeight}), "CMake SFML Project");
    window.setFramerateLimit(60);

    EntityManager entityManager;
    auto e = entityManager.addEntity("Bullet");
    e->cShape = std::make_shared<CShape>();
    auto& circle = e->cShape->shape;
    circle.setRadius(40.f);
    circle.setPosition({ 100.f, 100.f });
    circle.setFillColor(sf::Color::Red);
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        window.draw(circle);
        window.display();
    }
}
