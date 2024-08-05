#pragma once
#include <SFML/Graphics.hpp>

class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;
    std::string label;
    bool isActive;

    Button(sf::Vector2f size, sf::Vector2f position, const std::string& label, sf::Font& font)
            : label(label), isActive(false)
    {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(sf::Color::White);

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(34);
        text.setFillColor(sf::Color::Black);

        // Center the text within the button
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
        text.setPosition(position + size / 2.0f);
    }

    // Check if the button was clicked
    bool isClicked(const sf::Vector2f& mousePos) {
        return shape.getGlobalBounds().contains(mousePos);
    }

    // Activate or deactivate the button
    void setActive(bool active) {
        isActive = active;
        if (isActive) {
            shape.setFillColor(sf::Color(150, 150, 150)); // Dim color
        } else {
            shape.setFillColor(sf::Color::White); // Normal color
        }
    }
};
