#pragma once

#include "control_ui.hpp"

#include <iostream>
#include <chrono>
#include <exception>
#include <thread>
#include <mutex>
#include <map>

//#include <boost/signals2.hpp>

static constexpr std::chrono::milliseconds CONSOLE_UPDATE_INTERVAL{100};

class ncurses_ui : public control_ui
{
    public:

    ncurses_ui() = default;
    virtual ~ncurses_ui() = default;

    void console_loop(); // runs the console

    // updates the values displayed by the console
    void update_sensor_reading(const sensor_temp_t &);
    void update_temp_setting(const temp_reading_t &);

    // send user-specified temperature settings to the cryo controller
    void send_temp_setting_to_contoller();

    private:

    temp_t temp_setting{30};

    std::map<std::string,temp_reading_t> sensor_temp_readings; // sensor_id, reading

    std::mutex temp_lock;

    //std::shared_ptr<cryo_controller> cryo;
};
