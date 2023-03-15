#ifndef TIMEOUT_HANDLER
#define TIMEOUT_HANDLER

class TimeoutHandler
{
public:
  TimeoutHandler(uint64_t timeoutMs, bool writeError = true);
  void start();
  bool hasTimeout();
  void reset();

private:
  uint64_t _startTime;
  uint64_t _endTime;
  uint64_t _timeoutMs;
  bool _writeError;
};

#endif // TIMEOUT_HANDLER
