#include "serial_comm.hpp"

#include <termios.h>
#include <iostream>
#include <cstring>


int main() {
  // Replace with your serial port name
  const char* portname = "/dev/ttyS1";
  int fd = openSerialPort(portname);
  if (fd < 0)
    return 1;

  if (!configureSerialPort(fd, B9600)) {
    closeSerialPort(fd);
    return 1;
  }

  const char* message = "Hello, Serial Port!";
  if (writeToSerialPort(fd, message, strlen(message)) < 0) {
    std::cerr << "Error writing to serial port: " << strerror(errno) << std::endl;
  }

  char buffer[100];
  int n = readFromSerialPort(fd, buffer, sizeof(buffer));
  if (n < 0) {
    std::cerr << "Error reading from serial port: " << strerror(errno) << std::endl;
  } else {
    std::cout << "Read from serial port: " << std::string(buffer, n) << std::endl;
  }

  closeSerialPort(fd);
  return 0;
}
