//
//  CommHeader.h
//  SelfFly
//
//  Created by Maverick on 2016. 1. 21..
//

#ifndef __COMMON_HEADER__
#define __COMMON_HEADER__

// Define Axis
#define X_AXIS                              (0)
#define Y_AXIS                              (1)
#define Z_AXIS                              (2)

// Define Max Number of Each Field
#define MAX_CH_RC                           (5)
#define MAX_CH_ESC                          (4)

// Arduino Pin configuration
#define PIN_LED_BLUE                        (13)
#define PIN_LED_GREEN                       (12)
#define PIN_ESC_CH3                         (11)
#define PIN_ESC_CH2                         (10)
#define PIN_ESC_CH1                         (9)
#define PIN_ESC_CH0                         (8)
#define PIN_LED_RED                         (7)
#define PIN_RC_CH4                          (6)
#define PIN_RC_CH3                          (5)
#define PIN_RC_CH2                          (4)
#define PIN_RC_CH1                          (3)
#define PIN_RC_CH0                          (2)
#define PIN_RESERVED_D01                    (1)
#define PIN_RESERVED_D00                    (0)
#define PIN_RESERVED_A07                    (A7)
#define PIN_RESERVED_A06                    (A6)
#define PIN_GY86_SCL                        (A5)
#define PIN_GY86_SDA                        (A4)
#define PIN_CHECK_POWER_STAT                (A3)
#define PIN_RESERVED_A02                    (A2)
#define PIN_SONAR_TRIG                      (A1)
#define PIN_SONAR_ECHO                      (A0)

// ESC configuration
#define ESC_MIN                             (1000)
#define ESC_MAX                             (2000)
#define ESC_TAKEOFF_OFFSET                  (999)
#define ESC_ARM_DELAY                       (1000)

// RC configuration
#define RC_CH0_HIGH                         (1884)
#define RC_CH0_LOW                          (1055)
#define RC_CH1_HIGH                         (1884)
#define RC_CH1_LOW                          (1055)
#define RC_CH2_HIGH                         (1884)
#define RC_CH2_LOW                          (1055)
#define RC_CH3_HIGH                         (1884)
#define RC_CH3_LOW                          (1055)
#define RC_CH4_HIGH                         (1884)
#define RC_CH4_LOW                          (1055)

// PID configuration
#define PITCH_OUTER_P_GAIN                  (4.750)                         // angle control
#define PITCH_INNER_P_GAIN                  (2.933)                         // rate control
#define PITCH_INNER_I_GAIN                  (0.440)
#define PITCH_INNER_D_GAIN                  (0.335)
#define ROLL_OUTER_P_GAIN                   (4.750)                         // angle control
#define ROLL_INNER_P_GAIN                   (2.833)                         // rate control
#define ROLL_INNER_I_GAIN                   (0.440)
#define ROLL_INNER_D_GAIN                   (0.335)
#define YAW_P_GAIN                          (2.325)                         // yaw -> rate control
#define YAW_I_GAIN                          (0.650)

#define GYRO_FS_PRECISIOM                   (MPU6050_GYRO_FS_250)
#define GYRO_FS                             (131.0f)                        // (2^15 - 1) / (250 * (1 << GYRO_FS_PRECISIOM))
#define ACCEL_FS_PRECISIOM                  (MPU6050_ACCEL_FS_2)            //  MPU6050_ACCEL_FS_4  MPU6050_ACCEL_FS_8  MPU6050_ACCEL_FS_16
#define ACCEL_STD_DENOM                     (16384.0f / (1 << ACCEL_FS_PRECISIOM))

// Flight parameters
#define PITCH_ANG_MIN                       (-30)
#define PITCH_ANG_MAX                       (30)
#define ROLL_ANG_MIN                        (-30)
#define ROLL_ANG_MAX                        (30)
#define YAW_RATE_MIN                        (-30)
#define YAW_RATE_MAX                        (30)

// Offset values
#define PITCH_ANG_OFFSET                    (-4)
#define ROLL_ANG_OFFSET                     (1.6)

#define SAMPLEFREQ                          (133.0f)                        // sample frequency in Hz
#define BETADEF                             (1.1f)

// AccelGyro Offset Value
#define MPU6050_GYRO_OFFSET_X               (65)
#define MPU6050_GYRO_OFFSET_Y               (-42)
#define MPU6050_GYRO_OFFSET_Z               (-3)
#define MPU6050_ACCEL_OFFSET_X              (-73)
#define MPU6050_ACCEL_OFFSET_Y              (-737)
#define MPU6050_ACCEL_OFFSET_Z              (0)


// Sonar sensor
#define SONAR_MAX_WAIT                      (30000)                         // Unit: microsecond

#define ROUNDING_BASE                       (10)
#define SAMPLING_TIME                       (0.01)                          // Unit: Seconds

#define RAD_TO_DEG_SCALE                    (57.2958f)                      // = 180 / PI
#define DEG_TO_RAD_SCALE                    (0.0175f)                       // = PI / 180
#define SINGLE_RADIAN                       (3.141592)                      // = PI
#define DOUBLE_RADIAN                       (6.283184)                      // = 2 * PI
#define BARO_SEA_LEVEL_BASE                 (1013.25)                       // Base Sea Level

#define DRONE_STOP_TIME_TH                  (3000)                          // Unit: num of loop() count, About 30 Sec.


// LED Control
#define LED_BLINK_PERIOD                    (1000000)                       // 1000 ms




#endif /* CommHeader_h */
