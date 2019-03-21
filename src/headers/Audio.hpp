///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Audio.hpp
///////////////////////////////////////////////////////////////////////////////////////////////////
/// Description
///////////////////////////////////////////////////////////////////////////////////////////////////
/// Contains easy-to-use functions for handling and controlling sound.
///
/// This library stores information for each entry in a struct.
/// It checks whether or not files already exist, and can even load files during runtime.
/// It uses SDL2_mixer as it's backend, and thus it loads/plays files on a seperate thread,
/// so that it doesn't freeze the main thread.
///
/// The "name" parameter for AddBGM, PlayBGM AddEffect and PlayEffect functions can be anything,
/// as long as they are the same in Add and Play. Using AddBGM("bgm1.wav", "bgm1", somebool) and
/// PlayBGM("forrest_bgm") will not work.
///
/// When using the PlayBGM function, you can either send only the name, or both the name and
/// and an int as paramaters. The int tells it how many times the bgm will be played.
/// Note: you can send -1 as a paramter and it will loop until stopped or bgm.
///
/// Default number of soundeffect channels: 4
///
///
/// Note: This requires both the SDL2 library, and SDL_mixer library!
/// Link to SDL2:       https://www.libsdl.org/download-2.0.php
/// Link to SDL_mixer:  https://www.libsdl.org/projects/SDL_mixer/
///
/// Note: Currently works with .wav files cross platform.
/// Note: For other files, check SDL_mixer documentation.
///
///////////////////////////////////////////////////////////////////////////////////////////////////
/// Example of usage:
/// #include "Audio.hpp"
///
/// bool dynamic_load = true;
///
/// bool LoadMap() {
///     if (Audio::AddBGM("dessert_bgm.wav", "bgm1", dynamic_load))
///     {
///         std::cout << "Error adding song!\n";
///         return false;
///     }
///     return true;
/// }
///
/// void StartDessertMap()
/// {
///     Audio::PlayBGM("bgm1", -1);
///
///     GameLoop();
/// }
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>


#include <SDL2_mixer/include/SDL_mixer.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
/// Variables
///////////////////////////////////////////////////////////////////////////////////////////////////
/// Used internally, DO NOT CHANGE!
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \brief
///     Internal used struct
///
/// \details
///     Should not be used anywhere else.
///     Used to store variables for files.
///
/// \param name         For user's internal naming
/// \param path         File path
/// \param position     Position in files vector
/// \param dynamic_load Load during runtime or not
///
///////////////////////////////////////////////////////////
struct List
{
    std::string name;
    std::string path;
    int position;
    bool dynamic_load;
};

#ifndef AUDIO_IMPLEMENTATION
extern std::vector<List> bgm_list;
extern std::vector<List> effect_list;
extern std::vector<int> channels;
extern std::vector<Mix_Chunk*> files;

extern Mix_Chunk* bgm;
extern bool dynamic_loaded;
extern int bgm_volume;
extern int effect_volume;
#else
std::vector<List> bgm_list;
std::vector<List> effect_list;
std::vector<int> channels;
std::vector<Mix_Chunk*> files;

