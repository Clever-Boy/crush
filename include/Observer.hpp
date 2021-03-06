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

//implements the observer pattern

#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <CommandCategories.hpp>

#include <vector>

//namespace game
//{
    class Event
    {
    public:
        //when a scene node is modified
        struct NodeEvent
        {
            Category::Type type;
            enum Action
            {
                Spawn = 1, //placed into scene
                Despawn, //removed from scene
                KilledNode, //killed another node in scene
                HitWater, //this node landed in water
                InvincibilityExpired,
                WentTurbo,
                LeftTurbo
            }action;
            Category::Type target; //node type which was target of deathness
            Category::Type owner; //if this is a block, who last owned it
            float positionX, positionY;
            float speed; //speed at which the entity was travelling when event raised
        };

        //when players do stuff
        struct PlayerEvent
        {
            Category::Type playerId;
            enum Action
            {
                Landed = 1,
                Grabbed,
                Released,
                Died,
                PickedUp,
                Dropped,
                GotItem,
                Moved,
                Stopped,
                StartedFalling,
                Jumped,
                HitWater,
                GotHat,
                LostHat,
                KillStreak
            }action;
            enum Item
            {
                ExtraLife = 1,
                ExtraSpeed,
                JumpIncrease,
                ReverseControls,
                SuperStrength,
                SpeedReduction,
                JumpReduction,
                Size
            }item;
            float positionX, positionY;
        };
    
        struct NpcEvent
        {
            enum Action
            {
                Jumped,
                Landed,
                TurnedLeft,
                TurnedRight,
                HitWater,
                Stopped,
                Started
            }action;
            float positionX, positionY;
        };

        struct BlockEvent
        {
            enum Action
            {
                DragStart,
                DragEnd,
                HitGround,
                HitWater
            }action;
            float positionX, positionY;
        };

        struct GameEvent
        {
            enum Action
            {
                PlayerOneEnable = 1,
                PlayerTwoEnable,
                NpcEnable,
                PlayerOneDisable,
                PlayerTwoDisable,
                NpcDisable
            }action;
        };

        struct HatEvent
        {
            enum Action
            {
                HitWater,
                HitGround
            }action;
            float positionX, positionY;
        };

        enum Type
        {
            Node = 1,
            Player,
            Npc,
            Block,
            Game,
            Hat
        } type;

        union 
        {
            NodeEvent node;
            PlayerEvent player;
            NpcEvent npc;
            BlockEvent block;
            GameEvent game;
            HatEvent hat;
        };
    };
//}

class Subject;
class Observer
{
public:
    virtual ~Observer() = default;
    //when implementing this either deal with event directly, or
    //add to an event list in the inheriting class for deferred handling
    virtual void onNotify(Subject&, const Event& evt) = 0;
};

class Subject
{
public:
    virtual ~Subject(){};

    void addObserver(Observer& o)
    {
        m_observers.push_back(&o);
    }

    //use this if adding an observer during a notification
    //so as not to invalidate current notification list
    void delayAddObserver(Observer& o)
    {
        m_delayedObservers.push_back(&o);
    }

protected:
    void notify(Subject& s, Event evt)
    {
        for (auto& o : m_observers)
            o->onNotify(s, evt);

        //update with any new observers we may have received
        for (auto o : m_delayedObservers)
            m_observers.push_back(o);

        m_delayedObservers.clear();
    }

private:
    std::vector<Observer*> m_observers;
    std::vector<Observer*> m_delayedObservers;
};


#endif //OBSERVER_H_