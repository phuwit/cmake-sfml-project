#include <SFML/Graphics.hpp>

#include "Scenes.hpp"
#include "../CommonEnum.hpp"
#include "../Holders/TextureHolder.hpp"
#include "../Holders/FontHolder.cpp"
#include "../Tools/SetOriginCenter.cpp"
#include "../Tools/SetRectangleShapeOnMouseHover.cpp"

class Menu : public Scenes {
    public:
        virtual SceneChange run(RenderWindow &window);
};

SceneChange Menu::run(RenderWindow &window) {
    Sprite logo(TextureHolder::GetTexture("assets/sprites/brand/logo.png"));
    spriteSetOriginCenter(logo);
    logo.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y * 0.25));

    Font fontBebas = FontHolder::GetFont("assets/fonts/BebasNeue-Regular.otf");

    int BUTTON_PADDING = 30;

    Text playText("Play!", fontBebas);
    playText.setFillColor(Color::Black);
    playText.setCharacterSize(60);
    RectangleShape playButton(Vector2f(playText.getLocalBounds().width + BUTTON_PADDING, playText.getLocalBounds().height + BUTTON_PADDING));
    textSetOriginCenter(playText);
    shapeSetOriginCenter(playButton);
    playText.setPosition(logo.getPosition() + Vector2f(0, 400));
    playButton.setPosition(playText.getPosition() + Vector2f(0, BUTTON_PADDING / 2));

    Text exitText("Exit", fontBebas);
    exitText.setFillColor(Color::Black);
    exitText.setCharacterSize(60);
    RectangleShape exitButton(Vector2f(exitText.getLocalBounds().width + BUTTON_PADDING, exitText.getLocalBounds().height + BUTTON_PADDING));
    textSetOriginCenter(exitText);
    shapeSetOriginCenter(exitButton);
    exitText.setPosition(playText.getPosition() + Vector2f(0, 100));
    exitButton.setPosition(exitText.getPosition() + Vector2f(0, BUTTON_PADDING / 2));

    Text infoText("Info", fontBebas);
    infoText.setFillColor(Color::Black);
    infoText.setCharacterSize(60);
    RectangleShape infoButton(Vector2f(infoText.getLocalBounds().width + BUTTON_PADDING, infoText.getLocalBounds().height + BUTTON_PADDING));
    textSetOriginCenter(infoText);
    shapeSetOriginCenter(infoButton);
    infoText.setPosition(exitText.getPosition() + Vector2f(0, 100));
    infoButton.setPosition(infoText.getPosition() + Vector2f(0, BUTTON_PADDING / 2));

    while (window.isOpen()) {
        // Update frame
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::MouseMoved) {
                Vector2f mouseScreenPosition = Vector2f(Mouse::getPosition(window));

                setRectangleShapeColorOnMouseHover(playButton, mouseScreenPosition, Color(189, 189, 189), Color::White);
                setRectangleShapeColorOnMouseHover(exitButton, mouseScreenPosition, Color(189, 189, 189), Color::White);
                setRectangleShapeColorOnMouseHover(infoButton, mouseScreenPosition, Color(189, 189, 189), Color::White);
            }
            if (event.type == Event::MouseButtonPressed) {
                Vector2f mouseScreenPosition = Vector2f(Mouse::getPosition(window));

                if (playButton.getGlobalBounds().contains(Vector2f(mouseScreenPosition))) {
                    return SceneChange(ScenesList::SCENE_GAME);
                } else if (exitButton.getGlobalBounds().contains(Vector2f(mouseScreenPosition))) {
                    return SceneChange(ScenesList::EXIT);
                } else if (infoButton.getGlobalBounds().contains(Vector2f(mouseScreenPosition))) {
                    return SceneChange(ScenesList::SCENE_INFO);
                }
            }
        }

        // Draw frame

        window.clear(COLOR_BACKGROUND);
            window.draw(logo);

            window.draw(playButton);
            window.draw(playText);
            window.draw(exitButton);
            window.draw(exitText);
            window.draw(infoButton);
            window.draw(infoText);

        window.display();
    }

    return SceneChange(EXIT);
}
