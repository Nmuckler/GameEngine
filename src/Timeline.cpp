#include "Timeline.hpp"

/**
 * https://stackoverflow.com/questions/16177295/get-time-since-epoch-in-milliseconds-preferably-using-c11-chrono
 * gave the idea for
 * “std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()""
 */

Timeline::Timeline() : anchor(nullptr), tic(2), paused(false), start_time(0), elapsed_paused_time(0), last_paused_time(0)
{
}
Timeline::Timeline(Timeline *anchor, int64_t tic) : anchor(anchor), tic(tic), paused(false), start_time(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()), elapsed_paused_time(0), last_paused_time(0)
{
}

int64_t Timeline::getTime()
{
    // std::unique_lock<std::mutex> cv_lock(*m);

    // if anchor is not null get the time in reference to that
    int64_t current_time = anchor ? anchor->getTime() : std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); // Get time from the anchor timeline if available
    int64_t alt_current_time = (current_time - start_time) / tic;

    // if (paused)
    // {
        // Calculate the total elapsed paused time in units of this timeline
        int64_t total_elapsed_paused_time = elapsed_paused_time / tic;

        // Subtract the elapsed paused time from the current time
        alt_current_time -= total_elapsed_paused_time;
    // }

    return alt_current_time;
}

void Timeline::pause()
{
    // std::unique_lock<std::mutex> cv_lock(m);

    if (!paused)
    {
        paused = true;
        // last paused time is this current time
        last_paused_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
}

void Timeline::unpause()
{
    // std::unique_lock<std::mutex> cv_lock(m);

    if (paused)
    {
        paused = false;
        int64_t current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        // ela
        elapsed_paused_time += (current_time - last_paused_time);
    }
}

void Timeline::changeTic(int newTic)
{
    // std::unique_lock<std::mutex> cv_lock(m);

    tic = newTic;
}

bool Timeline::isPaused()
{
    // std::unique_lock<std::mutex> cv_lock(m);

    return paused;
}