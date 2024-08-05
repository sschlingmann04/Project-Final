#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "Button.h"
#include <random>
using namespace std;

int main() {
    int width = 1500;
    int height = 1000;

    sf::RenderWindow window(sf::VideoMode(width, height), "UFO Sightings!", sf::Style::Close | sf::Style::Titlebar);

    // Load font
    sf::Font font;
    font.loadFromFile("../files/font.ttf");

    // Define button attributes
    sf::Vector2f buttonSize(160, 70);
    std::vector<std::string> labels = { "City", "State", "Shape", "Year" };
    std::vector<Button> buttons;

    // Create buttons
    for (size_t i = 0; i < labels.size(); i++) {
        buttons.emplace_back(buttonSize, sf::Vector2f(380 + i * 200, 360), labels[i], font);
    }

    Button submit(buttonSize, {680, 600}, "Submit", font);

    sf::Text welcomeText;
    welcomeText.setString("USA UFO SIGHTINGS!");
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(50);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setOutlineColor(sf::Color::Black);
    welcomeText.setOutlineThickness(3);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setPosition(width / 2.0f, height / 5.0f );
    welcomeText.setOrigin(welcomeText.getGlobalBounds().width / 2.0f, 0);


    sf::Text parameterPrompt;
    parameterPrompt.setString(" Hashmap vs Map\nSelect Parameter:");
    parameterPrompt.setFont(font);
    parameterPrompt.setStyle(sf::Text::Bold);
    parameterPrompt.setFillColor(sf::Color::White);
    parameterPrompt.setCharacterSize(34);
    parameterPrompt.setPosition(width / 2.0f, height / 5.0f + 75);
    parameterPrompt.setOrigin(parameterPrompt.getLocalBounds().width / 2.0f, 0);

    sf::Text specific;
    specific.setString("Enter Specific Search:");
    specific.setFont(font);
    specific.setStyle(sf::Text::Bold);
    specific.setFillColor(sf::Color::White);
    specific.setCharacterSize(34);
    specific.setPosition(width / 2.0f - 20, height / 2.0f - 20);
    specific.setOrigin(parameterPrompt.getLocalBounds().width / 2.0f, 0);

    sf::Text byPass; // byPass specified parameters
    byPass.setString("For all Sightings, Press Submit Only");
    byPass.setFont(font);
    byPass.setStyle(sf::Text::Bold);
    byPass.setFillColor(sf::Color::White);
    byPass.setCharacterSize(34);
    byPass.setPosition(width / 2.0f - 200, height / 2.0f + 340);
    byPass.setOrigin(parameterPrompt.getLocalBounds().width / 2.0f, 0);





    sf::Text output;
    output.setFont(font);
    output.setStyle(sf::Text::Bold);
    output.setFillColor(sf::Color::White);
    output.setCharacterSize(30);
    output.setPosition(width / 2.0f, height / 5.0f + 75);
    output.setOrigin(parameterPrompt.getLocalBounds().width / 2.0f, 0);

    sf::Text output2;
    output2.setFont(font);
    output2.setStyle(sf::Text::Bold);
    output2.setFillColor(sf::Color::White);
    output2.setCharacterSize(30);
    output2.setOrigin(parameterPrompt.getLocalBounds().width / 2.0f, 0);

    sf::Text userInput;
    userInput.setFont(font);
    userInput.setCharacterSize(34);
    userInput.setStyle(sf::Text::Bold);
    userInput.setFillColor(sf::Color::Yellow);
    userInput.setPosition(width / 2.0f, height / 5.0f + 340);
    userInput.setOrigin(userInput.getLocalBounds().width / 2.0f, 0);

    sf::RectangleShape rectangle(sf::Vector2f(3, 34));
    rectangle.setFillColor(sf::Color::Yellow);
    rectangle.setPosition(width / 2.0f, height / 5.0f + 340);
    int adjustment = 0;
    rectangle.setOrigin(userInput.getLocalBounds().width / 2.0f + adjustment, 0);

    sf::Color colorPurple(35, 35, 70);

    sf::Texture ufoImage1;
    sf::Texture ufoImage2;
    ufoImage1.loadFromFile("../files/images/ufoImage.png");
    ufoImage2.loadFromFile("../files/images/ufoResults.png");

    sf::Sprite ufo;
    ufo.setTexture(ufoImage1);
    ufo.setPosition(width / 2.0f, height / 2.0f - 75);
    ufo.setOrigin(ufo.getLocalBounds().width / 2 , ufo.getLocalBounds().height / 2.35);
    ufo.setScale(2,2);
    ufo.setColor(sf::Color(255,255,255,128));

    bool windowVal = false;
    string parameter = "";
    string name = "";

    sf::Text nameDisplay;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) // Keep this
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 32 ) {


                    name += event.text.unicode;

                    adjustment -= 20;
                    userInput.setOrigin(userInput.getLocalBounds().width / 2.0f, 0);
                    rectangle.setOrigin(userInput.getLocalBounds().width / 2.0f + adjustment, 0);

                }
                userInput.setString(name);
            }

            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Backspace) {
                    name.pop_back();
                    adjustment += 20;

                    if (adjustment > userInput.getLocalBounds().width / 2.0f) {
                        adjustment = userInput.getLocalBounds().width / 2.0f;
                    }
                    rectangle.setOrigin(userInput.getLocalBounds().width / 2.0f + adjustment, 0);
                    userInput.setString(name);
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Check which buttons were clicked and activate it
                for (auto& button : buttons) {
                    if (button.isClicked(mousePos)) {
                        // Deactivate all buttons
                        for (auto &otherButton: buttons) {
                            otherButton.setActive(false);
                        }
                        // Activate the clicked button
                        button.setActive(true);
                        parameter = button.label;
                    }
                }
                if (submit.isClicked(mousePos)){
                    submit.setActive(true);
                    cout << "submitted parameter" << parameter << ":  \nspecific" << name << "  \n"  <<  submit.label;
                    window.close();
                    windowVal = true;
                }
            }
        }



        window.clear(colorPurple); // Keep Drawings Below this line
        window.draw(ufo);
        window.draw(welcomeText);
        window.draw(parameterPrompt);
        window.draw(specific);
        window.draw(byPass);
        window.draw(rectangle);
        window.draw(userInput);
        for (const auto& button : buttons) {
            window.draw(button.shape);
            window.draw(button.text);
        }
        window.draw(submit.shape);
        window.draw(submit.text);
        window.draw(nameDisplay);

        // #TODO cout << name << endl;
        window.display(); // Keep All Drawings Above this line



    }

    sf::RenderWindow Results(sf::VideoMode(width, height), "Hashmap vs Hashtable", sf::Style::Close | sf::Style::Titlebar);
    sf::View view(sf::FloatRect(0, 0, width, height));

    view.setCenter(width / 2 , height / 2);
    sf::RectangleShape largeContent(sf::Vector2f(800, 2000));
    largeContent.setFillColor(sf::Color::Green);

    while (Results.isOpen() && windowVal == true) {
        sf::Event event;
        welcomeText.setString("Searching for UFOS via: " + name);
        welcomeText.setOrigin(welcomeText.getGlobalBounds().width / 2.0f, 0);
        while (Results.pollEvent(event)) {
            if (event.type == sf::Event::Closed) // Keep this
                Results.close();
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                {
                    // Adjust the view's center to scroll up or down
                    float scrollSpeed = 30.0f;
                    view.move(0, -event.mouseWheelScroll.delta * scrollSpeed);
                }
            }
            float viewHeight = window.getSize().y;
            float contentHeight = largeContent.getSize().y;
            sf::Vector2f viewCenter = view.getCenter();
            if (viewCenter.y - viewHeight / 2 < 0)
                view.setCenter(viewCenter.x, viewHeight / 2);
            else if (viewCenter.y + viewHeight / 2 > contentHeight)
                view.setCenter(viewCenter.x, contentHeight - viewHeight / 2);

            string map = "Hashmap";
            string table = "Map";
            string foo = "";
            string boop = "";

            for (auto i : map){
                foo += i;
                foo += "\n";
            }
            for (auto j : table){
                boop += j;
                boop += "\n";
            }
            output.setString(foo);
            output2.setString(boop);


        }
        output.setPosition(width/ 4.0f, height / 5.0f + 75);
        output2.setPosition(width - 100, height / 5.0f + 75);

        ufo.setTexture(ufoImage2);
        Results.clear(colorPurple);
        Results.setView(view);

        Results.draw(output);
        Results.draw(output2);
        Results.draw(ufo);
        Results.draw(welcomeText);

        Results.display();
    }

}
