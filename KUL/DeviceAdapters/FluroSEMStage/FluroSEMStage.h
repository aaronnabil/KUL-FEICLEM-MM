 //////////////////////////////////////////////////////////////////////////////
// FILE:          FluroSEMStage.h
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   Adapter for PIController
//                Needs accompanying firmware to be installed on the board
// COPYRIGHT:     University of California, San Francisco, 2008
// LICENSE:       LGPL
//
// AUTHOR:        Nico Stuurman, nico@cmp.ucsf.edu, 11/09/2008
//                automatic device detection by Karl Hoover
//
//

#ifndef _FluroSEMStage_H_
#define _FluroSEMStage_H_

#define COM_ERROR -1L
#define PI_CNTR_NO_ERROR  0L
#define PI_CNTR_UNKNOWN_COMMAND 2L

#include "../../MMDevice/MMDevice.h"
#include "../../MMDevice/DeviceBase.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// Error codes
//

//class ArduinoInputMonitorThread;

class CFluroSEMStageHub : public HubBase<CFluroSEMStageHub>  
{
public:
   CFluroSEMStageHub();
   ~CFluroSEMStageHub();

   int Initialize();
   int Shutdown();
   void GetName(char* pszName) const;
   bool Busy();

   MM::DeviceDetectionStatus DetectDevice(void);
   int DetectInstalledDevices();

   // property handlers
   int OnPort(MM::PropertyBase* pPropt, MM::ActionType eAct);

   //int OnLogic(MM::PropertyBase* pPropt, MM::ActionType eAct);
   //int OnVersion(MM::PropertyBase* pPropt, MM::ActionType eAct);

   // custom interface for child devices
   bool IsPortAvailable() {return portAvailable_;}
   //bool IsLogicInverted() {return invertedLogic_;}
   //bool IsTimedOutputActive() {return timedOutputActive_;}
   //void SetTimedOutput(bool active) {timedOutputActive_ = active;}

   
   bool GCSCommandWithAnswer(const std::string command, std::vector<std::string>& answer, int nExpectedLines = -1);
   bool GCSCommandWithAnswer(unsigned char singleByte, std::vector<std::string>& answer, int nExpectedLines = -1);
   bool SendGCSCommand(const std::string command);
   bool SendGCSCommand(unsigned char singlebyte);
   bool ReadGCSAnswer(std::vector<std::string>& answer, int nExpectedLines = -1);
   int GetLastError() const { return lastError_; };



   static MMThreadLock& GetLock() {return lock_;}

private:
   //int GetControllerVersion(int&);
   std::string port_;
   bool initialized_;
   bool portAvailable_;
   int lastError_;
   //bool invertedLogic_;
   //bool timedOutputActive_;
   //int version_;
   static MMThreadLock lock_;
};

class CFluroSEMStageXY : public CXYStageBase<CFluroSEMStageXY>  
{
public:
   CFluroSEMStageXY();
   ~CFluroSEMStageXY();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   
   // Stage API

   virtual double GetStepSize();
   virtual int SetPositionSteps(long x, long y);
   virtual int GetPositionSteps(long &x, long &y);
   virtual int Home();
   virtual int Stop();
   virtual int SetOrigin();
   virtual int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
   virtual int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
   virtual double GetStepSizeXUm();
   virtual double GetStepSizeYUm();
   virtual int SetRelativePositionUm(double dx, double dy);

   int IsXYStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
	 
   // action interface
   // ----------------

private:
   bool initialized_;
   std::string name_;
};

class CFluroSEMStageLR : public CXYStageBase<CFluroSEMStageLR>  
{
public:
   CFluroSEMStageLR();
   ~CFluroSEMStageLR();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   
   // Stage API

   virtual double GetStepSize();
   virtual int SetPositionSteps(long x, long y);
   virtual int GetPositionSteps(long &x, long &y);
   virtual int Home();
   virtual int Stop();
   virtual int SetOrigin();
   virtual int GetLimitsUm(double& xMin, double& xMax, double& yMin, double& yMax);
   virtual int GetStepLimits(long& xMin, long& xMax, long& yMin, long& yMax);
   virtual double GetStepSizeXUm();
   virtual double GetStepSizeYUm();
   virtual int SetRelativePositionUm(double dx, double dy);

   int IsXYStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
	 
   // action interface
   // ----------------
   int CFluroSEMStageLR::OnStepVoltage(MM::PropertyBase* pProp, MM::ActionType eAct);
private:
   bool initialized_;
   std::string name_;
   double _umPerStepUp, _umPerStepDown, _umPerStepLeft, _umPerStepRight;
   double _xPos, _yPos;
};

class CFluroSEMStageZ : public CStageBase<CFluroSEMStageZ>  
{
public:
   CFluroSEMStageZ();
   ~CFluroSEMStageZ();
  
   // MMDevice API
   // ------------
   int Initialize();
   int Shutdown();
  
   void GetName(char* pszName) const;
   bool Busy();
   
   // Stage API

  int SetPositionUm(double pos);
  int GetPositionUm(double& pos);
  int SetRelativePositionUm(double d);
  int SetPositionSteps(long steps);
  int GetPositionSteps(long& steps);
  int SetOrigin();
  int GetLimits(double& min, double& max);

   int IsStageSequenceable(bool& isSequenceable) const {isSequenceable = false; return DEVICE_OK;}
	bool IsContinuousFocusDrive() const {return false;} 

   // action interface
   // ----------------
int CFluroSEMStageZ::OnFineVoltage(MM::PropertyBase* pProp, MM::ActionType eAct);
int CFluroSEMStageZ::OnStepVoltage(MM::PropertyBase* pProp, MM::ActionType eAct);
private:
   bool initialized_;
   std::string name_;
   double _umPerStepUp, _umPerStepDown;
   double _Pos;
};

#endif //_Arduino_H_