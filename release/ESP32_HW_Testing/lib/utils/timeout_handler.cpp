#include <Arduino.h>
#include <ArduinoLog.h>
#include "timeout_handler.h"

/// @brief create handler for timeout functionality
/// @param timeoutMs time in ms after which the timeout is reached
/// @param writeError if true, error will be logged when timeout is reached (default = true)
TimeoutHandler::TimeoutHandler(uint64_t timeoutMs, bool writeError)
{
    _timeoutMs = timeoutMs;
    _startTime = 0;
    _endTime = 0;
    _writeError = writeError;
}

/// @brief start the timer for timeout calculation
void TimeoutHandler::start()
{
    _startTime = millis();
    _endTime = _startTime + _timeoutMs;
}

/// @brief check if timeout was reached
/// @return true if timeout was reached, false if timeout not reached yet
bool TimeoutHandler::hasTimeout()
{
    uint64_t currentTime = millis();
    if (currentTime >= _endTime)
    {
        if (_writeError)
        {
            Log.errorln("timeout (%dms) reached.", _timeoutMs);
        }

        return true;
    }

    return false;
}

/// @brief reset starttime and endtime, timeout in ms stays the same as defined in ctor
void TimeoutHandler::reset()
{
    _startTime = 0;
    _endTime = 0;
}
