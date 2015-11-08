/*----------------------------------------------------------------------------------------
 Constant Definitions
 ----------------------------------------------------------------------------------------*/
#define __DEBUG__                           (1)
#define __GYROACCEL_ENABLED__               (1)
#if __GYROACCEL_ENABLED__
    #define __GYROACCEL_DMP_ENABLED__       (0)
#endif
#define __MAG_ENABLED__                     (1)
#define __BAROMETER_ENABLED__               (1)

// Arduino Pin configuration
#define PIN_GY86_EXT_INTERRUPT              (13)
#define PIN_RESERVED_12                     (12)
#define PIN_ESC_1                           (11)
#define PIN_ESC_2                           (10)
#define PIN_ESC_3                           (9)
#define PIN_ESC_4                           (8)
#define PIN_RESERVED_07                     (7)
#define PIN_RC_CH1                          (6)
#define PIN_RC_CH2                          (5)
#define PIN_RC_CH3                          (4)
#define PIN_RC_CH4                          (3)
#define PIN_RC_CH5                          (2)
#define PIN_RESERVED_01                     (1)
#define PIN_RESERVED_00                     (0)
#define PIN_CHECK_POWER_STAT                (A0)

// ESC configuration
#define ESC_MIN                             (800)
#define ESC_MAX                             (2200)
#define ESC_TAKEOFF_OFFSET                  (900)
#define ESC_ARM_DELAY                       (1000)

// RC configuration
#define RC_HIGH_CH1                         (1900)
#define RC_LOW_CH1                          (1050)
#define RC_HIGH_CH2                         (1900)
#define RC_LOW_CH2                          (1050)
#define RC_HIGH_CH3                         (1900)
#define RC_LOW_CH3                          (1050)
#define RC_HIGH_CH4                         (1900)
#define RC_LOW_CH4                          (1050)
#define RC_HIGH_CH5                         (1900)
#define RC_LOW_CH5                          (1050)

// PID configuration
#define PITCH_OUTER_P_GAIN                  (4.750)             // angle control
#define PITCH_INNER_P_GAIN                  (2.933)             // rate control
#define PITCH_INNER_I_GAIN                  (0.440)
#define PITCH_INNER_D_GAIN                  (0.335)
#define ROLL_OUTER_P_GAIN                   (4.750)             // angle control
#define ROLL_INNER_P_GAIN                   (2.833)             // rate control
#define ROLL_INNER_I_GAIN                   (0.440)
#define ROLL_INNER_D_GAIN                   (0.335)
#define YAW_P_GAIN                          (2.325)             // yaw -> rate control
#define YAW_I_GAIN                          (0.650)

#define GYRO_FS_PRECISIOM                   (MPU6050_GYRO_FS_250)
#define ACCEL_FS_PRECISIOM                  (MPU6050_ACCEL_FS_2)    //  MPU6050_ACCEL_FS_4  MPU6050_ACCEL_FS_8  MPU6050_ACCEL_FS_16
#define ACCEL_STD_DENOM                     (16384.0f / (1 << ACCEL_FS_PRECISIOM))

// Min & Max Val for Magnetic
#define M_X_MIN                             (-270)    //-654  -693   -688
#define M_X_MAX                             (585)     //185   209    170
#define M_Y_MIN                             (-600)    //-319  -311   -310
#define M_Y_MAX                             (260)     //513   563    546
#define M_Z_MIN                             (-425)    //-363  -374   -377
#define M_Z_MAX                             (285)     //386   429    502

// Flight parameters
#define PITCH_ANG_MIN                       (-30)
#define PITCH_ANG_MAX                       (30)
#define ROLL_ANG_MIN                        (-30)
#define ROLL_ANG_MAX                        (30)
#define YAW_RATE_MIN                        (-20)
#define YAW_RATE_MAX                        (20)

// Offset values
#define PITCH_ANG_OFFSET                    (-4)
#define ROLL_ANG_OFFSET                     (1.6)

#define ROUNDING_BASE                       (50)
#define SAMPLING_TIME                       (0.01)              // seconds

#define X_AXIS                              (0)
#define Y_AXIS                              (1)
#define Z_AXIS                              (2)

/*----------------------------------------------------------------------------------------
 File Inclusions
 ----------------------------------------------------------------------------------------*/
#include <Servo.h>
#include <I2Cdev.h>
#include <PinChangeInt.h>
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#endif

#if __GYROACCEL_ENABLED__
    //#include <MPU6050.h>
    #include <MPU6050_6Axis_MotionApps20.h>
#endif

#if __MAG_ENABLED__
    #include <HMC5883L.h>
#endif

#if __BAROMETER_ENABLED__
    #include <MS5611.h>
    #include <MS561101BA.h>
#endif


/*----------------------------------------------------------------------------------------
 Macro Definitions
 ----------------------------------------------------------------------------------------*/
#if __DEBUG__
    #define Serialprint(...)                Serial.print(__VA_ARGS__)
    #define Serialprintln(...)              Serial.println(__VA_ARGS__)
#else
    #define Serialprint(...)
    #define Serialprintln(...)
#endif


/*----------------------------------------------------------------------------------------
 Type Definitions
 ----------------------------------------------------------------------------------------*/
typedef struct _AxisInt16_T
{
    int16_t            XAxis;
    int16_t            YAxis;
    int16_t            ZAxis;
}AxisInt16_T;


typedef struct _AxisFloat_T
{
    float              XAxis;
    float              YAxis;
    float              ZAxis;
}AxisFloat_T;

typedef struct _AxisErrRate_T
{
    float               nAngleErr;
    float               nCurrErrRate;
    float               nPrevErrRate;
    float               nP_ErrRate;
    float               nI_ErrRate;
    float               nD_ErrRate;
    float               nBalance;
    float               nTorque;
}AxisErrRate_T;

#if __GYROACCEL_ENABLED__
typedef struct _GyroAccelParam_T
{
    int16_t             nGyro[3];
    int16_t             nAccel[3];
    float               nBaseGyro[3];
    float               nBaseAccel[3];
    float               nCurrReadTime;
    float               nPrevReadTime;
    float               nFineAngle[3];                          // Filtered Angles
    float               nFineRPY[3];                            // yaw pitch roll values
}GyroAccelParam_T;
#endif

#if __MAG_ENABLED__
typedef struct _MagParam_T
{
    byte                nRawBits[6];
    //Vector              nRawData;
    //Vector              nNormData;
    float               nRawData[3];
    float               nNormData[3];
    float               nMagHeadingRad;
    float               nMagHeadingDeg;
}MagneticParam_T;
#endif

