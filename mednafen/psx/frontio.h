#ifndef __MDFN_PSX_FRONTIO_H
#define __MDFN_PSX_FRONTIO_H

namespace MDFN_IEN_PSX
{

class InputDevice_Multitap;

class InputDevice
{
 public:

 InputDevice();
 virtual ~InputDevice();

 virtual void Power(void);
 virtual void UpdateInput(const void *data);

 virtual void Update(const pscpu_timestamp_t timestamp);
 virtual void ResetTS(void);
 //
 //
 //
 virtual void SetDTR(bool new_dtr);
 virtual bool GetDSR(void);

 virtual bool Clock(bool TxD, int32 &dsr_pulse_delay);

 //
 //
 virtual uint32 GetNVSize(void);
 virtual void ReadNV(uint8 *buffer, uint32 offset, uint32 count);
 virtual void WriteNV(const uint8 *buffer, uint32 offset, uint32 count);

 //
 // Dirty count should be incremented on each call to a method this class that causes at least 1 write to occur to the
 // nonvolatile memory(IE Clock() in the correct command phase, and WriteNV()).
 //
 virtual uint64 GetNVDirtyCount(void);
 virtual void ResetNVDirtyCount(void);
};

class FrontIO
{
 public:

 FrontIO(bool emulate_memcards_[8], bool emulate_multitap_[2]);
 ~FrontIO();

 void Power(void);
 void Write(pscpu_timestamp_t timestamp, uint32 A, uint32 V);
 uint32 Read(pscpu_timestamp_t timestamp, uint32 A);
 int32 CalcNextEvent(int32 next_event);
 pscpu_timestamp_t Update(pscpu_timestamp_t timestamp);
 void ResetTS(void);

 void UpdateInput(void);
 void SetInput(unsigned int port, const char *type, void *ptr);

 uint64 GetMemcardDirtyCount(unsigned int which);
 void LoadMemcard(unsigned int which, const char *path);
 void SaveMemcard(unsigned int which, const char *path); //, bool force_save = false);

 private:

 void DoDSRIRQ(void);
 void CheckStartStopPending(pscpu_timestamp_t timestamp, bool skip_event_set = false);

 void MapDevicesToPorts(void);

 bool emulate_memcards[8];
 bool emulate_multitap[2];

 InputDevice *Ports[2];
 InputDevice *MCPorts[2];

 InputDevice *DummyDevice;
 InputDevice_Multitap *DevicesTap[2];

 InputDevice *Devices[8];
 void *DeviceData[8];

 InputDevice *DevicesMC[8];

 //
 //
 //

 int32 ClockDivider;

 bool ReceivePending;
 bool TransmitPending;

 bool ReceiveInProgress;
 bool TransmitInProgress;

 bool ReceiveBufferAvail;

 uint8 ReceiveBuffer;
 uint8 TransmitBuffer;

 int32 ReceiveBitCounter;
 int32 TransmitBitCounter;

 uint16 Mode;
 uint16 Control;
 uint16 Baudrate;


 bool istatus;
 //
 //
 int32 dsr_pulse_delay[4];
 int32 dsr_active_until_ts[4];
 int32 lastts;
};

extern InputInfoStruct FIO_InputInfo;

}
#endif
