#include "audio.hpp"

audio_player::audio_player ()
: file_name ("")
, child_pid ((pid_t)0) {
}

audio_player::audio_player (std::string file_name)
: file_name (file_name)
, child_pid ((pid_t)0) {
}

void audio_player::set_file (std::string file_name) {
    this->file_name = file_name;
}

void audio_player::play (unsigned int time /*= 0*/) {
    child_pid = fork ();
    switch (child_pid) {
        case -1: // error
            std::perror ("failed to start audio player");
            break;

        case 0: // child process
            call_player (time);
            exit (1);

        default: // parent process, pid now contains the child pid
                 ;
    }
    return;
}

void audio_player::call_player (unsigned int time) {
    std::string time_str = std::to_string (time);
#ifdef DEBUG_AUDIO
    execlp ("ogg123", "ogg123", "-q", "-k", time_str.c_str (),
    file_name.c_str (), (char*)NULL);
#else
    execlp ("ogg123", "ogg123", "-k", time_str.c_str (), file_name.c_str (), (char*)NULL);
#endif
    std::perror ("error executing ogg123");
}

int audio_player::stop () {
    return kill (child_pid, SIGKILL);
}

void audio_player::set_volume (unsigned int volume) {
    int status;
    pid_t pid = fork ();
    switch (pid) {
        case -1: // error
            std::perror ("failed to start audio player");
            break;

        case 0: // child process
            call_mixer (volume);
            exit (1); // if fails, exit the process

        default: // parent process, pid now contains the child pid
                 ;
    }
}

void audio_player::call_mixer (unsigned int volume) {
    std::string vol_str = std::to_string (volume) + "%";
#ifdef DEBUG_AUDIO
    execlpx ("amixer", "amixer", "sset", "Master", vol_str.c_str (), (char*)NULL);
#else
    execlpx ("amixer", "amixer", "sset", "Master", vol_str.c_str (), (char*)NULL);
#endif
    std::perror ("error executing amixer");
}