#ifndef TIMELINE_HPP
#define TIMELINE_HPP

#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System/Clock.hpp>
#include <mutex>


class Timeline
{
private:
    // std::mutex m;      // if tics can change size and the game is multithreaded
    int64_t start_time; // the time of the *anchor when created
    int64_t elapsed_paused_time;
    int64_t last_paused_time;
    double tic; // units of anchor timeline per step
    bool paused;
    Timeline *anchor; // for most general game time, system library pointer

public:
    Timeline(Timeline *anchor, int64_t tic);
    Timeline();        // optional, may not be included
    int64_t getTime(); // this can be game or system time implementation
    void pause();
    void unpause();
    void changeTic(int tic); // optional
    bool isPaused();         // optional
};

#endif