/**
 * @file       bsp_mpu.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Dung Nguyen
 * @brief      Board Support Package for mpu6050
 * @note       Device address has been shifted left
 */

/* Includes ----------------------------------------------------------- */
#include "driver_mpu6050.h"

/* Private defines ---------------------------------------------------- */
/* DLPF_CFG Register */
#define DLPF_CFG 0x1A
#define DLPF_CFG_5Hz 0x06

/* Gyro sensitivities in degrees/s */
#define MPU6050_GYRO_SENS_250 ((float) 131)
#define MPU6050_GYRO_SENS_500 ((float) 65.5)
#define MPU6050_GYRO_SENS_1000 ((float) 32.8)
#define MPU6050_GYRO_SENS_2000 ((float) 16.4)

/* Acce sensitivities in g/s */
#define MPU6050_ACCE_SENS_2 ((float) 16384)
#define MPU6050_ACCE_SENS_4 ((float) 8192)
#define MPU6050_ACCE_SENS_8 ((float) 4096)
#define MPU6050_ACCE_SENS_16 ((float) 2048)

/* Set Timeout for I2C */
#define TIMEOUT 500

/* Number of Trial for Device */
#define TRIALS 5

/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
extern I2C_HandleTypeDef hi2c1;

/* Private function prototypes ---------------------------------------- */
static mpu6050_status_t mpu6050_set_accelerometer(mpu6050_t *mpu6050, mpu6050_accelerometer_t accel_sens);
static mpu6050_status_t mpu6050_set_gyroscope(mpu6050_t *mpu6050, mpu6050_gyroscope_t gyro_sens);
static mpu6050_status_t mpu6050_power_management_config(mpu6050_t *mpu6050, uint8_t pwr_mng);
static mpu6050_status_t mpu6050_sample_rate_config(mpu6050_t *mpu6050, uint8_t sample_rate);

/* Public Function definitions -----------------------------------------------
 */
mpu6050_status_t mpu6050_init(mpu6050_t *mpu6050,
                              uint8_t pwr_mng,
                              uint8_t sample_rate,
                              mpu6050_accelerometer_t accel_sens,
                              mpu6050_gyroscope_t gyro_sens)
{
  uint8_t check = 0;
  mpu6050->i2c1_is_device_ready = bsp_i2c1_is_device_ready;
  mpu6050->i2c1_mem_read = bsp_i2c1_mem_read;
  mpu6050->i2c1_mem_write = bsp_i2c1_write_mem;
  // check device ready or not
  if (mpu6050->i2c1_is_device_ready(MPU6050_ADDR, TRIALS, TIMEOUT) != BSP_I2C_STATUS_OK)
  {
    return BSP_I2C_STATUS_ERROR;
  }
  // check device ID WHO_AM_I
  mpu6050->i2c1_mem_read(MPU6050_ADDR, WHO_AM_I_REG, &check, 1, TIMEOUT);

  // 0x68 will be returned by the sensor if everything goes well
  if (check == 104)
  {
    // power management register 0X6B we should write all 0's to wake the sensor
    if (mpu6050_power_management_config(mpu6050, pwr_mng) != MPU6050_STATUS_OK)
    {
      return MPU6050_STATUS_ERROR;
    }

    // Set DATA RATE by writing SMPLRT_DIV register
    if (mpu6050_sample_rate_config(mpu6050, sample_rate) != MPU6050_STATUS_OK)
    {
      return MPU6050_STATUS_ERROR;
    }

    /* Config Accelerometer */
    if (mpu6050_set_accelerometer(mpu6050, accel_sens) != MPU6050_STATUS_OK)
    {
      return MPU6050_STATUS_ERROR;
    }

    /* Config Gyroscope */
    if (mpu6050_set_gyroscope(mpu6050, gyro_sens) != MPU6050_STATUS_OK)
    {
      return MPU6050_STATUS_ERROR;
    }

    /* DLPF_CFG with bandwidth 5Hz */
    mpu6050->i2c1_mem_write(MPU6050_ADDR, DLPF_CFG, (uint8_t *) DLPF_CFG_5Hz, 1, TIMEOUT);

    return MPU6050_STATUS_OK;
  }
  return MPU6050_STATUS_DEVICENOTCONNECTED;
}