#if __BAROMETER_ENABLED__
typedef struct _BaroParam_T
{
    uint32_t            nRawTemp;                               // Raw Temperature Data
    uint32_t            nRawPressure;                           // Raw Pressure Data
    double              nRealTemperature;                       // Real Temperature Data
    float               nRealPressure;                          // Real Pressure Data
    float               nAvgpressure;                           // Average Pressure Data
    float               nAbsoluteAltitude;                      // Estimated Absolute Altitude
    float               nRelativeAltitude;                      // Estimated Relative Altitude
    double              nRefTemperature;                        // Reference Temperature Value
    int32_t             nRefPressure;                           // Reference Pressure Value
}BaroParam_T;
#endif


/*----------------------------------------------------------------------------------------
 Static Function
 ----------------------------------------------------------------------------------------*/
float Clip3Float(const float nValue, const int MIN, const int MAX);
int Clip3Int(const int nValue, const int MIN, const int MAX);
inline void acquireLock();
inline void releaseLock();
inline void nRCInterrupt_CB1();
inline void nRCInterrupt_CB2();
inline void nRCInterrupt_CB3();
inline void nRCInterrupt_CB5();
inline void CalculatePID(struct _AxisErrRate_T *pRoll, struct _AxisErrRate_T *pPitch, struct _AxisErrRate_T *pYaw, float nGyro[3], float nRPY[3]);
inline void CalculateThrottleVal(struct _AxisErrRate_T *pRoll, struct _AxisErrRate_T *pPitch, struct _AxisErrRate_T *pYaw, int nThrottle[4]);
inline void UpdateESCs(int nThrottle[4]);
inline void dmpDataReady();
inline void arm();
void _ESC_Initialize();
void _RC_Initialize();
#if __MAG_ENABLED__
    void MagHMC5883Int();
    void Magnetic_ReadData(struct _MagParam_T *pMagParam);
    float tiltCompensate(Vector mag, Vector normAccel);
    void CalibrateMagnetic();
#endif

#if __DEBUG__
    void _print_RC_Signals();
    void _print_Gyro_Signals(float nGyro[3]);
    void _print_Throttle_Signals(int nThrottle[4]);
    void _print_RPY_Signals(float nRPY[3]);
    void _print_MagData(struct _MagParam_T *pMagParam);
    void _print_BarometerData(struct _BaroParam_T *pBaroParam);
#endif

/*----------------------------------------------------------------------------------------
 Static Variable
 ----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 Global Variable
 ----------------------------------------------------------------------------------------*/
volatile bool           nMPUInterruptFlag = false;
bool                    nInterruptLockFlag = false;             // Interrupt lock
bool                    nDMPReadyFlag = false;                  // set true if DMP init was successful
uint16_t                nPacketSize = 0;                            // expected DMP packet size (default is 42 bytes)
float                   nRC_Ch[5] = {0, };                      // RC channel inputs
unsigned long           nRCPrevChangeTime1 = micros();
unsigned long           nRCPrevChangeTime2 = micros();
unsigned long           nRCPrevChangeTime3 = micros();
unsigned long           nRCPrevChangeTime4 = micros();
unsigned long           nRCPrevChangeTime5 = micros();
float                   nDeclinationAngle = 0.0f;

Servo                   nESC[4];
#if __GYROACCEL_ENABLED__
    MPU6050             nGyroAccel;                                   // MPU6050 Gyroscope Interface
    GyroAccelParam_T    nGyroAccelParam;
    Quaternion          nQuater;
    VectorFloat         nGravity;
#endif

#if __MAG_ENABLED__
    HMC5883L             nMagHndl;                              // HMC5883 Magnetic Interface
    MagneticParam_T      nMagParam;
#endif
#if __BAROMETER_ENABLED__
    MS561101BA           nBarometer;
    BaroParam_T          nBaroParam;
#endif


/*----------------------------------------------------------------------------------------
 Function Implementation
 ----------------------------------------------------------------------------------------*/
#if __GYROACCEL_ENABLED__
#define GYROACCEL_GET_BASE_GYRO(pGyroAccelParam, idx)               (pGyroAccelParam->nBaseGyro[idx])
#define GYROACCEL_GET_BASE_Accel(pGyroAccelParam, idx)              (pGyroAccelParam->nBaseAccel[idx])
#define GYROACCEL_GET_PREV_GYROANG(pGyroAccelParam, idx)               (pGyroAccelParam->nPrevGyro[idx])
#define GYROACCEL_GET_PREV_AccelANG(pGyroAccelParam, idx)              (pGyroAccelParam->nPrevAccel[idx])

int _GyroAccel_Initialize(struct _GyroAccelParam_T *pGyroAccelParam);
void _GyroAccel_GetData(struct _GyroAccelParam_T *pGyroAccelParam);
void _GyroAccel_CalculateAngle(struct _GyroAccelParam_T *pGyroAccelParam);
void _GyroAccel_Calibrate(struct _GyroAccelParam_T *pGyroAccelParam);
#if __GYROACCEL_DMP_ENABLED__
    int _GyroAccel_GetDMPData(struct _GyroAccelParam_T *pGyroAccelParam);
#endif

