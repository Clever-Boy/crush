/*********************************************************************
Matt Marchant 2014 - 2015
http://trederia.blogspot.com

Crush - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include <UITextBox.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

using namespace ui;

namespace
{
    const float borderThickness = 2.f;
    const sf::Vector2f defaultSize(320.f, 40.f);
    const float flashTime = 0.4f;
    const float padding = 20.f;
}

TextBox::TextBox(const sf::Font& font, const sf::Color& backColour, const sf::Color& borderColour)
    : m_text        ("", font, 30u),
    m_cursorShape   (sf::Vector2f(12.f, 34.f)),
    m_showCursor    (false),
    m_lastKey       (sf::Keyboard::Unknown),
    m_borderColour  (borderColour),
    m_selectedColour(borderColour.r / 2u, borderColour.g / 2u, borderColour.b / 2u, borderColour.a)
{
    m_backShape.setFillColor(backColour);
    m_backShape.setOutlineColor(borderColour);
    m_backShape.setOutlineThickness(borderThickness);
    m_backShape.setSize(defaultSize);

    m_cursorShape.setFillColor(m_selectedColour);

    m_text.setColor(borderColour);
    m_text.setPosition(padding, 0.f);
}

//public
bool TextBox::selectable() const
{
    return true;
}

void TextBox::select()
{
    Control::select();
    m_backShape.setOutlineColor(m_selectedColour);
}

void TextBox::deselect()
{
    Control::deselect();
    m_backShape.setOutlineColor(m_borderColour);
}

void TextBox::activate()
{
    Control::activate();
    m_showCursor = true;
}

void TextBox::deactivate()
{
    Control::deactivate();
    m_showCursor = false;
}

void TextBox::handleEvent(const sf::Event& e, const sf::Vector2f& mousePos)
{
    if (e.type == sf::Event::KeyReleased)
    {
        if (e.key.code == sf::Keyboard::BackSpace)
        {
            if (m_string.size())
            {
                m_string.pop_back();
            }
        }
        else if (e.key.code == sf::Keyboard::Return)
        {
            deactivate();
        }
    }

    else if (e.type == sf::Event::TextEntered)
    {
        if (e.text.unicode > 31
            && e.text.unicode < 127)
        {
            m_string += static_cast<char>(e.text.unicode);
        }
    }
    else if (e.type == sf::Event::JoystickButtonReleased)
    {
        if (e.joystickButton.button == 1)
        {
            deactivate();
        }
    }
}

void TextBox::update(float dt)
{
    if (active())
    {
        sf::FloatRect bounds = m_text.getGlobalBounds();
        m_cursorShape.setPosition(bounds.left + bounds.width, (m_backShape.getSize().y - m_cursorShape.getSize().y) / 2.f);
        m_text.setString(m_string);

        if (m_cursorClock.getElapsedTime().asSeconds() > flashTime)
        {
            m_showCursor = !m_showCursor;
            m_cursorClock.restart();
        }
    }
}

void TextBox::setAlignment(Alignment a)
{
    switch (a)
    {
    case Alignment::TopLeft:
        setOrigin(0.f, 0.f);
        break;
    case Alignment::BottomLeft:
        setOrigin(0.f, m_backShape.getSize().y);
        break;
    case Alignment::Centre:
        setOrigin(m_backShape.getSize() / 2.f);
        break;
    case Alignment::TopRight:
        setOrigin(m_backShape.getSize().x, 0.f);
        break;
    case Alignment::BottomRight:
        setOrigin(m_backShape.getSize());
        break;
    default:break;
    }
}

bool TextBox::contains(const sf::Vector2f& mousePos) const
{
    return getTransform().transformRect(m_backShape.getGlobalBounds()).contains(mousePos);
}

const std::string& TextBox::getText() const
{
    return m_string;
}

void TextBox::setTexture(const sf::Texture& t)
{
    m_backShape.setTexture(&t);
}

void TextBox::showBorder(bool show)
{
    (show) ? 
        m_backShape.setOutlineThickness(borderThickness) : 
        m_backShape.setOutlineThickness(0.f);
}

void TextBox::setSize(const sf::Vector2f& size)
{
    m_backShape.setSize(size);
    m_cursorShape.setScale(1.f, size.y / defaultSize.y);

    sf::Uint8 charSize = static_cast<sf::Uint8>(size.y * 0.75f);
    m_text.setCharacterSize(charSize);
    //TODO set text padding?
}

void TextBox::setText(const std::string& text)
{
    m_string = text;
    m_text.setString(m_string);
}

//private
void TextBox::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();
    rt.draw(m_backShape, states);
    rt.draw(m_text, states);
    if (m_showCursor) rt.draw(m_cursorShape, states);
}