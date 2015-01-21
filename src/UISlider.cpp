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

#include <UISlider.hpp>
#include <Util.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <cassert>

using namespace ui;

namespace
{
    const float thickness = 5.f;
    const sf::Color borderColour(160u, 160u, 160u);
    const float deadzone = 40.f;
}

Slider::Slider(const sf::Font& font, const sf::Texture& texture, float length, float maxValue)
    : m_maxValue    (maxValue),
    m_length        (length),
    m_direction     (Direction::Horizontal),
    m_handleSprite  (texture),
    m_slotShape     ({ length, thickness }),
    m_text          ("", font, 20u),
    m_borderColour  (160u, 160u, 160u),
    m_activeColour  (255u, 208u, 0u)
{
    sf::IntRect subrect(0, 0, texture.getSize().x, texture.getSize().y / 2u);
    m_subRects.push_back(subrect);
    subrect.top += subrect.height;
    m_subRects.push_back(subrect);

    m_handleSprite.setTextureRect(m_subRects[State::Normal]);

    Util::Position::centreOrigin(m_handleSprite);
    m_slotShape.setOrigin(0.f, thickness / 2.f);
    m_slotShape.setFillColor(sf::Color::Black);
    m_slotShape.setOutlineColor(m_borderColour);
    m_slotShape.setOutlineThickness(3.f);
}

//public
bool Slider::selectable() const
{
    return true;
}

void Slider::select()
{
    Control::select();
    m_handleSprite.setTextureRect(m_subRects[State::Selected]);
}

void Slider::deselect()
{
    Control::deselect();
    m_handleSprite.setTextureRect(m_subRects[State::Normal]);
}

void Slider::activate()
{
    Control::activate();
    m_slotShape.setOutlineColor(m_activeColour);
}

void Slider::deactivate()
{
    Control::deactivate();
    m_slotShape.setOutlineColor(m_borderColour);
}

void Slider::handleEvent(const sf::Event& e)
{
    if (active())
    {
        if (e.type == sf::Event::KeyPressed)
        {
            if (e.key.code == sf::Keyboard::Left)
            {
                decrease();
            }
            else if (e.key.code == sf::Keyboard::Right)
            {
                increase();
            }           
        }
        else if (e.type == sf::Event::KeyReleased)
        {
            if (e.key.code == sf::Keyboard::Return)
            {
                deactivate();
            }
        }
        else if (e.type == sf::Event::JoystickMoved)
        {
            if (e.joystickMove.axis == sf::Joystick::PovX)
            {
                if (e.joystickMove.position > deadzone)
                {
                    increase();
                }
                else if (e.joystickMove.position < -deadzone)
                {
                    decrease();
                }
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
}

void Slider::setAlignment(Alignment a)
{
    switch (a)
    {
    case Alignment::TopLeft:
        setOrigin(0.f, 0.f);
        break;
    case Alignment::BottomLeft:
        setOrigin(0.f, m_slotShape.getSize().y);
        break;
    case Alignment::Centre:
        setOrigin(m_slotShape.getSize() / 2.f);
        break;
    case Alignment::TopRight:
        setOrigin(m_slotShape.getSize().x, 0.f);
        break;
    case Alignment::BottomRight:
        setOrigin(m_slotShape.getSize());
        break;
    default:break;
    }
}

void Slider::setMaxValue(float value)
{
    assert(value > 0);
    m_maxValue = value;
}

void Slider::setDirection(Direction direction)
{
    if (direction != m_direction)
    {
        m_direction = direction;

        float newX = m_slotShape.getSize().y;
        float newY = m_slotShape.getSize().x;
        m_slotShape.setSize({ newX, newY });

        newX = m_handleSprite.getPosition().y;
        newY = m_handleSprite.getPosition().x;
        m_handleSprite.setPosition(newX, newY);

        newX = m_localBounds.height;
        newY = m_localBounds.width;
        m_localBounds.width = newX;
        m_localBounds.height = newY;

        newX = m_localBounds.top;
        newY = m_localBounds.left;
        m_localBounds.left = newX;
        m_localBounds.top = newY;
    }
}

void Slider::setLength(float length)
{
    auto currentSize = m_slotShape.getSize();
    if (m_direction == Direction::Horizontal)
    {
        float pos = m_handleSprite.getPosition().x / currentSize.x;
        pos *= length;
        m_handleSprite.setPosition(pos, m_handleSprite.getPosition().y);

        currentSize.x = length;
        m_slotShape.setSize(currentSize);
        m_localBounds.width = length;
    }
    else
    {
        float pos = m_handleSprite.getPosition().y / currentSize.y;
        pos *= length;
        m_handleSprite.setPosition(m_handleSprite.getPosition().x, pos);

        currentSize.y = length;
        m_slotShape.setSize(currentSize);
        m_localBounds.height = length;
    }
    m_length = length;
}

void Slider::setValue(float value)
{
    assert(value <= m_maxValue && value >= 0);
    auto pos = m_handleSprite.getPosition();
    if (m_direction == Direction::Horizontal)
    {
        pos.x = (m_length / m_maxValue) * value;
    }
    else
    {
        pos.y = (m_length / m_maxValue) * value;
    }
    m_handleSprite.setPosition(pos);
}

float Slider::getValue() const
{
    if (m_direction == Direction::Horizontal)
    {
        return m_handleSprite.getPosition().x / m_length * m_maxValue;
    }
    else
    {
        return m_handleSprite.getPosition().y / m_length * m_maxValue;
    }
}

float Slider::getLength() const
{
    return m_length;
}

void Slider::setText(const std::string& text)
{
    m_text.setString(text);
    updateText();
}

void Slider::setTextColour(const sf::Color& colour)
{
    m_text.setColor(colour);
}

void Slider::setFontSize(sf::Uint16 size)
{
    m_text.setCharacterSize(size);
}

//private
void Slider::draw(sf::RenderTarget& rt, sf::RenderStates states)const
{
    states.transform *= getTransform();
    states.blendMode = sf::BlendMultiply;
    rt.draw(m_slotShape, states);
    states.blendMode = sf::BlendAlpha;
    rt.draw(m_handleSprite, states);
    rt.draw(m_text, states);
}

void Slider::updateText()
{
    Util::Position::centreOrigin(m_text);
    m_text.setPosition(m_text.getOrigin().x, -m_text.getLocalBounds().height * 2.f);
}

void Slider::increase()
{
    auto pos = m_handleSprite.getPosition();    
    if (m_direction == Direction::Horizontal)
    {
        pos.x = std::min(pos.x + (m_length / m_maxValue), m_length);
    }
    else
    {
        pos.y = std::min(pos.y + (m_length / m_maxValue), m_length);
    }
    m_handleSprite.setPosition(pos);
}

void Slider::decrease()
{
    auto pos = m_handleSprite.getPosition();
    if (m_direction == Direction::Horizontal)
    {
        pos.x = std::max(pos.x - (m_length / m_maxValue), 0.f);
    }
    else
    {
        pos.y = std::max(pos.y - (m_length / m_maxValue), 0.f);
    }
    m_handleSprite.setPosition(pos);
}