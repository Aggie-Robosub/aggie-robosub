// Make as a class, so you can open multiple serials?

/*
Tutorials
https://www.geeksforgeeks.org/cpp/serial-port-connection-in-cpp/
https://tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html
https://www.man7.org/linux/man-pages/man3/termios.3.html
https://pubs.opengroup.org/onlinepubs/7908799/xsh/termios.h.html
https://en.wikibooks.org/wiki/Serial_Programming/termios#Example_terminal_program
*/


#include "serial_comm.hpp"

#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream> // Only used for testing
#include <termios.h>
#include <unistd.h>


/**
 * Open serial port with given name
 */
int openSerialPort(const char* portname) {
  int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);

  if (fd < 0) {
    std::cerr << "Error opening " << portname << ": " << strerror(errno) << std::endl;
    return -1;
  }

  return fd;
}

/**
 * Configure serial port with fd and speed
 */
bool configureSerialPort(int fd, int speed) {
  struct termios tty;

  if (tcgetattr(fd, &tty) != 0) {
    std::cerr << "Error from tcgetattr: " << strerror(errno) << std::endl;
    return false;
  }
  
  cfsetospeed(&tty, speed);
  cfsetispeed(&tty, speed);

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit characters
  tty.c_iflag &= ~IGNBRK; // disable break processing
  tty.c_lflag = 0; // no signaling chars, no echo, no
                    // canonical processing
  tty.c_oflag = 0; // no remapping, no delays
  tty.c_cc[VMIN] = 0; // read doesn't block
  tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

  tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls, enable reading
  tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

  if (tcsetattr(fd, TCSANOW, &tty) != 0) {
    std::cerr << "Error from tcsetattr: " << strerror(errno) << std::endl;
    return false;
  }

  return true;
}

/**
 * Read data from the serial port
 */
int readFromSerialPort(int fd, char* buffer, size_t size)
{
  return read(fd, buffer, size);
}

/**
 * Write data from the serial port
 */
int writeToSerialPort(int fd, const char* buffer, size_t size)
{
  return write(fd, buffer, size);
}

/**
 * Close the serial port
 */
void closeSerialPort(int fd) { close(fd); }