Mix_Chunk* bgm;
bool dynamic_loaded;
int bgm_volume;
int effect_volume;
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
/// \namespace
namespace Audio
{
    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Initialization of SDL_mixer and
    ///     standard values.
    ///
    /// \details
    ///     Should be used during initial
    ///     start of the program before
    ///     use of other functions.
    ///     Should always be used after
    ///     SDL_Init();
    ///
    /// \return true if successful.
    ///////////////////////////////////////////////////////////
    inline bool InitAudio()
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) return false;
        bgm_volume = 128;
        effect_volume = 128;
        for (int i = 0; i < 5; i++) channels.push_back(0);
        return true;
    }


    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Sets a number of channels to be
    ///     used for soundeffects.
    ///
    /// \details
    ///     Default channels for soundeffects
    ///     is 4. Maximum number of channels
    ///     is based off of when you run out
    ///     of memory (segfault).
    ///
    ///////////////////////////////////////////////////////////
    inline void SetAudioChannels(int num_channels)
    {
        channels.empty();
        channels.push_back(1);
        for (int i = 0; i < num_channels; i++) channels.push_back(0);
        Mix_AllocateChannels(num_channels);
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Adds a file to the bgm_list.
    ///
    /// \details
    ///     Should be used for the longer
    ///     runnning music. Will load
    ///     file into memory when called
    ///     if dynamic_load is false.
    ///     else the song will be loaded.
    ///     when PlayBGM is called.
    ///     Will not add existing file.
    ///
    /// \param path          Filepath of the sound file.
    /// \param name          For user's internal naming.
    /// \param dynamic_load  Load file now or during PlayBGM.
    ///
    /// \return Returns successfull if file was added.
    ///////////////////////////////////////////////////////////
    inline bool AddBGM(const char* path, const std::string& name, bool dynamic_load)
    {
        for (auto& entry : bgm_list)
            if (entry.name == name) return false;

        int file_position = -2;

        if (!dynamic_load)
        {
            files.push_back(Mix_LoadWAV(path));
            file_position = files.size() - 1;

            if (files[file_position] == nullptr)
                return false;
        }

        unsigned int size = bgm_list.size();
        bgm_list.push_back({ name, path, file_position, dynamic_load });

        if (bgm_list.size() == size)
            return false;

        return true;
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Adds a file to the effect_list.
    ///
    /// \details
    ///     Should be used for the shorter
    ///     soundeffects. Will load
    ///     file into memory.
    ///     Will not add existing file.
    ///
    /// \param path          Filepath of the sound file.
    /// \param name          For user's internal naming.
    ///
    /// \return Returns successfull if file was added.
    ///////////////////////////////////////////////////////////
    inline bool AddEffect(const char* path, const std::string& name)
    {
        for (auto& entry : effect_list)
            if (entry.name == name) return false;

        int file_position = -2;


        files.push_back(Mix_LoadWAV(path));
        file_position = files.size() - 1;

        if (files[file_position] == nullptr)
            return false;

        unsigned int size = effect_list.size();
        effect_list.push_back({ name, path, file_position, false });

        if (effect_list.size() == size)
            return false;

        return true;
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Plays a file from bgm_list.
    ///
    /// \details
    ///     Plays a file if it is added
    ///     through AddBGM. Plays once.
    ///     Frees up memory of previous
    ///     played bgm if it was loaded
    ///     during runtime.
    ///
    /// \param name          For user's internal naming.
    ///
    /// \return Returns successfull if file was played.
    ///////////////////////////////////////////////////////////
    inline bool PlayBGM(const std::string& name)
    {
        int file_positon = -1;
        List entry_found;
        for (auto& entry : bgm_list)
            if (entry.name == name)
            {
                entry_found = entry;
                file_positon = entry.position;
            }

        if (file_positon == -1)
            return false;

        Mix_HaltChannel(0);
        channels[0] = 1;
        if (bgm && dynamic_loaded)
            Mix_FreeChunk(bgm);
        bgm = nullptr;

        if (file_positon == -2)
            bgm = Mix_LoadWAV(entry_found.path.c_str());
        else
            bgm = files[file_positon];
        dynamic_loaded = entry_found.dynamic_load;

        if (Mix_PlayChannel(0, bgm, 0) == -1) return false;
        return true;
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Overloaded from PlayBGM(const std::string& name).
    ///
    /// \details
    ///     Plays a file if it is added
    ///     through AddBGM num_loops times.
    ///     If -1 is send as parameter, it
    ///     will loop until stopped manually.
    ///
    /// \param name         For user's internal naming.
    /// \param num_loops    Number of times song is played
    ///
    /// \return Returns successfull if file was played.
    ///////////////////////////////////////////////////////////
    inline bool PlayBGM(const std::string& name, int num_loops)
    {
        int file_positon = -1;
        List entry_found;
        for (auto& entry : bgm_list)
            if (entry.name == name)
            {
                entry_found = entry;
                file_positon = entry.position;
            }

        if (file_positon == -1)
            return false;

        Mix_HaltChannel(0);
        channels[0] = 1;
        if (bgm && dynamic_loaded)
            Mix_FreeChunk(bgm);
        bgm = nullptr;

        if (file_positon == -2)
            bgm = Mix_LoadWAV(entry_found.path.c_str());
        else
            bgm = files[file_positon];
        dynamic_loaded = entry_found.dynamic_load;

        if (Mix_PlayChannel(0, bgm, num_loops) == -1) return false;
        return true;
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Plays a file from effect_list.
    ///
    /// \details
    ///     Plays a file if it is added
    ///     through AddEffect. Plays once.
    ///     If there are no channels available
    ///     no sound is played. Return value
    ///     to be used with ToggleEffectPause()
    ///
    /// \param name         For user's internal naming.
    ///
    /// \return Returns The number of the channel if successful.
    ///////////////////////////////////////////////////////////
    inline int PlayEffect(const std::string& name)
    {
        int file_position = -2;
        for (auto& entry : effect_list)
            if (entry.name == name) file_position = entry.position;

        if (file_position == -2) return -1;

        int empty_channel = -1;
        for (size_t i = channels.size() - 1; i >= 1; i--)
        {
            if (Mix_Playing(i)) channels[i] = 1;
            else
            {
                channels[i] = 0;
                empty_channel = i;
            }
        }

        if (empty_channel == -1) return -1;

        if (Mix_PlayChannel(empty_channel, files[file_position], 0) == -1) return false;
        channels[empty_channel] = 1;
        return empty_channel;
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Stops the bgm currently playing.
    ///
    /// \details
    ///     Frees up memory of bgm played
    ///     if it was loaded during runtime.
    ///
    /// \return Returns successfull if file was stopped.
    ///////////////////////////////////////////////////////////
    inline bool StopBGM()
    {
        Mix_HaltChannel(0);
        if (Mix_Playing(0)) return false;
        if (bgm && dynamic_loaded) Mix_FreeChunk(bgm);
        bgm = nullptr;
        return true;
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Stops all effects currently playing.
    ///
    /// \details
    ///     Loops through and stops all channels
    ///     playing.
    ///
    /// \return Returns successfull if files were stopped.
    ///////////////////////////////////////////////////////////
    inline bool StopEffects()
    {
        for (size_t i = 1; i < channels.size(); i++)
        {
            Mix_HaltChannel(i);
            if (Mix_Playing(i)) return false;
        }
        return true;
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Toggles pause on bgm currently playing.
    ///
    /// \details
    ///     Checks whether anything is playing
    ///     and toggles pause.
    ///
    /// \return Returns successfull if paused.
    ///////////////////////////////////////////////////////////
    inline bool ToggleBGMPause()
    {
        if (Mix_Paused(0))
        {
            Mix_Resume(0);
            return true;
        }
        else
        {
            Mix_Pause(0);
            return false;
        }
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Toggles pause on effect channel.
    ///
    /// \details
    ///     Checks whether anything is playing
    ///     and toggles pause. Use the return
    ///     value from PlayEffect(const std::string& name)
    ///
    /// \param channel  Where the effect is played.
    ///
    /// \return Returns successfull if paused.
    ///////////////////////////////////////////////////////////
    inline bool ToggleEffectPause(int channel)
    {
        if (Mix_Paused(channel))
        {
            Mix_Resume(channel);
            return true;
        }
        else
        {
            Mix_Pause(channel);
            return false;
        }
    }
    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Toggles pause on effect channel.
    ///
    /// \details
    ///     Checks whether anything is playing
    ///     and toggles pause. Use the return
    ///     value from PlayEffect(const std::string& name)
    ///
    /// \param channel  Where the effect is played.
    ///
    /// \return Returns successfull if paused.
    ///////////////////////////////////////////////////////////
    inline bool ToggleEffectPause()
    {
        if (Mix_Paused(channels[1]))
        {
            for (size_t i = 1; i < channels.size(); i++)
                Mix_Resume(i);
            return true;
        }
        else
        {
            for (size_t i = 1; i < channels.size(); i++)
                Mix_Pause(i);
            return false;
        }
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Volume control for bgm.
    ///
    /// \details
    ///     Volume scales between 0-128.
    ///
    /// \return Returns the new volume.
    ///////////////////////////////////////////////////////////
    inline int IncreaseBGMVolume()
    {
        Mix_Volume(0, bgm_volume + 2);
        return bgm_volume = Mix_Volume(0, -1);
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Volume control for bgm.
    ///
    /// \details
    ///     Volume scales between 0-128.
    ///
    /// \return Returns the new volume.
    ///////////////////////////////////////////////////////////
    inline int DecreaseBGMVolume()
    {
        Mix_Volume(0, bgm_volume - 2);
        return bgm_volume = Mix_Volume(0, -1);
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Volume control for effects.
    ///
    /// \details
    ///     Volume scales between 0-128.
    ///
    /// \return Returns the new volume.
    ///////////////////////////////////////////////////////////
    inline int IncreaseEffectVolume()
    {
        for (size_t i = 1; i < channels.size(); i++)
            Mix_Volume(i, effect_volume + 2);
        return effect_volume = Mix_Volume(1, -1);
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Volume control for effects.
    ///
    /// \details
    ///     Volume scales between 0-128.
    ///
    /// \return Returns the new volume.
    ///////////////////////////////////////////////////////////
    inline int DecreaseEffectVolume()
    {
        for (size_t i = 1; i < channels.size(); i++)
        {
            Mix_Volume(i, effect_volume - 2);
        }
        return effect_volume = Mix_Volume(1, -1);
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Manual volume control for bgm.
    ///
    /// \details
    ///     Volume scales between 0-128.
    ///
    /// \param volume   Volume to be set.
    ///
    /// \return Returns the new volume.
    ///////////////////////////////////////////////////////////
    inline int SetBGMVolume(int volume)
    {
        Mix_Volume(0, volume);
        return bgm_volume = Mix_Volume(0, -1);
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Manual volume control for effects.
    ///
    /// \details
    ///     Volume scales between 0-128.
    ///
    /// \param volume   Volume to be set.
    ///
    /// \return Returns the new volume.
    ///////////////////////////////////////////////////////////
    inline int SetEffectVolume(int volume)
    {
        for (size_t i = 1; i < channels.size(); i++)
            Mix_Volume(i, volume);
        return effect_volume = Mix_Volume(1, -1);
    }

    ///////////////////////////////////////////////////////////
    /// \brief
    ///     Function to free up memory when
    ///     exiting the application.
    ///
    /// \details
    ///     Stops every channel and releases memory
    ///     used from loading files. Should
    ///     always be called before SDL_Quit();
    ///
    ///////////////////////////////////////////////////////////
    inline void QuitAudio()
    {
        StopBGM();
        StopEffects();
        for (size_t i = 0; i < files.size(); i++)
            Mix_FreeChunk(files[i]);
    }
}
