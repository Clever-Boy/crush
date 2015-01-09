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

//contains one or more ui controls

#ifndef UI_CONTAINER_H_
#define UI_CONTAINER_H_

#include <UIControl.hpp>

#include <vector>

namespace ui
{
    //not final, forms inherit from this
    class Container : public Control
    {
    public:
        typedef std::shared_ptr<Container> Ptr;

        Container();
        ~Container() = default;

        void addControl(Control::Ptr control);

        virtual bool selectable() const override;
        virtual void handleEvent(const sf::Event& e) override;

    private:
        std::vector<Control::Ptr> m_controls;
        sf::Int16 m_selectedIndex;

        bool hasSelection() const;
        void select(sf::Int16 index);
        void selectNext();
        void selectPrevious();

        virtual void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
    };
}

#endif //UI_CONTAINER_H_