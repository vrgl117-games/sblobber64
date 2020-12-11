#include <stdlib.h>
#include <string.h>

#include <mikmod.h>

#include "sounds.h"
#include "dfs.h"

static signed short *buffer;
static int fp = 0;
MIKMODAPI extern UWORD md_mode __attribute__((section(".data")));
MIKMODAPI extern UWORD md_mixfreq __attribute__((section(".data")));

void sound_init()
{
    audio_init(44100, 4);
    buffer = calloc(audio_get_buffer_length() * 2, sizeof(signed short));

    MikMod_RegisterAllDrivers();
    MikMod_RegisterAllLoaders();

    md_mode |= DMODE_16BITS;
    md_mode |= DMODE_SOFT_MUSIC;

    md_mixfreq = audio_get_frequency();
    MikMod_Init("");
}

void sound_start_music()
{
    MODULE *music = Player_Load("rom://sfx/music.mod", 256, 0);
    music->wrap = 1;
    music->fadeout = 1;
    audio_write_silence();
    audio_write_silence();
    Player_Start(music);
}

void sound_start_sfx(char *sound)
{
    dfs_close(fp);
    fp = dfs_openf("/sfx/%s.raw", sound);
}

void sound_pause_music()
{
    if (!Player_Paused())
        Player_TogglePause();
}

void sound_resume_music()
{
    if (Player_Paused())
        Player_TogglePause();
}

// TODO: since we're using libmikmod for music and "raw" for sound effects,
// music will stop while playing sound effects. This is not ideal, see if there is a way to improve.
void sound_update()
{
    if (fp != 0 && audio_can_write())
    {
        memset(buffer, 0, audio_get_buffer_length() * 2 * sizeof(signed short));
        int did_read = dfs_read(buffer, sizeof(signed short), audio_get_buffer_length(), fp);
        did_read = did_read / sizeof(signed short);
        if (dfs_eof(fp))
        {
            dfs_close(fp);
            fp = 0;
        }
        // |a|b|c|d|.|.|.|.|.|.| -> |a|a|b|b|c|c|d|d|.|.|
        for (int i = did_read - 1; i >= 0; i--)
        {
            buffer[(i * 2) + 1] = buffer[i];
            buffer[i * 2] = buffer[i];
        }

        audio_write(buffer);
    }

    MikMod_Update();
}