void mpu6050_read_accel(mpu6050_t *mpu6050)
{
  uint8_t rec_data[6];

  // Read 6 BYTES of data starting from ACCEL_XOUT_H register
  mpu6050->i2c1_mem_read(MPU6050_ADDR, ACCEL_XOUT_H_REG, rec_data, 6, TIMEOUT);

  // Get Accelerometer Data from register
  mpu6050->accel_x_raw = (int16_t)(rec_data[0] << 8 | rec_data[1]);
  mpu6050->accel_y_raw = (int16_t)(rec_data[2] << 8 | rec_data[3]);
  mpu6050->accel_z_raw = (int16_t)(rec_data[4] << 8 | rec_data[5]);

  /*** convert the RAW values into acceleration in 'g'
       we have to divide according to the Full scale value set in FS_SEL
       I have configured FS_SEL = 0. So I am dividing by @ref accel_mult
       for more details check ACCEL_CONFIG Register              ****/
  mpu6050->accelerometer_x = mpu6050->accel_x_raw / mpu6050->accel_mult;
  mpu6050->accelerometer_y = mpu6050->accel_y_raw / mpu6050->accel_mult;
  mpu6050->accelerometer_z = mpu6050->accel_z_raw / mpu6050->accel_mult;
}

void mpu6050_read_gyro(mpu6050_t *mpu6050)
{
  uint8_t rec_data[6];

  // Read 6 BYTES of data starting from GYRO_XOUT_H register
  mpu6050->i2c1_mem_read(MPU6050_ADDR, GYRO_XOUT_H_REG, rec_data, 6, TIMEOUT);

  // Get Gyroscope Data from register
  mpu6050->gyro_x_raw = (int16_t)(rec_data[0] << 8 | rec_data[1]);
  mpu6050->gyro_y_raw = (int16_t)(rec_data[2] << 8 | rec_data[3]);
  mpu6050->gyro_z_raw = (int16_t)(rec_data[4] << 8 | rec_data[5]);

  /*** convert the RAW values into dps (degrees/s)
       we have to divide according to the Full scale value set in FS_SEL
       I have configured FS_SEL = 0. So I am dividing by @ref gyro_mult
       for more details check GYRO_CONFIG Register              ****/
  mpu6050->gyroscope_x = mpu6050->gyro_x_raw / mpu6050->gyro_mult;
  mpu6050->gyroscope_y = mpu6050->gyro_y_raw / mpu6050->gyro_mult;
  mpu6050->gyroscope_z = mpu6050->gyro_z_raw / mpu6050->gyro_mult;
}

/* Private Function definitions -----------------------------------------------
 */
/**
 * @brief The function configures the power management settings of an MPU6050
 * sensor.
 *
 * @param mpu6050 A pointer to the MPU6050 structure that contains the necessary
 * information for communication with the MPU6050 sensor.
 * @param pwr_mng The pwr_mng parameter is a uint8_t variable that represents
 * the power management configuration for the MPU6050 sensor. It is used to
 * control the power mode and clock source of the sensor.
 *
 * @return an mpu6050_status_t value, specifically MPU6050_STATUS_OK.
 */
static mpu6050_status_t mpu6050_power_management_config(mpu6050_t *mpu6050, uint8_t pwr_mng)
{
  mpu6050->i2c1_mem_write(MPU6050_ADDR, PWR_MGMT_1_REG, (uint8_t *) &pwr_mng, 1, TIMEOUT);
  return MPU6050_STATUS_OK;
}

/**
 * @brief The function configures the sample rate of an MPU6050 sensor.
 *
 * @param mpu6050 A pointer to an instance of the MPU6050 structure, which
 * contains the necessary information for communicating with the MPU6050 sensor.
 * @param sample_rate The sample_rate parameter is the desired sample rate for
 * the MPU6050 sensor. It specifies how frequently the sensor should take
 * measurements and update its registers. The value of sample_rate should be a
 * uint8_t type, which means it should be an unsigned 8-bit integer.
 *
 * @return an mpu6050_status_t value, specifically MPU6050_STATUS_OK.
 */
static mpu6050_status_t mpu6050_sample_rate_config(mpu6050_t *mpu6050, uint8_t sample_rate)
{
  mpu6050->i2c1_mem_write(MPU6050_ADDR, SMPLRT_DIV_REG, (uint8_t *) &sample_rate, 1, TIMEOUT);
  return MPU6050_STATUS_OK;
}

