/*********************************************************************
Matt Marchant 2014
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

//creates an aggregated stack of commands, executed by the scene graph each frame

#ifndef COMMAND_STACK_H_
#define COMMAND_STACK_H_

#include <queue>
#include <functional>

//category for node targets
namespace Category
{
    enum Type
    {
        None      = 0,
        PlayerOne = (1 << 0), //probably shouldn't rely on these actually being '1' and '2' respectively
        PlayerTwo = (1 << 1),
        Block     = (1 << 2),
        Enemy     = (1 << 3)
    };
}

class Node;
struct Command
{
    Command();
    ~Command() = default;
    std::function<void(Node&, float)> action;
    unsigned short categoryMask; //target node categories are OR'd into this
};

class CommandStack final
{
public:
    void push(const Command& command);
    Command pop();
    bool empty() const;

private:
    std::queue<Command> m_stack;
};

#endif //COMMAND_STACK_H_