int _GyroAccel_Initialize(struct _GyroAccelParam_T *pGyroAccelParam)
{
    Serialprintln(F("Initializing MPU..."));
    nGyroAccel.initialize();
    
    // verify connection
    Serialprint(F("    Testing device connections..."));
    Serialprintln(nGyroAccel.testConnection() ? F("  MPU6050 connection successful") : F("  MPU6050 connection failed"));
    
    nGyroAccel.setI2CMasterModeEnabled(false);
    nGyroAccel.setI2CBypassEnabled(true);
    nGyroAccel.setSleepEnabled(false);
    
    #if __GYROACCEL_DMP_ENABLED__
    {
        uint8_t                 nDevStatus;                         // return status after each device operation (0 = success, !0 = error)
        
        // load and configure the DMP
        Serialprintln(F("    Initializing DMP..."));
        nDevStatus = nGyroAccel.dmpInitialize();
        
        // make sure it worked (returns 0 if so)
        if(0 == nDevStatus)
        {
            // turn on the DMP, now that it's ready
            Serialprintln(F("        Enabling DMP..."));
            nGyroAccel.setDMPEnabled(true);
            
            // enable Arduino interrupt detection
            Serialprintln(F("            Enabling interrupt detection (Arduino external interrupt 0)..."));
            pinMode(PIN_GY86_EXT_INTERRUPT, INPUT);
            digitalWrite(PIN_GY86_EXT_INTERRUPT, HIGH);
            PCintPort::attachInterrupt(PIN_GY86_EXT_INTERRUPT, dmpDataReady, RISING);
            
            // set our DMP Ready flag so the main loop() function knows it's okay to use it
            Serialprintln(F("    Done... DMP ready! Waiting for first interrupt..."));
            nDMPReadyFlag = true;
            
            // get expected DMP packet size for later comparison
            nPacketSize = nGyroAccel.dmpGetFIFOPacketSize();
        }
        else
        {
            // ERROR!
            // 1 = initial memory load failed
            // 2 = DMP configuration updates failed
            // (if it's going to break, usually the code will be 1)
            Serialprint(F("     DMP Initialization failed (code "));
            Serialprint(nDevStatus);
            Serialprintln(F(")"));
        }
    }
    #endif
    
    // supply your own gyro offsets here, scaled for min sensitivity
    nGyroAccel.setXGyroOffset(220);
    nGyroAccel.setYGyroOffset(76);
    nGyroAccel.setZGyroOffset(-85);
    nGyroAccel.setZAccelOffset(1788);                                     // 1688 factory default for my test chip
    nGyroAccel.setRate(1);                                                // Sample Rate (500Hz = 1Hz Gyro SR / 1+1)
    nGyroAccel.setDLPFMode(MPU6050_DLPF_BW_20);                           // Low Pass filter 20hz
    nGyroAccel.setFullScaleGyroRange(GYRO_FS_PRECISIOM);                // 250? / s (MPU6050_GYRO_FS_250)
    nGyroAccel.setFullScaleAccelRange(ACCEL_FS_PRECISIOM);                // +-2g (MPU6050_ACCEL_FS_2)

    // Calibrate GyroAccel
    Serialprint(F("    Calibrating Gyro & Accel   "));
    _GyroAccel_Calibrate(pGyroAccelParam);
    Serialprintln(F("    Done"));
    Serialprintln(F("Done"));
    
    return 0;
}


void _GyroAccel_GetData(struct _GyroAccelParam_T *pGyroAccelParam)
{
    int16_t             *pGyro = &(pGyroAccelParam->nGyro[X_AXIS]);
    int16_t             *pAccel = &(pGyroAccelParam->nAccel[X_AXIS]);

    pGyroAccelParam->nCurrReadTime = micros();

    // Read Gyro and Accelerate Data
    nGyroAccel.getRotation(&pGyro[X_AXIS], &pGyro[Y_AXIS], &pGyro[Z_AXIS]);
    nGyroAccel.getAcceleration(&pAccel[X_AXIS], &pAccel[Y_AXIS], &pAccel[Z_AXIS]);
    //nGyroAccel.getMotion6(&pGyro[X_AXIS], &pGyro[Y_AXIS], &pGyro[Z_AXIS], &pAccel[X_AXIS], &pAccel[Y_AXIS], &pAccel[Z_AXIS]);
}


void _GyroAccel_CalculateAngle(struct _GyroAccelParam_T *pGyroAccelParam)
{
    const float         nFS = 131.0f;
    const float         nRadtoDegOffset = 57.2958;             // = 180.0 / M_PI;
    const int16_t       *pGyro = &(pGyroAccelParam->nGyro[X_AXIS]);
    const int16_t       *pAccel = &(pGyroAccelParam->nAccel[X_AXIS]);
    const float         *pBaseGyro = &(pGyroAccelParam->nBaseGyro[X_AXIS]);
    float               *pFineAngle = &(pGyroAccelParam->nFineAngle[X_AXIS]);
    static float               nGyroAngle[3] = {0, };
    float               nAccelAngle[3] = {0, };
    float               nGyroDiffAngle[3] = {0, };
    float               nRawAngle[3] = {0, };
    static float        nPrevRawAngle[3] = {0, };
    
    // Convert Gyro Values to Degrees/Sec
    nGyroDiffAngle[X_AXIS] = (pGyro[X_AXIS] - pBaseGyro[X_AXIS]) / nFS;
    nGyroDiffAngle[Y_AXIS] = (pGyro[Y_AXIS] - pBaseGyro[Y_AXIS]) / nFS;
    nGyroDiffAngle[Z_AXIS] = (pGyro[Z_AXIS] - pBaseGyro[Z_AXIS]) / nFS;
    
    // float accel_vector_length = sqrt(pow(accel_x,2) + pow(accel_y,2) + pow(accel_z,2));
    nAccelAngle[X_AXIS] = atan(pAccel[Y_AXIS] / sqrt(pow(pAccel[X_AXIS], 2) + pow(pAccel[Z_AXIS], 2))) * nRadtoDegOffset;
    nAccelAngle[Y_AXIS] = atan((-1) * pAccel[X_AXIS] / sqrt(pow(pAccel[Y_AXIS], 2) + pow(pAccel[Z_AXIS], 2))) * nRadtoDegOffset;
    nAccelAngle[Z_AXIS] = 0;
    
    // Compute the (filtered) gyro angles
    {
        float dt =(pGyroAccelParam->nCurrReadTime - pGyroAccelParam->nPrevReadTime) / 1000000.0;
        
        nGyroAngle[X_AXIS] = nGyroDiffAngle[X_AXIS] * dt + pFineAngle[X_AXIS];
        nGyroAngle[Y_AXIS] = nGyroDiffAngle[Y_AXIS] * dt + pFineAngle[Y_AXIS];
        nGyroAngle[Z_AXIS] = nGyroDiffAngle[Z_AXIS] * dt + pFineAngle[Z_AXIS];
        
        // Compute the drifting gyro angles
        //nRawAngle[X_AXIS] = nGyroDiffAngle[X_AXIS] * dt + nPrevRawAngle[X_AXIS];
        //nRawAngle[Y_AXIS] = nGyroDiffAngle[Y_AXIS] * dt + nPrevRawAngle[Y_AXIS];
        //nRawAngle[Z_AXIS] = nGyroDiffAngle[Z_AXIS] * dt + nPrevRawAngle[Z_AXIS];
    }
    
    // Apply the complementary filter to figure out the change in angle - choice of alpha is
    // estimated now.  Alpha depends on the sampling rate...
    {
        const float     nOffset0 = 0.80;
        const float     nOffset1 = 1.0 - nOffset0;
        
        pFineAngle[X_AXIS] = nOffset0 * nGyroAngle[X_AXIS] + nOffset1 * nAccelAngle[X_AXIS];
        pFineAngle[Y_AXIS] = nOffset0 * nGyroAngle[Y_AXIS] + nOffset1 * nAccelAngle[Y_AXIS];
        pFineAngle[Z_AXIS] = nGyroAngle[Z_AXIS];  //Accelerometer doesn't give z-angle
    }
    
    // Update the saved data with the latest values
    pGyroAccelParam->nPrevReadTime = pGyroAccelParam->nCurrReadTime;
    
    //Serialprint(nGyroAngle[0]);Serialprint("  ");Serialprint(nGyroAngle[1]);Serialprint("  ");Serialprint(nGyroAngle[2]);Serialprint("  /  ");
    //Serialprint(nAccelAngle[0]);Serialprint("  ");Serialprint(nAccelAngle[1]);Serialprint("  ");Serialprint(nAccelAngle[2]);Serialprint("  /  ");
    //Serialprint(pFineAngle[0]);Serialprint("  ");Serialprint(pFineAngle[1]);Serialprint("  ");Serialprint(pFineAngle[2]);Serialprint("  /  ");
}


