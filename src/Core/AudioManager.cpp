//
// Created by Oier Álvarez on 01/02/2026.
//

#include "Core/AudioManager.h"

#include <iostream>

#include "SFML/Audio/Sound.hpp"

bool AudioManager::loadSound(const std::string& id, const std::string& filePath)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath))
    {
        std::cout << "[Audio] Failed to load: " << filePath << "\n";
        return false;
    }
    std::cout << "[Audio] Loaded: " << id << " from " << filePath << "\n";
    m_buffers[id] = buffer;
    return true;
}

void AudioManager::playSound(const std::string& id, const float volume)
{
    auto iter = m_buffers.find(id);
    if (iter == m_buffers.end())
    {
        std::cout << "[Audio] Sound id not found: " << id << "\n";
        return;
    }

    /* Reuse stopped sounds */
    for (auto& s : m_sounds)
    {
        if (s.getStatus() == sf::Sound::Status::Stopped)
        {
            s.setBuffer(iter->second);
            s.setVolume(volume);
            s.play();
            return;
        }
    }

    /* Create new sound instance */
    m_sounds.emplace_back();
    m_sounds.back().setBuffer(iter->second);
    m_sounds.back().setVolume(volume);
    m_sounds.back().play();
}