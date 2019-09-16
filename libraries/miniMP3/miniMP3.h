#ifndef MINIMP3_h
#define MINIMP3_h

#include "Arduino.h"

static uint8_t send_buf[10] = {0x7E, 0xFF, 06, 00, 00, 00, 00, 00, 00, 0xEF};
HardwareSerial *_hardware_serial;
int _busyPIN;

void mp3_set_serial (HardwareSerial &theSerial, int busyPIN) {
  _hardware_serial = &theSerial;
  _hardware_serial->begin(9600);
  _busyPIN = busyPIN;
  delay(10);
}

void h_send_func () {
  for (int i=0; i<10; i++) {
    _hardware_serial->write (send_buf[i]);
  }
}

uint16_t mp3_get_checksum (uint8_t *thebuf) {
  uint16_t sum = 0;
  for (int i=1; i<7; i++) {
    sum += thebuf[i];
  }
  return -sum;
}

void fill_uint16_bigend (uint8_t *thebuf, uint16_t data) {
  *thebuf = (uint8_t)(data>>8);
  *(thebuf+1) = (uint8_t)data;
}

void mp3_fill_checksum () {
  uint16_t checksum = mp3_get_checksum (send_buf);
  fill_uint16_bigend (send_buf+7, checksum);
}

void mp3_send_cmd (uint8_t cmd, uint16_t arg) {
  send_buf[3] = cmd;
  fill_uint16_bigend ((send_buf+5), arg);
  mp3_fill_checksum ();
  h_send_func ();
}

void mp3_play (uint16_t num) {
  while(!digitalRead(_busyPIN));
  delay(50);
  mp3_send_cmd (0x12, num);
  while(!digitalRead(_busyPIN));
  while(digitalRead(_busyPIN));
}

void mp3_set_volume (uint16_t volume) {
  mp3_send_cmd (0x06, volume);
}

#endif