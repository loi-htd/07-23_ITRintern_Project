/**
 * @file       bsp_mpu.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Dung Nguyen
 * @brief      Board Support Package for mpu6050
 * @note
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_MPU_H_
#define BSP_MPU_H_

/* Includes ----------------------------------------------------------- */
#include "bsp_i2c.h"
#include "main.h"

/* Public defines ----------------------------------------------------- */
/**
 * @brief  Basic register config MPU6050
 * @{
 */
#define MPU6050_ADDR 0x68
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

/**
 * @brief  MPU6050 Acceleration Config
 * @{
 */
#define MPU6050_ACCE_SENS_REG_2 0x00
#define MPU6050_ACCE_SENS_REG_4 0x08
#define MPU6050_ACCE_SENS_REG_8 0x10
#define MPU6050_ACCE_SENS_REG_16 0x18

/**
 * @brief  MPU6050 Gyroscope Config
 * @{
 */
#define MPU6050_GYRO_SENS_REG_250 0x00
#define MPU6050_GYRO_SENS_REG_500 0x08
#define MPU6050_GYRO_SENS_REG_1000 0x10
#define MPU6050_GYRO_SENS_REG_2000 0x18

/**
 * @brief  MPU6050 Power Management Config
 * @{
 */
#define MPU6050_PWR_MNG_INTERNAL 0x00
#define MPU6050_PWR_MNG_PLL_WITH_X_AXIS 0x01

/**
 * @brief  Sample rates predefined constants
 * @{
 */
#define MPU6050_SAMPLERATE_8KHZ 0   /*!< Sample rate set to 8 kHz */
#define MPU6050_SAMPLERATE_4KHZ 1   /*!< Sample rate set to 4 kHz */
#define MPU6050_SAMPLERATE_2KHZ 3   /*!< Sample rate set to 2 kHz */
#define MPU6050_SAMPLERATE_1KHZ 7   /*!< Sample rate set to 1 kHz */
#define MPU6050_SAMPLERATE_500HZ 15 /*!< Sample rate set to 500 Hz */
#define MPU6050_SAMPLERATE_250HZ 31 /*!< Sample rate set to 250 Hz */
#define MPU6050_SAMPLERATE_125HZ 63 /*!< Sample rate set to 125 Hz */
#define MPU6050_SAMPLERATE_100HZ 79 /*!< Sample rate set to 100 Hz */

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief  Main MPU6050 structure
 */
typedef struct mpu6050_s
{
  /* Private */
  uint8_t address;  /*!< I2C address of device. */
  float gyro_mult;  /*!< Gyroscope corrector from raw data to "degrees/s". Only for private use */
  float accel_mult; /*!< Accelerometer corrector from raw data to "g". Only for private use */

  /* Public */
  int16_t accel_x_raw; /*!< Accelerometer value X axis RAW */
  int16_t accel_y_raw; /*!< Accelerometer value Y axis RAW */
  int16_t accel_z_raw; /*!< Accelerometer value Z axis RAW */
  int16_t gyro_x_raw;  /*!< Gyroscope value X axis RAW*/
  int16_t gyro_y_raw;  /*!< Gyroscope value Y axis RAW*/
  int16_t gyro_z_raw;  /*!< Gyroscope value Z axis RAW*/

  float accelerometer_x; /*!< Accelerometer value X axis */
  float accelerometer_y; /*!< Accelerometer value Y axis */
  float accelerometer_z; /*!< Accelerometer value Z axis */
  float gyroscope_x;     /*!< Gyroscope value X axis */
  float gyroscope_y;     /*!< Gyroscope value Y axis */
  float gyroscope_z;     /*!< Gyroscope value Z axis */
  float Temperature;     /*!< Temperature in degrees */
  // MPU6050 Function Pointer
  bsp_i2c_status_t (*i2c1_is_device_ready)(uint8_t dev_addr, uint32_t trials, uint32_t timeout);
  bsp_i2c_status_t (
    *i2c1_mem_read)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout);
  bsp_i2c_status_t (
    *i2c1_mem_write)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *p_data, uint16_t len, uint32_t timeout);
} mpu6050_t;

/**
 * @brief  MPU6050 status enumeration
 */
