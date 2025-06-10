#ifndef DZWIEK_H
#define DZWIEK_H

#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class Dzwiek
{
private:
    SoundBuffer buffer_chodzenie;
    Sound sound_chodzenie;
    bool is_playing_chodzenie = false;
public:
    Dzwiek() = default;
    bool load_chodzenie_sound(const string& filepath)
    {
        if (!buffer_chodzenie.loadFromFile(filepath))
        {
            cout << "Nie udalo sie zaladowac dzwieku chodzenia: " << filepath << endl;
            return false;
        }
        sound_chodzenie.setBuffer(buffer_chodzenie);
        sound_chodzenie.setLoop(true);
        sound_chodzenie.setVolume(50.f);
        return true;
    }
    void start_chodzenie()
    {
        if (!is_playing_chodzenie)
        {
            sound_chodzenie.play();
            is_playing_chodzenie = true;
        }
    }

    void stop_chodzenie()
    {
        if (is_playing_chodzenie)
        {
            sound_chodzenie.stop();
            is_playing_chodzenie = false;
        }
    }
    bool is_chodzenie_playing() const
    {
        return is_playing_chodzenie;
    }

    void set_chodzenie_volume(float volume)
    {
        sound_chodzenie.setVolume(volume);
    }
};

#endif