void _GyroAccel_Calibrate(struct _GyroAccelParam_T *pGyroAccelParam)
{
    int                     i = 0;
    int32_t                 nGyro[3] = {0, };
    int32_t                 nAccel[3] = {0, };
    const int               nLoopCnt = 20;
    
    for(i=0 ; i<nLoopCnt ; i++)
    {
        _GyroAccel_GetData(pGyroAccelParam);
        
        nGyro[X_AXIS] += pGyroAccelParam->nGyro[X_AXIS];
        nGyro[Y_AXIS] += pGyroAccelParam->nGyro[Y_AXIS];
        nGyro[Z_AXIS] += pGyroAccelParam->nGyro[Z_AXIS];
        nAccel[X_AXIS] += pGyroAccelParam->nAccel[X_AXIS];
        nAccel[Y_AXIS] += pGyroAccelParam->nAccel[Y_AXIS];
        nAccel[Z_AXIS] += pGyroAccelParam->nAccel[Z_AXIS];
        
        delay(20);
        
        Serialprint(".");
    }
    
    // Store the raw calibration values globally
    pGyroAccelParam->nBaseGyro[X_AXIS] = nGyro[X_AXIS] / nLoopCnt;
    pGyroAccelParam->nBaseGyro[Y_AXIS] = nGyro[Y_AXIS] / nLoopCnt;
    pGyroAccelParam->nBaseGyro[Z_AXIS] = nGyro[Z_AXIS] / nLoopCnt;
    pGyroAccelParam->nBaseAccel[X_AXIS] = nAccel[X_AXIS] / nLoopCnt;
    pGyroAccelParam->nBaseAccel[Y_AXIS] = nAccel[Y_AXIS] / nLoopCnt;
    pGyroAccelParam->nBaseAccel[Z_AXIS] = nAccel[Z_AXIS] / nLoopCnt;
}


