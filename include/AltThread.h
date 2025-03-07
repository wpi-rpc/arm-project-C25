#include <functional>
#include <vector>

#ifndef ALT_THREAD_H
#define ALT_THREAD_H

/**
 * @brief Handles running alternative non-blocking threaded logic on alternative Pico W multicore processor. 
 */
class AltThread {
    private:

    // registered functions for the alternative thread on Pico W multicore
    static std::vector<std::function<void()>> threads;

    public: 

    // robot loop delay in millis
    static const int DELAY_MILLIS = 5;

    /**
     * @brief Appends a specified function to thread on alternative Pico W multicore. 
     * @param thread (std::function<void()>) : The specified function to thread. 
     */
    static void registerThread(std::function<void()> thread);

    /**
     * @brief Pulses number of running threaded actions are running on the alternative Pico W multicore.
     * This method works when iteratively called by pulsing the on-board LED once per threaded action and will
     * repeat every minute at runtime. 
     */
    static void pulse();

    /**
     * @brief Initializes the Pico W multicore to begin running registered alternative non-blocking threads. 
     */
    static void init();
};

#endif