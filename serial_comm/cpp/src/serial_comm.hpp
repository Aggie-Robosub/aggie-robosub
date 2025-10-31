#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

int openSerialPort(const char* portname);
bool configureSerialPort(int fd, int speed);
int readFromSerialPort(int fd, char* buffer, size_t size);
int writeToSerialPort(int fd, const char* buffer, size_t size);
void closeSerialPort(int fd);

#endif
