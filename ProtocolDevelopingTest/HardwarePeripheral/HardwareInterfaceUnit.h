#ifndef HARDWARE_INTERFACE_UNIT_H
#define HARDWARE_INTERFACE_UNIT_H

int Write(void);
int Recv(void);
int SentInterrupt(void* arg); //TransmitInterrupt();
int ReceiveInterrupt(void* arg);


#endif // !HARDWARE_INTERFACE_UNIT_H