#if __GYROACCEL_DMP_ENABLED__
int _GyroAccel_GetDMPData(struct _GyroAccelParam_T *pGyroAccelParam)
{
    uint16_t                nFIFOCnt = 0;                                               // count of all bytes currently in FIFO
    uint8_t                 nFIFOBuf[64];                                               // FIFO storage buffer
    uint8_t                 nMPUInterruptStat;                                          // holds actual interrupt status byte from MPU
    
    if(!nDMPReadyFlag)
    {
        Serialprintln("DMP Not Ready    Loop Run Fails");
        return -1;
    }
    
    while(!nMPUInterruptFlag && nFIFOCnt < nPacketSize)
    {
        // Wait Till Interrupt Up!!
    }
    
    // reset interrupt flag and get INT_STATUS byte
    nMPUInterruptFlag = false;
    nMPUInterruptStat = nGyroAccel.getIntStatus();
    
    // get current FIFO count
    nFIFOCnt = nGyroAccel.getFIFOCount();
    
    // check for overflow (this should never happen unless our code is too inefficient)
    if((nMPUInterruptStat & 0x10) || nFIFOCnt == 1024)
    {
        // reset so we can continue cleanly
        nGyroAccel.resetFIFO();
        //Serialprintln(F("FIFO overflow!"));
    }
    else if(nMPUInterruptStat & 0x02)
    {
        int16_t             nGyro[3];
        
        // wait for correct available data length, should be a VERY short wait
        while (nFIFOCnt < nPacketSize)
            nFIFOCnt = nGyroAccel.getFIFOCount();
        
        // read a packet from FIFO
        nGyroAccel.getFIFOBytes(nFIFOBuf, nPacketSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        nFIFOCnt -= nPacketSize;
        
        nGyroAccel.dmpGetGyro(nGyro, nFIFOBuf);
        nGyroAccel.dmpGetQuaternion(&nQuater, nFIFOBuf);
        nGyroAccel.dmpGetGravity(&nGravity, &nQuater);
        nGyroAccel.dmpGetYawPitchRoll(pGyroAccelParam->nFineRPY, &nQuater, &nGravity);
        
        pGyroAccelParam->nFineAngle[X_AXIS] = nGyro[X_AXIS];
        pGyroAccelParam->nFineAngle[Y_AXIS] = nGyro[Y_AXIS];
        pGyroAccelParam->nFineAngle[Z_AXIS] = nGyro[Z_AXIS];
    }
    
    return 0;
}
#endif
#endif


#if __MAG_ENABLED__
int _Mag_Initialize(struct _MagParam_T *pMagParam);
void _Mag_Start();
void _Mag_ReadData(struct _MagParam_T *pMagParam);
void _Mag_CalculateDirection(struct _MagParam_T *pMagParam);
float _Mag_TiltCompensate(Vector mag, Vector normAccel);
void _Mag_Calibrate(struct _MagParam_T *pMagParam);


int _Mag_Initialize(struct _MagParam_T *pMagParam)
{
    // initialize Magnetic
    Serialprintln(F("Initializing Magnetic..."));
    _Mag_Start();
    
    // Calibrate Magnetic
    Serialprint(F("    Calibrating Magnetic...."));
    _Mag_Calibrate(&nMagParam);
    
    // Date: 2015-11-05
    // Location: Seoul, South Korea
    // Latitude: 37.0000° North
    // Longitude: 126.0000° East
    // Magnetic declination: 7° 59.76' West
    // Annual Change (minutes/year): 3.9 '/y West
    // http://www.geomag.nrcan.gc.ca/calc/mdcal-en.php
    nDeclinationAngle = (7.0 + (59.76 / 60.0)) / (180 / M_PI);
    Serialprintln(F("Done"));
}


void _Mag_Start()
{
    // Open communication with HMC5883L
    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.write(0x00);                                           // Configuration Register A
    Wire.write(0x70);                                           // Num samples: 8 ; output rate: 15Hz ; normal measurement mode
    Wire.endTransmission();
    delay(1);
    
    // Open communication with HMC5883L
    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.write(0x01);                                           // Configuration Register B
    Wire.write(0x20);                                           // Configuration gain 1.3Ga
    Wire.endTransmission();
    delay(1);
    
    // Put the HMC5883 IC into the correct operating mode
    // Open communication with HMC5883L
    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.write(0x02);                                           // Select mode register
    Wire.write(0x00);                                           // Continuous measurement mode
    Wire.endTransmission();
    delay(1);
}


void _Mag_ReadData(struct _MagParam_T *pMagParam)
{
    int                 i = 0;
    byte                result[6];
    int                 MagX,MagY,MagZ;
    static float        MagXf = 0.0f ,MagYf = 0.0f ,MagZf = 0.0f;
    const float         nRadtoDegOffset = 57.2958;                  // = 180.0 / M_PI;
    const float         nDougleRad = 6.283184;                      // = 2 * 3.141592
    
    //Tell the HMC5883 where to begin reading data
    Wire.beginTransmission(HMC5883L_ADDRESS);
    Wire.write(0x03);                                               // Select register 3, X MSB register
    Wire.endTransmission();
    
    //Read data from each axis, 2 registers per axis
    Wire.requestFrom(HMC5883L_ADDRESS, 6);
    while(Wire.available())
    {
        pMagParam->nRawBits[i] = Wire.read();
        i++;
    }
    Wire.endTransmission();
    
    pMagParam->nRawData[X_AXIS] = ((pMagParam->nRawBits[0] << 8) | pMagParam->nRawBits[1]);                  // Offset + 1.05
    pMagParam->nRawData[Z_AXIS] = ((pMagParam->nRawBits[2] << 8) | pMagParam->nRawBits[3]);                  // + 0.05
    pMagParam->nRawData[Y_AXIS] = ((pMagParam->nRawBits[4] << 8) | pMagParam->nRawBits[5]);                  // - 0.55
    
    //pMagParam->nRawData = nMagHndl.readRaw();
    //pMagParam->nNormData = nMagHndl.readNormalize();
}


void _Mag_CalculateDirection(struct _MagParam_T *pMagParam)
{
    int                 i = 0;
    int                 MagX,MagY,MagZ;
    static float        MagXf = 0.0f ,MagYf = 0.0f ,MagZf = 0.0f;
    const float         nRadtoDegOffset = 57.2958;                  // = 180.0 / M_PI;
    const float         nDougleRad = 6.283184;                      // = 2 * 3.141592
    
    pMagParam->nNormData[X_AXIS] = pMagParam->nNormData[X_AXIS] + (pMagParam->nRawData[X_AXIS] - pMagParam->nNormData[X_AXIS])*0.55;
    pMagParam->nNormData[Y_AXIS] = pMagParam->nNormData[Y_AXIS] + (pMagParam->nRawData[Y_AXIS] - pMagParam->nNormData[Y_AXIS])*0.55;
    pMagParam->nNormData[Z_AXIS] = pMagParam->nNormData[Z_AXIS] + (pMagParam->nRawData[Z_AXIS] - pMagParam->nNormData[Z_AXIS])*0.55;
    
    // adjust for  Magnetic axis offsets/sensitivity differences by scaling to +/-5 range
    //pMagParam->nNormData[X_AXIS] = ((float)(MagXf - M_X_MIN) / (M_X_MAX - M_X_MIN))*10.0 - 5.0;
    //pMagParam->nNormData[Y_AXIS] = ((float)(MagYf - M_Y_MIN) / (M_Y_MAX - M_Y_MIN))*10.0 - 5.0;
    //pMagParam->nNormData[Z_AXIS] = ((float)(MagZf - M_Z_MIN) / (M_Z_MAX - M_Z_MIN))*10.0 - 5.0;
    
    pMagParam->nMagHeadingRad = atan2(pMagParam->nRawData[Y_AXIS], pMagParam->nRawData[X_AXIS]) + nDeclinationAngle;
    
    if(pMagParam->nMagHeadingRad < 0)
        pMagParam->nMagHeadingRad += nDougleRad;
    
    if(pMagParam->nMagHeadingRad > nDougleRad)
        pMagParam->nMagHeadingRad -= nDougleRad;
    
    pMagParam->nMagHeadingDeg = pMagParam->nMagHeadingRad * nRadtoDegOffset;
    
    //if(pMagParam->nMagHeadingDeg >= 1 && pMagParam->nMagHeadingDeg < 240)
    //    pMagParam->nMagHeadingDeg = map(pMagParam->nMagHeadingDeg, 0, 239, 0, 179);
    //else if(pMagParam->nMagHeadingDeg >= 240)
    //    pMagParam->nMagHeadingDeg = map(pMagParam->nMagHeadingDeg, 240, 360, 180, 360);
    
    // Smooth angles rotation for +/- 3deg
    //int smoothHeadingDegrees = round(pMagParam->nMagHeadingDeg);
    //int previousDegree = 0;
    //if(smoothHeadingDegrees < (previousDegree + 3) && smoothHeadingDegrees > (previousDegree - 3))
    //    smoothHeadingDegrees = previousDegree;
}


float _Mag_TiltCompensate(Vector mag, Vector normAccel)
{
    // Pitch & Roll
    float roll;
    float pitch;
    
    roll = asin(normAccel.YAxis);
    pitch = asin(-normAccel.XAxis);
    
    if(roll > 0.78 || roll < -0.78 || pitch > 0.78 || pitch < -0.78)
        return -1000;
    
    // Some of these are used twice, so rather than computing them twice in the algorithem we precompute them before hand.
    float cosRoll = cos(roll);
    float sinRoll = sin(roll);
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    
    // Tilt compensation
    float Xh = mag.XAxis * cosPitch + mag.ZAxis * sinPitch;
    float Yh = mag.XAxis * sinRoll * sinPitch + mag.YAxis * cosRoll - mag.ZAxis * sinRoll * cosPitch;
    
    float heading = atan2(Yh, Xh);
    
    return heading;
}


void _Mag_Calibrate(struct _MagParam_T *pMagParam)
{
    int                 i = 0;
    float               nSumMinX = 0.0f;
    float               nSumMaxX = 0.0f;
    float               nSumMinY = 0.0f;
    float               nSumMaxY = 0.0f;
    const int           nLoopCnt = 50;
    
    for(i=0 ; i<nLoopCnt ; i++)
    {
        float           nMinX = 0.0f;
        float           nMaxX = 0.0f;
        float           nMinY = 0.0f;
        float           nMaxY = 0.0f;
        
        _Mag_ReadData(pMagParam);
        
        // Determine Min / Max values
        if(pMagParam->nRawData[X_AXIS] < nMinX)
            nMinX = pMagParam->nRawData[X_AXIS];
        if(pMagParam->nRawData[X_AXIS] > nMaxX)
            nMaxX = pMagParam->nRawData[X_AXIS];
        if(pMagParam->nRawData[Y_AXIS] < nMinY)
            nMinY = pMagParam->nRawData[Y_AXIS];
        if(pMagParam->nRawData[Y_AXIS] > nMaxY)
            nMaxY = pMagParam->nRawData[Y_AXIS];
        
        nSumMinX += nMinX;
        nSumMaxX += nMaxX;
        nSumMinY += nMinY;
        nSumMaxY += nMaxY;
        
        delay(20);
        
        Serialprint(".");
    }
    
    // Calculate offsets
    nMagHndl.setOffset(((nSumMaxX + nSumMinX) / 2 / nLoopCnt), ((nSumMaxY + nSumMinY) / 2 / nLoopCnt));
}
#endif


#if __BAROMETER_ENABLED__
int _Barometer_Initialize(struct _BaroParam_T *pBaroParam)
{
    Serialprint(F("Initializing Barometer..."));
    nBarometer = MS561101BA();
    nBarometer.init(MS561101BA_ADDR_CSB_LOW);
    Serialprintln(F("Done"));
}


void _Barometer_GetData(struct _BaroParam_T *pBaroParam)
{
    pBaroParam->nRealTemperature = nBarometer.getTemperature(MS561101BA_OSR_256);
    pBaroParam->nRealPressure = nBarometer.getPressure(MS561101BA_OSR_256);
    nBarometer.pushPressure(pBaroParam->nRealPressure);
    pBaroParam->nAvgpressure = nBarometer.getAvgPressure();
    pBaroParam->nAbsoluteAltitude = nBarometer.getAltitude(pBaroParam->nAvgpressure, pBaroParam->nRealTemperature);
}
#endif


void setup()
{
    uint8_t                 nDevStatus;                         // return status after each device operation (0 = success, !0 = error)
    
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
    #endif
    
    #if __DEBUG__
    Serial.begin(115200);
    Serial.flush();
    
    while(!Serial); // wait for Leonardo enumeration, others continue immediately
    #endif
    
    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.
    
    #if __GYROACCEL_ENABLED__
    {
        // Initialize Gyro_Accel
        _GyroAccel_Initialize(&nGyroAccelParam);
    }
    #endif
    
    #if __MAG_ENABLED__
    {
        // Initialize Magnetic
        _Mag_Initialize(&nMagParam);
    }
    #endif
    
    #if __BAROMETER_ENABLED__
    {
        // Initialize Barometer
        _Barometer_Initialize(&nBaroParam);
    }
    #endif
    
    // Initialize RemoteController
    _RC_Initialize();
    
    // Initialize ESCs
    _ESC_Initialize();
}


void loop()
{
    volatile bool           bSkipFlag = false;
    float                   nStartTime0 = 0.0f, nStartTime1 = 0.0f, nStartTime2 = 0.0f, nStartTime3 = 0.0f, nEndTime = 0.0f;
    int16_t                 nGyro[3];                                                   // Gyroscope Value
    float                   nRPY[3];                                                    // yaw pitch roll values

    nStartTime0 = micros();
    
    ////////////////////////////////// Get angle & gyro ///////////////////////////////////////////////////////
    
    nStartTime1 = micros();
    #if __GYROACCEL_ENABLED__
    {
        #if !__GYROACCEL_DMP_ENABLED__
        _GyroAccel_GetData(&nGyroAccelParam);
        _GyroAccel_CalculateAngle(&nGyroAccelParam);
        #else
        if(0 != _GyroAccel_GetDMPData(&nGyroAccelParam))
            return;
        #endif
    }
    #endif
    
    nStartTime2 = micros();
    // Get Magnetic Sensor Value
    #if __MAG_ENABLED__
    {
        _Mag_ReadData(&nMagParam);
        _Mag_CalculateDirection(&nMagParam);
    }
    #endif
    
    // Get Barometer Sensor Value
    #if __BAROMETER_ENABLED__
    {
        _Barometer_GetData(&nBaroParam);
    }
    #endif
    
    nStartTime3 = micros();
    if(false == bSkipFlag)
    {
        AxisErrRate_T           nPitch = {0, };
        AxisErrRate_T           nRoll = {0, };
        AxisErrRate_T           nYaw = {0, };
        int                     nThrottle[4] = {ESC_MIN, ESC_MIN, ESC_MIN, ESC_MIN};        // throttles
        
        // PID Computation
        CalculatePID(&nRoll, &nPitch, &nYaw, nGyroAccelParam.nFineAngle, nGyroAccelParam.nFineRPY);
        
        //Throttle Calculation
        CalculateThrottleVal(&nRoll, &nPitch, &nYaw, nThrottle);
        
        //Update BLDCs
        UpdateESCs(nThrottle);
    }
    
    #if __DEBUG__
    //_print_Gyro_Signals(nGyroAccelParam.nFineAngle);
    //_print_RPY_Signals(nGyroAccelParam.nFineRPY);
    //_print_MagData(&nMagParam);
    //_print_BarometerData(&nBaroParam);
    //_print_Throttle_Signals(nThrottle);
    //_print_RC_Signals();
    nEndTime = micros();
    Serialprint(" ");Serialprint((nEndTime - nStartTime0)/1000);
    Serialprintln("");
    #endif
}


float Clip3Float(const float nValue, const int MIN, const int MAX)
{
    float           nClipVal = nValue;
    
    if(nValue < MIN)
        nClipVal = MIN;
    else if(nValue > MAX)
        nClipVal = MAX;
    
    return nClipVal;
}


int Clip3Int(const int nValue, const int MIN, const int MAX)
{
    int             nClipVal = nValue;
    
    if(nValue < MIN)
        nClipVal = MIN;
    else if(nValue > MAX)
        nClipVal = MAX;
    
    return nClipVal;
}

inline void acquireLock()
{
    nInterruptLockFlag = true;
}


inline void releaseLock()
{
    nInterruptLockFlag = false;
}


inline void CalculatePID(struct _AxisErrRate_T *pRoll, struct _AxisErrRate_T *pPitch, struct _AxisErrRate_T *pYaw, float nGyro[3], float nRPY[3])
{
    static float            nRC_PrevCh[5] = {0, };              // Filter variables
    static float            nPrevRPY[3];
    
    acquireLock();
    
    nRC_Ch[0] = floor(nRC_Ch[0] / ROUNDING_BASE) * ROUNDING_BASE;
    nRC_Ch[1] = floor(nRC_Ch[1] / ROUNDING_BASE) * ROUNDING_BASE;
    nRC_Ch[3] = floor(nRC_Ch[3] / ROUNDING_BASE) * ROUNDING_BASE;
    
    nRC_Ch[0] = map(nRC_Ch[0], RC_LOW_CH1, RC_HIGH_CH1, ROLL_ANG_MIN, ROLL_ANG_MAX) - 1;
    nRC_Ch[1] = map(nRC_Ch[1], RC_LOW_CH2, RC_HIGH_CH2, PITCH_ANG_MIN, PITCH_ANG_MAX) - 1;
    nRC_Ch[3] = map(nRC_Ch[3], RC_LOW_CH4, RC_HIGH_CH4, YAW_RATE_MIN, YAW_RATE_MAX);
    
    if((nRC_Ch[0] < ROLL_ANG_MIN) || (nRC_Ch[0] > ROLL_ANG_MAX))
        nRC_Ch[0] = nRC_PrevCh[0];
    if((nRC_Ch[1] < PITCH_ANG_MIN) || (nRC_Ch[1] > PITCH_ANG_MAX))
        nRC_Ch[1] = nRC_PrevCh[1];
    if((nRC_Ch[3] < YAW_RATE_MIN) || (nRC_Ch[3] > YAW_RATE_MAX))
        nRC_Ch[3] = nRC_PrevCh[3];
    
    nRC_PrevCh[0] = nRC_Ch[0];
    nRC_PrevCh[1] = nRC_Ch[1];
    nRC_PrevCh[3] = nRC_Ch[3];
    
    nRPY[1] = nRPY[1] * 180 / M_PI + PITCH_ANG_OFFSET;
    nRPY[2] = nRPY[2] * 180 / M_PI + ROLL_ANG_OFFSET;
    
    if(abs(nRPY[1] - nPrevRPY[1]) > 30)
        nRPY[1] = nPrevRPY[1];
    
    if(abs(nRPY[2] - nPrevRPY[2]) > 30)
        nRPY[2] = nPrevRPY[2];
    
    //ROLL control
    pRoll->nAngleErr = nRC_Ch[0] - nRPY[2];
    pRoll->nCurrErrRate = pRoll->nAngleErr * ROLL_OUTER_P_GAIN - nGyro[0];
    pRoll->nP_ErrRate = pRoll->nCurrErrRate * ROLL_INNER_P_GAIN;
    pRoll->nI_ErrRate = Clip3Float((pRoll->nI_ErrRate + (pRoll->nCurrErrRate * ROLL_INNER_I_GAIN) * SAMPLING_TIME), -100, 100);
    pRoll->nD_ErrRate = (pRoll->nCurrErrRate - pRoll->nPrevErrRate) * ROLL_INNER_D_GAIN / SAMPLING_TIME;
    pRoll->nBalance = pRoll->nP_ErrRate + pRoll->nI_ErrRate + pRoll->nD_ErrRate;
    
    //PITCH control
    pPitch->nAngleErr = nRC_Ch[1] - nRPY[1];
    pPitch->nCurrErrRate = pPitch->nAngleErr * PITCH_OUTER_P_GAIN + nGyro[1];
    pPitch->nP_ErrRate = pPitch->nCurrErrRate * PITCH_INNER_P_GAIN;
    pPitch->nI_ErrRate = Clip3Float((pPitch->nI_ErrRate + (pPitch->nCurrErrRate * PITCH_INNER_I_GAIN) * SAMPLING_TIME), -100, 100);
    pPitch->nD_ErrRate = (pPitch->nCurrErrRate - pPitch->nPrevErrRate) * PITCH_INNER_D_GAIN / SAMPLING_TIME;
    pPitch->nBalance = pPitch->nP_ErrRate + pPitch->nI_ErrRate + pPitch->nD_ErrRate;
    
    //YAW control
    pYaw->nCurrErrRate = nRC_Ch[3] - nGyro[2];
    pYaw->nP_ErrRate = pYaw->nCurrErrRate * YAW_P_GAIN;
    pYaw->nI_ErrRate = Clip3Float((pYaw->nI_ErrRate + pYaw->nCurrErrRate * YAW_I_GAIN * SAMPLING_TIME), -50, 50);
    pYaw->nTorque = pYaw->nP_ErrRate + pYaw->nI_ErrRate;
    
    // Backup for Next
    pRoll->nPrevErrRate = pRoll->nCurrErrRate;
    pPitch->nPrevErrRate = pPitch->nCurrErrRate;
    nPrevRPY[1] = nRPY[1];
    nPrevRPY[2] = nRPY[2];
    
    releaseLock();
}


inline void CalculateThrottleVal(struct _AxisErrRate_T *pRoll, struct _AxisErrRate_T *pPitch, struct _AxisErrRate_T *pYaw, int nThrottle[4])
{
    float                   nEstimatedThrottle = 0.0f;
    static float            nPrevEstimatedThrottle = 0.0f;
    
    acquireLock();
    
    nRC_Ch[2] = floor(nRC_Ch[2] / ROUNDING_BASE) * ROUNDING_BASE;
    nEstimatedThrottle = (float)(map(nRC_Ch[2], RC_LOW_CH3, RC_HIGH_CH3, ESC_MIN, ESC_MAX));
    
    if((nEstimatedThrottle < ESC_MIN) || (nEstimatedThrottle > ESC_MAX))
        nEstimatedThrottle = nPrevEstimatedThrottle;
    
    nPrevEstimatedThrottle = nEstimatedThrottle;
    
    releaseLock();
    
    if(nEstimatedThrottle > ESC_TAKEOFF_OFFSET)
    {
        nThrottle[0] = Clip3Int((( pPitch->nBalance - pRoll->nBalance) * 0.5 + pYaw->nTorque + nEstimatedThrottle), ESC_MIN, ESC_MAX);
        nThrottle[1] = Clip3Int(((-pPitch->nBalance - pRoll->nBalance) * 0.5 - pYaw->nTorque + nEstimatedThrottle), ESC_MIN, ESC_MAX);
        nThrottle[2] = Clip3Int(((-pPitch->nBalance + pRoll->nBalance) * 0.5 + pYaw->nTorque + nEstimatedThrottle), ESC_MIN, ESC_MAX);
        nThrottle[3] = Clip3Int((( pPitch->nBalance + pRoll->nBalance) * 0.5 - pYaw->nTorque + nEstimatedThrottle), ESC_MIN, ESC_MAX);
    }
}


inline void UpdateESCs(int nThrottle[4])
{
    nESC[1].writeMicroseconds(nThrottle[0]);
    nESC[3].writeMicroseconds(nThrottle[1]);
    nESC[2].writeMicroseconds(nThrottle[2]);
    nESC[4].writeMicroseconds(nThrottle[3]);
}


inline void nRCInterrupt_CB1()
{
    if(!nInterruptLockFlag)
        nRC_Ch[0] = micros() - nRCPrevChangeTime1;
    
    nRCPrevChangeTime1 = micros();
}


inline void nRCInterrupt_CB2()
{
    if(!nInterruptLockFlag)
        nRC_Ch[1] = micros() - nRCPrevChangeTime2;
    
    nRCPrevChangeTime2 = micros();
}


inline void nRCInterrupt_CB3()
{
    if(!nInterruptLockFlag)
        nRC_Ch[2] = micros() - nRCPrevChangeTime3;
    
    nRCPrevChangeTime3 = micros();
}


inline void nRCInterrupt_CB4()
{
    if(!nInterruptLockFlag)
        nRC_Ch[3] = micros() - nRCPrevChangeTime4;
    
    nRCPrevChangeTime4 = micros();
}


inline void nRCInterrupt_CB5()
{
    if(!nInterruptLockFlag)
        nRC_Ch[4] = micros() - nRCPrevChangeTime5;
    
    nRCPrevChangeTime5 = micros();
}

inline void dmpDataReady()
{
    nMPUInterruptFlag = true;
}


inline void arm()
{
    nESC[1].writeMicroseconds(ESC_MIN);
    nESC[2].writeMicroseconds(ESC_MIN);
    nESC[3].writeMicroseconds(ESC_MIN);
    nESC[4].writeMicroseconds(ESC_MIN);
    
    delay(ESC_ARM_DELAY);
}


void _ESC_Initialize()
{
    nESC[1].attach(PIN_ESC_1);
    nESC[2].attach(PIN_ESC_2);
    nESC[3].attach(PIN_ESC_3);
    nESC[4].attach(PIN_ESC_4);
    
    delay(1000);
    
    arm();
}


void _RC_Initialize()
{
    pinMode(PIN_CHECK_POWER_STAT, OUTPUT);
    digitalWrite(PIN_CHECK_POWER_STAT, HIGH);
    
    PCintPort::attachInterrupt(PIN_RC_CH1, nRCInterrupt_CB1, CHANGE);
    PCintPort::attachInterrupt(PIN_RC_CH2, nRCInterrupt_CB2, CHANGE);
    PCintPort::attachInterrupt(PIN_RC_CH3, nRCInterrupt_CB3, CHANGE);
    PCintPort::attachInterrupt(PIN_RC_CH4, nRCInterrupt_CB4, CHANGE);
    PCintPort::attachInterrupt(PIN_RC_CH5, nRCInterrupt_CB5, CHANGE);
}


#if __DEBUG__
void _print_RC_Signals()
{
    Serialprint("   //   RC_Roll:");
    if(nRC_Ch[0] - 1480 < 0)Serialprint("<<<");
    else if(nRC_Ch[0] - 1520 > 0)Serialprint(">>>");
    else Serialprint("-+-");
    Serialprint(nRC_Ch[0]);
    
    Serialprint("   RC_Pitch:");
    if(nRC_Ch[1] - 1480 < 0)Serialprint("^^^");
    else if(nRC_Ch[1] - 1520 > 0)Serialprint("vvv");
    else Serialprint("-+-");
    Serialprint(nRC_Ch[1]);
    
    Serialprint("   RC_Throttle:");
    if(nRC_Ch[2] - 1480 < 0)Serialprint("vvv");
    else if(nRC_Ch[2] - 1520 > 0)Serialprint("^^^");
    else Serialprint("-+-");
    Serialprint(nRC_Ch[2]);
    
    Serialprint("   RC_Yaw:");
    if(nRC_Ch[3] - 1480 < 0)Serialprint("<<<");
    else if(nRC_Ch[3] - 1520 > 0)Serialprint(">>>");
    else Serialprint("-+-");
    Serialprint(nRC_Ch[3]);
    
    Serialprint("   RC_Gear:");
    if(nRC_Ch[4] - 1480 < 0)Serialprint("<<<");
    else if(nRC_Ch[4] - 1520 > 0)Serialprint(">>>");
    else Serialprint("-+-");
    Serialprint(nRC_Ch[4]);
}


void _print_Gyro_Signals(float nGyro[3])
{
    Serialprint("   //    Roll Gyro : ");
    Serialprint(nGyro[0]);
    Serialprint("   Pitch Gyro : ");
    Serialprint(nGyro[1]);
    Serialprint("   Yaw Gyro : ");
    Serialprint(nGyro[2]);
}


void _print_Throttle_Signals(int nThrottle[4])
{
    Serialprint("   //    Thrt1 : ");
    Serialprint(nThrottle[0]);
    Serialprint("  Thrt2 : ");
    Serialprint(nThrottle[1]);
    Serialprint("  Thrt3 : ");
    Serialprint(nThrottle[2]);
    Serialprint("  Thrt4 : ");
    Serialprint(nThrottle[3]);
}


void _print_RPY_Signals(float nRPY[3])
{
    Serialprint("   //    Roll: ");
    Serialprint(nRPY[2]);
    Serialprint("   Pitch: ");
    Serialprint(nRPY[1]);
    Serialprint("   Yaw: ");
    Serialprint(nRPY[0]);
}

void _print_MagData(struct _MagParam_T *pMagParam)
{
    #if __MAG_ENABLED__
    Serialprint("   //    Magnetic -> X:"); Serialprint(pMagParam->nNormData[X_AXIS]);
    Serialprint("   Y:"); Serialprint(pMagParam->nNormData[Y_AXIS]);
    Serialprint("   Z:"); Serialprint(pMagParam->nNormData[Z_AXIS]);
    Serialprint("   Head:"); Serialprint(pMagParam->nMagHeadingDeg);
    #endif
}

void _print_BarometerData(struct _BaroParam_T *pBaroParam)
{
    #if __BAROMETER_ENABLED__
    Serialprint("   //    Barometer -> Temp:"); Serialprint(pBaroParam->nRealTemperature);
    Serialprint("   Press:"); Serialprint(pBaroParam->nRealPressure);
    Serialprint("   AvgPress:"); Serialprint(pBaroParam->nAvgpressure);
    Serialprint("   AbsAlt:"); Serialprint(pBaroParam->nAbsoluteAltitude);
    #endif
}
#endif