typedef enum mpu6050_status_s
{
  MPU6050_STATUS_OK = 0x00,          /*!< Everything OK */
  MPU6050_STATUS_ERROR,              /*!< Unknown error */
  MPU6050_STATUS_DEVICENOTCONNECTED, /*!< There is no device with valid slave address */
  MPU6050_STATUS_DEVICEINVALID       /*!< Connected device with address is not MPU6050 */
} mpu6050_status_t;

/**
 * @brief  Parameters for accelerometer range
 */
typedef enum mpu6050_accelerometer_s
{
  MPU6050_ACCELEROMETER_2G = 0x00, /*!< Range is +- 2G */
  MPU6050_ACCELEROMETER_4G = 0x01, /*!< Range is +- 4G */
  MPU6050_ACCELEROMETER_8G = 0x02, /*!< Range is +- 8G */
  MPU6050_ACCELEROMETER_16G = 0x03 /*!< Range is +- 16G */
} mpu6050_accelerometer_t;

/**
 * @brief  Parameters for gyroscope range
 */
typedef enum mpu6050_gyroscope_s
{
  MPU6050_GYROSCOPE_250S = 0x00,  /*!< Range is +- 250 degrees/s */
  MPU6050_GYROSCOPE_500S = 0x01,  /*!< Range is +- 500 degrees/s */
  MPU6050_GYROSCOPE_1000S = 0x02, /*!< Range is +- 1000 degrees/s */
  MPU6050_GYROSCOPE_2000S = 0x03  /*!< Range is +- 2000 degrees/s */
} mpu6050_gyroscope_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief The function initializes the MPU6050 sensor by checking its readiness, configuring power management,
 * sample rate, accelerometer, and gyroscope settings.
 *
 * @param mpu6050 A pointer to the MPU6050 structure that holds the configuration and status of the
 * MPU6050 sensor.
 * @param pwr_mng The pwr_mng parameter is used to configure the power management settings of the
 * MPU6050 sensor. It determines how the sensor operates in terms of sleep mode, clock source, and
 * temperature sensor.
 * @param sample_rate The sample_rate parameter is used to configure the data rate of the MPU6050
 * sensor. It determines how frequently the sensor will update its measurements. The value of
 * sample_rate should be within the range of 0 to 7, where each value corresponds to a specific sample
 * rate as follows:
 * @param accel_sens The `accel_sens` parameter is used to specify the sensitivity of the accelerometer
 * in the MPU6050 sensor. It is of type `mpu6050_accelerometer_t`, which is an enumeration type that
 * defines different sensitivity options for the accelerometer. The available options depend on the
 * specific implementation of
 * @param gyro_sens The `gyro_sens` parameter is used to specify the sensitivity of the gyroscope in
 * the MPU6050 sensor. It is of type `mpu6050_gyroscope_t`, which is an enumeration type that defines
 * different sensitivity options for the gyroscope. The available options depend on the
 *
 * @return The function `mpu6050_init` returns an `mpu6050_status_t` value. The possible return values
 * are:
 */
mpu6050_status_t mpu6050_init(mpu6050_t *mpu6050,
                              uint8_t pwr_mng,
                              uint8_t sample_rate,
                              mpu6050_accelerometer_t accel_sens,
                              mpu6050_gyroscope_t gyro_sens);

/**
 * @brief The function mpu6050_read_accel reads accelerometer data from the MPU6050 sensor and converts it
 * into acceleration values in 'g'.
 *
 * @param mpu6050 The parameter "mpu6050" is a pointer to a structure of type "mpu6050_t". This
 * structure contains various variables and functions related to the MPU6050 sensor.
 */
void mpu6050_read_accel(mpu6050_t *mpu6050);

/**
 * @brief The function mpu6050_read_gyro reads gyroscopic data from the MPU6050 sensor and converts it into
 * degrees per second.
 *
 * @param mpu6050 The parameter "mpu6050" is a pointer to a structure of type "mpu6050_t". This
 * structure contains various variables related to the MPU6050 sensor, such as the I2C interface used
 * for communication, the raw gyro data, and the converted gyro data.
 */
void mpu6050_read_gyro(mpu6050_t *mpu6050);

#endif /* BSP_MPU_H_ */
/* End of file -------------------------------------------------------- */
