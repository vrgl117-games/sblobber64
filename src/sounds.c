#include <stdlib.h>
#include <string.h>

#include "sounds.h"
#include "dfs.h"

static signed short *buffer;
static int fp;
static bool paused = false;

void sound_init()
{
    audio_init(44100, 4);
    buffer = calloc(audio_get_buffer_length() * 2, sizeof(signed short));
    audio_write_silence();
}

void sound_start(char *sound)
{
    fp = dfs_openf("/sfx/%s.raw", sound);
    paused = false;
}

void sound_stop()
{
    dfs_close(fp);
    fp = 0;
    audio_write_silence();
    audio_write_silence();
    audio_write_silence();
    audio_write_silence();
}

void sound_play_pause()
{
    paused = !paused;
}

void sound_update()
{
    if (!paused && fp != 0 && audio_can_write())
    {
        memset(buffer, 0, audio_get_buffer_length() * 2 * sizeof(signed short));
        int did_read = dfs_read(buffer, sizeof(signed short), audio_get_buffer_length(), fp);
        did_read = did_read / sizeof(signed short);
        if (dfs_eof(fp))
            sound_stop();
        // |a|b|c|d|.|.|.|.|.|.| -> |a|a|b|b|c|c|d|d|.|.|
        for (int i = did_read - 1; i >= 0; i--)
        {
            buffer[(i * 2) + 1] = buffer[i];
            buffer[i * 2] = buffer[i];
        }
        // |a|a|b|b|c|c|d|d|.|.| -> |a|a|b|b|c|c|d|d|0|0|
        //for (int i = did_read; i < buffer_length * 2; i++)
        //    buffer[i] = 0;
        if (audio_can_write())
            audio_write(buffer);
    }
}