/**
 * @brief The function sets the accelerometer sensitivity of an MPU6050 sensor
 *based on the desired range.
 *
 * @param mpu6050 A pointer to the MPU6050 structure that contains the necessary
 *information for communication with the MPU6050 sensor.
 * @param accel_sens The accel_sens parameter is an enumeration type that
 *represents the desired accelerometer sensitivity for the MPU6050 sensor. It
 *can take one of the following values:
 * @note 		2g  ~  0000 0000  ~ 0x00 16,384
 *		 		  4g  ~  0000 1000  ~ 0x08 8192
 *		 		  8g  ~  0001 0000  ~ 0x10 4096
 *		 		  16g ~  0001 1000  ~ 0x18 2048
 * @return an mpu6050_status_t value, specifically MPU6050_STATUS_OK.
 */
static mpu6050_status_t mpu6050_set_accelerometer(mpu6050_t *mpu6050, mpu6050_accelerometer_t accel_sens)
{
  uint8_t reg_add = (uint8_t) ACCEL_CONFIG_REG;
  uint8_t data;
  switch (accel_sens)
  {
  case MPU6050_ACCELEROMETER_2G:
    mpu6050->accel_mult = MPU6050_ACCE_SENS_2;
    data = MPU6050_ACCE_SENS_REG_2;
    break;
  case MPU6050_ACCELEROMETER_4G:
    mpu6050->accel_mult = MPU6050_ACCE_SENS_4;
    data = MPU6050_ACCE_SENS_REG_4;
    break;
  case MPU6050_ACCELEROMETER_8G:
    mpu6050->accel_mult = MPU6050_ACCE_SENS_8;
    data = MPU6050_ACCE_SENS_REG_8;
    break;
  case MPU6050_ACCELEROMETER_16G:
    mpu6050->accel_mult = MPU6050_ACCE_SENS_16;
    data = MPU6050_ACCE_SENS_REG_16;
    break;
  }
  mpu6050->i2c1_mem_write(MPU6050_ADDR, reg_add, &data, 1, TIMEOUT);
  return MPU6050_STATUS_OK;
}

/**
 * @brief The function sets the gyroscope sensitivity of an MPU6050 sensor based
 *on the input parameter.
 *
 * @param mpu6050 A pointer to the MPU6050 structure that contains the necessary
 *information for communication with the MPU6050 sensor.
 * @param gyro_sens The parameter "gyro_sens" is an enumeration type that
 *represents the desired gyroscope sensitivity. It can take one of the following
 *values:
 * @note  		250 degrees/s      ~  0000 0000  ~ 0x00 131
 *				    500 degrees/s      ~  0000 1000  ~ 0x08 65.5
 *				    1000 degrees/s   	 ~  0001 0000  ~
 *0x10 32.8 2000 degrees/s     ~  0001 1000  ~ 0x18 16.4
 * @return an mpu6050_status_t value, specifically MPU6050_STATUS_OK.
 */
static mpu6050_status_t mpu6050_set_gyroscope(mpu6050_t *mpu6050, mpu6050_gyroscope_t gyro_sens)
{
  uint8_t reg_add = (uint8_t) GYRO_CONFIG_REG;
  uint8_t data;
  switch (gyro_sens)
  {
  case MPU6050_GYROSCOPE_250S:
    mpu6050->gyro_mult = MPU6050_GYRO_SENS_250;
    data = MPU6050_GYRO_SENS_REG_250;
    break;
  case MPU6050_GYROSCOPE_500S:
    mpu6050->gyro_mult = MPU6050_GYRO_SENS_500;
    data = MPU6050_GYRO_SENS_REG_500;
    break;
  case MPU6050_GYROSCOPE_1000S:
    mpu6050->gyro_mult = MPU6050_GYRO_SENS_1000;
    data = MPU6050_GYRO_SENS_REG_1000;
    break;
  case MPU6050_GYROSCOPE_2000S:
    mpu6050->gyro_mult = MPU6050_GYRO_SENS_2000;
    data = MPU6050_GYRO_SENS_REG_2000;
    break;
  }
  mpu6050->i2c1_mem_write(MPU6050_ADDR, reg_add, &data, 1, TIMEOUT);
  return MPU6050_STATUS_OK;
}
/* End of file -------------------------------------------------------- */
