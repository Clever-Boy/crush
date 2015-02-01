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

#include <SoundPlayer.hpp>
#include <Node.hpp>

#include <SFML/Audio/Listener.hpp>

#include <cmath>
#include <cassert>

namespace
{
    const float listenerDepth = 900.f; //TODO test these values
    const float attenuation = 1.f;
    const float minDistance2D = 700.f; //bear in mind listener won't be moving and we want to hear everything on screen
    const float minDistance3D = std::sqrt((minDistance2D * minDistance2D) + (listenerDepth * listenerDepth));

    float volume = 100.f; //static value as it is shared by all sound players
}

SoundPlayer::SoundPlayer()
{

}

//public
void SoundPlayer::update()
{
    //update playing sounds
    for (const auto& p : m_loopedSounds)
    {
        auto pos = p.first->getPosition();
        p.second->setPosition(pos.x, -pos.y, 0.f);
        auto cb = p.first->getCollisionBody();
        if (cb)
        {
            float speed = cb->getSpeed();
            //hmm magic consts ....
            speed = std::max(speed / 330000.f, 0.9f);
            p.second->setPitch(speed);
            //std::cerr << speed << std::endl;
        }
    }

    flushSounds();
}

void SoundPlayer::play(AudioId id)
{
    play(id, getListenerPosition());
}

void SoundPlayer::play(AudioId id, const sf::Vector2f& position, bool loop, Node* owner)
{
    m_sounds.emplace_back();

    auto& sound = m_sounds.back();
    sound.setBuffer(m_buffers[id]);
    sound.setPosition(position.x, -position.y, 0.f);
    sound.setAttenuation(attenuation);
    sound.setMinDistance(minDistance3D);
    sound.setLoop(loop);
    sound.setVolume(volume);
    sound.play();

    if (owner)
    {
        m_loopedSounds.push_back(std::make_pair(owner, &sound));
    }
}

void SoundPlayer::play(AudioId id, const sf::Vector3f& position)
{
    m_sounds.emplace_back();

    auto& sound = m_sounds.back();
    sound.setBuffer(m_buffers[id]);
    sound.setPosition(position.x, -position.y, position.z);
    sound.setAttenuation(attenuation);
    sound.setMinDistance(minDistance3D);
    sound.setVolume(volume);
    sound.play();
}

void SoundPlayer::stop(Node* owner)
{
    assert(owner);

    m_loopedSounds.remove_if([owner](const std::pair<Node*, sf::Sound*>& p)
    {
        if (p.first == owner)
        {
            p.second->stop();
            return true;
        }
        else return false;
    });
}

void SoundPlayer::setListenerPosition(const sf::Vector2f& position)
{
    sf::Listener::setPosition(position.x, -position.y, listenerDepth);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
    auto pos = sf::Listener::getPosition();
    return{ pos.x, -pos.y };
}

void SoundPlayer::cacheSound(AudioId id, const std::string& path)
{
    m_buffers.insert(std::make_pair(id, sf::SoundBuffer()));
    m_buffers[id].loadFromFile(path);
}

void SoundPlayer::setVolume(float vol)
{
    volume = vol;
}

float SoundPlayer::getVolume()
{
    return volume;
}

//private
void SoundPlayer::flushSounds()
{
    m_sounds.remove_if([](const sf::Sound& s){return (s.getStatus() == sf::Sound::Stopped); });
}
