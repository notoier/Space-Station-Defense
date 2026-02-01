//
// Created by Oier Álvarez on 01/02/2026.
//

#ifndef SPACESTATIONDEFENSE_AUDIOMANAGER_H
#define SPACESTATIONDEFENSE_AUDIOMANAGER_H
#include <string>
#include <unordered_map>
#include <vector>
#include <SFML/Audio/Sound.hpp>
#include "SFML/Audio/SoundBuffer.hpp"


class AudioManager
{
public:

    bool loadSound(const std::string& id, const std::string& filePath);
    void playSound(const std::string& id, float volume = 100.f);

private:
    std::unordered_map<std::string, sf::SoundBuffer> m_buffers;
    std::vector<sf::Sound> m_sounds;
};

#endif //SPACESTATIONDEFENSE_AUDIOMANAGER_H