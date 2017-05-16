#ifndef AZ7798_H
#define AZ7798_H

#include <Arduino.h>
#include <TimeLib.h>        // https://github.com/PaulStoffregen/Time
#include <NtpClientLib.h>   // https://github.com/gmag11/NtpClient
#include <pitimer.h>
#include <etools.h>
#include <xlogger.h>        // logger https://github.com/merlokk/xlogger

#define AZ_DEBUG

// poll
#define MILLIS_TO_POLL          15*1000       // max time to wait for poll
#define MILLIS_TO_SET_TIME      30*60*1000    // settime interval
#define MILLIS_TIMEOUT          700           // AZ response timeout
// timers
#define TID_POLL                0x0001        // timer UID for poll
#define TID_SET_TIME            0x0002        // timer UID for set time
#define TID_TIMEOUT             0x0003        // timer UID for response timeout

#define TIMESTAMP_01_01_2000    946684800     // start time of AZ time.  01/01/2000 @ 12:00am (UTC)

enum AZState {
  asInit         = 0x00,
  asWait         = 0x01,
  asSentCommand  = 0x02,
  asGotResponse  = 0x03,
  asTimeout      = 0x04
};

enum AZProcessCommands {
  acNone             = 0x00,
  acGetVersion       = 0x01,
  acGetDateTime      = 0x02,
  acSetDateTime      = 0x03,
  acGetMeasurements  = 0x04
};

class az7798 {
public:
  az7798();

  void begin(Stream *_serial = NULL, xLogger *_logger = NULL);
  void handle();
  void SetLogger(xLogger * _logger);
  void SetSerial(Stream *_serial);

  //last connect to AZ
  int LastGetMeasurements = -MILLIS_TO_POLL * 4;
  bool Connected();

  String GetVersion() const;
  String GetMeasurements() const;
  float GetTemperature() const;
  float GetHumidity() const;
  int GetCO2() const;

private:
  xLogger *logger = NULL;
  Stream  *serial = NULL;
  piTimer atimer;

  AZState state = asInit;
  AZProcessCommands processingCommand = acNone;
  String responseBuffer;

  // strings from AZ
  String Version;
  String DateTime;
  bool SetDateTime;
  String Measurements;
  // decoded measurements
  float Temperature;
  float Humidity;
  int CO2;

  void SendCommand(AZProcessCommands cmd);
  void ProcessCommand(AZProcessCommands cmd);
  void ExtractMeasurements();

  template <typename... Args>
  void DEBUG_PRINTLN(Args... args);
};

template <typename... Args>
void az7798::DEBUG_PRINTLN(Args... args) {
#ifdef AZ_DEBUG
  if (logger) {
    logger->println(args...);
  }
#endif
}

#endif // AZ7798_H