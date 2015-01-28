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

//checkbox control with text label

#ifndef UI_CHECKBOX_H_
#define UI_CHECKBOX_H_

#include <UIControl.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>

namespace ui
{
    class CheckBox final : public Control
    {
    public:
        typedef std::shared_ptr<CheckBox> Ptr;

        CheckBox(const sf::Font& font, const sf::Texture& t);
        ~CheckBox() = default;

        bool selectable() const override;
        void select() override;
        void deselect() override;

        void activate() override;
        void deactivate() override;

        void handleEvent(const sf::Event&, const sf::Vector2f&) override;
        void setAlignment(Alignment) override;
        bool contains(const sf::Vector2f& mousePos) const override;

        void setText(const std::string& text);
        void setTextColour(const sf::Color&);
        void setFont(const sf::Font&);
        void setFontSize(sf::Uint8);

        bool checked() const;
        void check(bool checked = true);
        
    private:
        enum State
        {
            Normal = 0,
            Selected,
            CheckedNormal,
            CheckedSelected
        };

        const sf::Texture& m_texture;
        sf::Sprite m_sprite;
        sf::Text m_text;
        std::vector<sf::IntRect> m_subRects;
        bool m_checked;
        Alignment m_alignment;

        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    };
}

#endif //UI_CHECKBOX_H_