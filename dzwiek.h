#ifndef DZWIEK_H
#define DZWIEK_H

#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <cmath>

using namespace sf;
using namespace std;

class Dzwiek
{
private:
    SoundBuffer buffer_serce;
    Sound sound_serce;
    Clock timer_serce;
    float ostatni_interwal_serca = 1.0f;
    bool serce_loaded = false;
    SoundBuffer buffer_chodzenie;
    Sound sound_chodzenie;
    bool is_playing_chodzenie = false;

public:
    Dzwiek() = default;
    bool load_serce_sound(const string& filepath)
    {
        if (!buffer_serce.loadFromFile(filepath))
        {
            cout << "Nie udalo sie zaladowac dzwieku bicia serca: " << filepath << endl;
            return false;
        }
        sound_serce.setBuffer(buffer_serce);
        sound_serce.setLoop(false);
        sound_serce.setVolume(150.f);
        serce_loaded = true;
        return true;
    }
    void update_serce_heartbeat(float distance_to_monster, float max_distance = 1000.0f)
{
    if (!serce_loaded) return;
    float normalized_distance = std::min(distance_to_monster / max_distance, 1.0f);
    float nowy_interwal = 0.3f + (normalized_distance * 1.7f);
    ostatni_interwal_serca = nowy_interwal;
    if (timer_serce.getElapsedTime().asSeconds() >= ostatni_interwal_serca)
    {
        play_serce();
        timer_serce.restart();
    }

    float volume = std::max(20.0f, 120.0f * (1.0f - normalized_distance));
    sound_serce.setVolume(volume);
}

void play_serce()
{
    if (serce_loaded)
    {
        sound_serce.play();
    }
}

void stop_serce()
{
    if (serce_loaded)
    {
        sound_serce.stop();
    }
}

void set_serce_volume(float volume)
{
    if (serce_loaded)
    {
        sound_serce.setVolume(volume);
    }
}

void start_serce_system()
{
    if (serce_loaded)
    {
        timer_serce.restart();
    }
}

float get_current_heartbeat_interval() const
{
    return ostatni_interwal_serca;
}
    bool load_chodzenie_sound(const string& filepath)
    {
        if (!buffer_chodzenie.loadFromFile(filepath))
        {
            cout << "Nie udalo sie zaladowac dzwieku chodzenia: " << filepath << endl;
            return false;
        }
        sound_chodzenie.setBuffer(buffer_chodzenie);
        sound_chodzenie.setLoop(true);
        sound_chodzenie.setVolume(150.f);
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
