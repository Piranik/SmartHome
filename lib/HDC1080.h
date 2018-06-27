/*
 * HDC1080 library.
 * Get temperature and humidity.
 * It works via I2C Wire object. It needs to init it first. ESP-8266 tested.
 *
 * (c) Oleg Moiseenko 2018
 */

#ifndef HDC1080_H
#define HDC1080_H

#include <Arduino.h>
#include <pitimer.h>
#include <etools.h>
#include <xlogger.h>            // logger https://github.com/merlokk/xlogger

#include <xmqtt.h>

#include <Wire.h>
#include <ClosedCube_HDC1080.h> // original: https://github.com/closedcube/ClosedCube_HDC1080_Arduino
                                // with error handling fix: https://github.com/merlokk/ClosedCube_HDC1080_Arduino

#define HDC_DEBUG

// poll
#define MILLIS_TO_POLL          10*1000       // max time to wait for poll
#define MILLIS_TO_SET_TIME      10*60*60*1000 // settime interval 12 hours
#define MILLIS_TIMEOUT          700           // AZ response timeout

// timers
#define TID_POLL                0x0001        // timer UID for poll
#define TID_SET_TIME            0x0002        // timer UID for set time
#define TID_TIMEOUT             0x0003        // timer UID for response timeout

class hdc1080 {
public:
  hdc1080();

  void begin(xLogger *_logger);
  void handle();
  void SetLogger(xLogger * _logger);
  void SetMQTT(xMQTT *_mqtt, String _topicOnline, String _topicT, String _topicH, String _topicHeater);
  uint8_t Reset();

  bool Connected();

  String GetTextIDs() const;
  float GetTemperature() const;
  float GetHumidity() const;

private:
  xLogger *logger = NULL;
  piTimer atimer;
  xMQTT *amqtt = NULL;

  ClosedCube_HDC1080 hdc;

  bool aConnected = false;
  // string ID
  String TextIDs;
  // readed registers
  HDC1080_Registers reg;

  // decoded measurements
  float Temperature;
  float Humidity;

  void SensorInit();

  template <typename... Args>
  void DEBUG_PRINTLN(Args... args);
};

template <typename... Args>
void hdc1080::DEBUG_PRINTLN(Args... args) {
#ifdef HDC_DEBUG
  if (logger) {
    logger->println(args...);
  }
#endif
}

#endif // HDC1080_H