#include <signal.h>
#include <thread>
#include <future>
#include <iostream>

#include "defs.hpp"
#include "cryo_control.hpp"
#include "console_ui.hpp"
#include "mock_temp_sensor.hpp"
#include "rpiPWM1.h"

std::atomic_bool terminate_flag{false}; // TODO: remove this

// need to set SIGNAL handlers..
void sig_handler(int s)
{
    terminate_flag = true;
}

// create and detach control interface thread(s) and start main control loop
int main(int argc, char ** argv)
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = sig_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, nullptr);

    std::shared_ptr<cryo_control> cryo = std::make_shared<cryo_control>(std::make_shared<mock_temp_sensor>(), std::make_shared<pwm_control>());
    std::shared_ptr<console_ui> console = std::make_shared<console_ui>(cryo);
    cryo->register_ui_observer(console);

    auto console_thread = std::async(std::launch::async, &console_ui::console_task, std::ref(*console));
    console_thread.get();

    std::cout << "cryo process terminated\n" << std::flush;

    return 0;
}