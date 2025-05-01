#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Window.hpp>
#include <fmt/format.h>

constexpr auto kWindowWidth = 800;
constexpr auto kWindowHeight = 600;

int main() {
    sf::Window window{sf::VideoMode({kWindowWidth, kWindowHeight}), 
                      "Diffraction", sf::Style::Close, sf::State::Windowed};

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.display();
    }
}
