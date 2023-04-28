#include <chrono>
#include <iostream>
#include <map>
#include <thread>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Metronome {
public:
    Metronome();
    void play();
    void set_tempo(int tempo);
    void set_volume(int volume);
    void set_subdivision(int subdivision);
    void set_time_signature(std::pair<int, int> time_signature);
    void add_custom_pattern(std::string name, std::vector<int> pattern);
    void remove_custom_pattern(std::string name);
    json get_preset_data(std::string name);
    void add_preset(std::string name, int tempo, int volume, int subdivision, std::pair<int, int> time_signature, std::vector<int> pattern);
    void remove_preset(std::string name);
    void load_presets();
    void save_presets();
private:
    void _vibrate();
    int tempo;
    int volume;
    int subdivision;
    std::pair<int, int> time_signature;
    std::vector<int> pattern;
    std::map<std::string, std::vector<int>> custom_patterns;
    std::map<std::string, json> preset_data;
};

Metronome::Metronome() {
    tempo = 120;
    volume = 50;
    subdivision = 2;
    time_signature = std::make_pair(4, 4);
    pattern = std::vector<int>(subdivision, 1);
    load_presets();
}

void Metronome::play() {
    while (true) {
        for (int i = 0; i < subdivision; ++i) {
            if (pattern[i]) {
                _vibrate();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(60000 / tempo / subdivision));
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(60000 / tempo / subdivision));
    }
}

void Metronome::_vibrate() {
    double duration = 0.05;
    double interval = 0.05;
    for (int i = 0; i < pattern.size(); ++i) {
        if (pattern[i]) {
            if (i == 0) {
                // accent the beat
                // red color
                // TODO: send vibration signal
                duration *= 2;
            } else {
                // non-accented beat
                // white color
                // TODO: send vibration signal
            }
        } else {
            // no vibration
        }
        // sleep for the interval time
        std::this_thread::sleep_for(std::chrono::milliseconds(interval * 1000));
    }
}

void Metronome::set_tempo(int tempo) {
    this->tempo = tempo;
}

void Metronome::set_volume(int volume) {
    this->volume = volume;
}

void Metronome::set_subdivision(int subdivision) {
    this->subdivision = subdivision;
    pattern = std::vector<int>(subdivision, 1);
}

void Metronome::set_time_signature(std::pair<int, int> time_signature) {
    this->time_signature = time_signature;
}

void Metronome::add_custom_pattern(std::string name, std::vector<int> pattern) {
    custom_patterns[name] = pattern;
}

void Metronome::remove_custom_pattern(std::string name) {
    if (custom_patterns.count(name)) {
        custom_patterns.erase(name);
    }
}

json Metronome::get_preset_data(std::string name) {
    if (this->preset_data.count(name) > 0) {
        return this->preset_data[name];
    }
    else {
        return json::object();
    }
}

void Metronome::add_preset(std::string name, int tempo, int volume, int subdivision, std::pair<int, int> time_signature, std::vector<int> pattern) {
    this->preset_data[name] = {
        {"tempo", tempo},
        {"volume", volume},
        {"subdivision", subdivision},
        {"time_signature", time_signature},
        {"pattern", pattern}
    };
}

void Metronome::remove_preset(std::string name) {
    if (this->preset_data.count(name) > 0) {
        this->preset_data.erase(name);
    }
}

void Metronome::load_presets() {
    std::ifstream file("presets.json");
    if (file.good()) {
        file >> this->preset_data;
    }
}

void Metronome::save_presets() {
    std::ofstream file("presets.json");
    file << std::setw(4) << this->preset_data << std::endl;
}

void Metronome::play() {
    while (true) {
        for (int i = 0; i < this->subdivision; i++) {
            if (this->pattern[i]) {
                this->_vibrate();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(60000.0 / (this->tempo * this->subdivision))));
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(60000.0 / this->tempo)));
    }
}

void Metronome::_vibrate() {
    double duration = 0.05; // vibration duration in seconds
    double interval = 0.05; // time between vibrations in seconds
    for (size_t i = 0; i < this->pattern.size(); i++) {
        if (this->pattern[i]) {
            // accent the beat
            std::tuple<int, int, int, int> vibration;
            if (i == 0) {
                vibration = std::make_tuple(255, 0, 0, this->volume); // red color
            } else {
                vibration = std::make_tuple(255, 255, 255, this->volume); // white color
            }
            duration *= 2;
            // send the vibration signal
            send_vibration(vibration, duration);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(interval * 1000.0)));
        } else {
            // non-accented beat
            std::tuple<int, int, int, int> vibration = std::make_tuple(0, 0, 0, 0); // no vibration
            // send the vibration signal
            send_vibration(vibration, duration);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(interval * 1000.0)));
        }
    }
}

void Metronome::set_tempo(int tempo) {
    /**
     * Set the tempo of the metronome.
     *
     * @param tempo The new tempo (in beats per minute).
     */
    this->tempo = tempo;
}

void Metronome::set_volume(int volume) {
    /**
     * Set the volume of the metronome.
     *
     * @param volume The new volume (as a percentage).
     */
    this->volume = volume;
}

void Metronome::set_subdivision(int subdivision) {
    /**
     * Set the subdivision of the metronome.
     *
     * @param subdivision The new subdivision (e.g. 2 for eighth notes, 3 for triplets, etc.).
     */
    this->subdivision = subdivision;
    this->pattern = std::vector<bool>(subdivision, true);
}

void Metronome::set_time_signature(std::tuple<int, int> time_signature) {
    /**
     * Set the time signature of the metronome.
     *
     * @param time_signature The new time signature (as a tuple of two integers, e.g. (4, 4) for 4/4 time).
     */
    this->time_signature = time_signature;
}

void Metronome::add_custom_pattern(std::string name, std::vector<bool> pattern) {
    /**
     * Add a custom vibration pattern.
     *
     * @param name The name of the pattern.
     * @param pattern The pattern data (a vector of booleans).
     */
    this->custom_patterns[name] = pattern;
}

void Metronome::remove_custom_pattern(std::string name) {
    /**
     * Remove a custom vibration pattern.
     *
     * @param name The name of the pattern to remove.
     */
    if (this->custom_patterns.count(name)) {
        this->custom_patterns.erase(name);
    }
}

void Metronome::add_custom_pattern(std::string name, std::vector<bool> pattern) {
    /**
     * Add a custom vibration pattern.
     *
     * @param name The name of the pattern.
     * @param pattern The pattern data (a vector of booleans).
     */
    this->custom_patterns[name] = pattern;
}

void Metronome::add_preset(std::string name, int tempo, int volume, int subdivision, std::pair<int, int> time_signature, std::vector<bool> pattern) {
    /**
     * Add a preset to the metronome.
     *
     * @param name The name of the preset.
     * @param tempo The tempo of the preset (in beats per minute).
     * @param volume The volume of the preset (as a percentage).
     * @param subdivision The subdivision of the preset (e.g. 2 for eighth notes, 3 for triplets, etc.).
     * @param time_signature The time signature of the preset (as a tuple of two integers, e.g. (4, 4) for 4/4 time).
     * @param pattern The vibration pattern of the preset (a vector of booleans).
     */
    this->preset_data[name] = {
        {"tempo", tempo},
        {"volume", volume},
        {"subdivision", subdivision},
        {"time_signature", time_signature},
        {"pattern", pattern}
    };
}

void Metronome::remove_preset(std::string name) {
    /**
     * Remove a preset from the metronome.
     *
     * @param name The name of the preset to remove.
     */
    if (this->preset_data.count(name) > 0) {
        this->preset_data.erase(name);
    }
}

