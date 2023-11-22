/**
 * @file       bsp_mfrc522.c
 * @version    0.1.0
 * @date       2023-08-23
 * @author     Dung Nguyen
 *
 * @brief      BSP MFRC522 Function
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_mfrc522.h"

/* Private defines ---------------------------------------------------- */
/**
 * @brief MFRC522 Check HAL Status
 *
 */
#define bsp_mfrc522_check_hal(x) \
  if ((x) != HAL_OK)             \
  {                              \
    return BSP_MFRC522_ERROR;    \
  }

/**
 * @brief MFRC522 Check Status
 *
 */
#define bsp_mfrc522_check_status(x) \
  if ((x) != BSP_MFRC522_OK)        \
  {                                 \
    return BSP_MFRC522_ERROR;       \
  }
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
static bsp_mfrc522_status_t bsp_mfrc522_antenna_on(void);
static void bsp_mfrc522_calculate_crc(uint8_t *data_in, uint8_t len, uint8_t *data_out);
static bsp_mfrc522_status_t bsp_mfrc522_cs(uint8_t bit);
static bsp_mfrc522_status_t bsp_mfrc522_write_reg(uint8_t reg, uint8_t data);
static uint8_t bsp_mfrc522_read_reg(uint8_t reg);
static bsp_mfrc522_status_t bsp_mfrc522_set_bit_reg(uint8_t reg, uint8_t bit);
static bsp_mfrc522_status_t bsp_mfrc522_clear_bit_reg(uint8_t reg, uint8_t bit);
static bsp_mfrc522_status_t bsp_mfrc522_to_card(uint8_t command,
                                                uint8_t *send_data,
                                                uint8_t send_len,
                                                uint8_t *back_data,
                                                uint16_t *back_len);
/* Function definitions ----------------------------------------------- */
bsp_mfrc522_status_t bsp_mfrc522_init(void)
{
  bsp_mfrc522_cs(1);

  // Reset MFRC522
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_COMMAND, BSP_PCD_RESETPHASE);

  bsp_delay_tim_ms(1);

  // defines settings for the internal timer
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_T_MODE, 0x8D);
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_T_PRESCALER, 0x3E);

  // defines the 16-bit timer reload value
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_T_RELOAD_L, 30);
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_T_RELOAD_H, 0);

  /* 48dB gain */
  /* configures the receiver gain */
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_RF_CFG, 0x70);

  /* controls the setting of the transmission modulation */
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_TX_AUTO, 0x40);

  /* defines general modes for transmitting and receiving */
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_MODE, 0x3D);

  bsp_mfrc522_antenna_on(); /* open the antenna */

  return BSP_MFRC522_OK;
}

bsp_mfrc522_status_t bsp_mfrc522_request(uint8_t request_mode, uint8_t *tag_type)
{
  bsp_mfrc522_status_t status;
  uint16_t back_bits;

  bsp_mfrc522_write_reg(BSP_MFRC522_REG_BIT_FRAMING, 0x07);

  tag_type[0] = request_mode;
  status = bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, tag_type, 1, tag_type, &back_bits);

  if ((status != BSP_MFRC522_OK) || (back_bits != 0x10))
  {
    status = BSP_MFRC522_ERROR;
  }

  return status;
}

bsp_mfrc522_status_t bsp_mfrc522_anticoll(uint8_t *data)
{
  bsp_mfrc522_status_t status;
  uint8_t i;
  uint8_t num_check = 0;
  uint16_t len;

  bsp_mfrc522_write_reg(BSP_MFRC522_REG_BIT_FRAMING, 0x00);

  data[0] = BSP_PICC_ANTICOLL;
  data[1] = 0x20;
  status = bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, data, 2, data, &len);

  if (status == BSP_MFRC522_OK)
  {
    /* check card serial number */
    for (i = 0; i < 4; i++)
    {
      num_check ^= data[i];
    }
    if (num_check != data[i])
    {
      status = BSP_MFRC522_ERROR;
    }
  }
  return status;
}

bsp_mfrc522_status_t bsp_mfrc522_haft(void)
{
  uint16_t len;
  uint8_t buff[4];

  buff[0] = BSP_PICC_HALT;
  buff[1] = 0;
  bsp_mfrc522_calculate_crc(buff, 2, &buff[2]);

  return bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, buff, 4, buff, &len);
}

bsp_mfrc522_status_t bsp_mfrc522_read_address(uint8_t block_address, uint8_t *receive_data)
{
  bsp_mfrc522_status_t status;
  uint16_t len;
  receive_data[0] = BSP_PICC_READ;
  receive_data[1] = block_address;
  bsp_mfrc522_calculate_crc(receive_data, 2, &receive_data[2]);
  status = bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, receive_data, 4, receive_data, &len);

  if ((status != BSP_MFRC522_OK) || (len != 0x90))
  {
    status = BSP_MFRC522_ERROR;
  }
  return status;
}

bsp_mfrc522_status_t bsp_mfrc522_write_address(uint8_t block_address, uint8_t *send_data)
{
  bsp_mfrc522_status_t status;
  uint16_t receive_bits;
  uint8_t i;
  uint8_t buff[18];

  buff[0] = BSP_PICC_WRITE;
  buff[1] = block_address;
  bsp_mfrc522_calculate_crc(buff, 2, &buff[2]);
  status = bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, buff, 4, buff, &receive_bits);

  if ((status != BSP_MFRC522_OK) || (receive_bits != 4) || ((buff[0] & 0x0F) != 0x0A))
  {
    status = BSP_MFRC522_ERROR;
  }
  if (status == BSP_MFRC522_OK)
  {
    for (i = 0; i < 16; i++)
    {
      buff[i] = *(send_data + i);
    }
    bsp_mfrc522_calculate_crc(buff, 16, &buff[16]);
    status = bsp_mfrc522_to_card(BSP_PCD_TRANSCEIVE, buff, 18, buff, &receive_bits);

    if ((status != BSP_MFRC522_OK) || (receive_bits != 4) || ((buff[0] & 0x0F) != 0x0A))
    {
      status = BSP_MFRC522_ERROR;
    }
  }
  return status;
}

/**
 * The function `bsp_mfrc522_antenna_on` turns on the antenna of the MFRC522 module.
 *
 * @return a value of type "bsp_mfrc522_status_t".
 */
static bsp_mfrc522_status_t bsp_mfrc522_antenna_on(void)
{
  uint8_t temp = bsp_mfrc522_read_reg(BSP_MFRC522_REG_TX_CONTROL);
  return bsp_mfrc522_write_reg(BSP_MFRC522_REG_TX_CONTROL, temp | 0x03);
}

/**
 * The function calculates the CRC (Cyclic Redundancy Check) of a given data array using the MFRC522
 * RFID reader.
 *
 * @param data_in A pointer to the input data array that needs to be used for CRC calculation.
 * @param len The parameter "len" represents the length of the data_in array, which is the number of
 * bytes to be used for CRC calculation.
 * @param data_out The `data_out` parameter is a pointer to an array of 2 bytes. This is where the
 * calculated CRC value will be stored. The lower byte of the CRC value will be stored in `data_out[0]`
 * and the higher byte will be stored in `data_out[1]
 */
static void bsp_mfrc522_calculate_crc(uint8_t *data_in, uint8_t len, uint8_t *data_out)
{
  uint8_t i, n;

  bsp_mfrc522_clear_bit_reg(BSP_MFRC522_REG_DIV_IRQ, 0x04);
  bsp_mfrc522_set_bit_reg(BSP_MFRC522_REG_FIFO_LEVEL, 0x80);

  /* writing data to the FIFO */
  for (i = 0; i < len; i++)
  {
    bsp_mfrc522_write_reg(BSP_MFRC522_REG_FIFO_DATA, *(data_in + i));
  }
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_COMMAND, BSP_PCD_CALCCRC);

  i = 0xFF;
  do
  {
    n = bsp_mfrc522_read_reg(BSP_MFRC522_REG_DIV_IRQ);
    i--;
  } while ((i != 0) && !(n & 0x04));

  /* read CRC calculation result */
  data_out[0] = bsp_mfrc522_read_reg(BSP_MFRC522_REG_CRC_RESULT_L);
  data_out[1] = bsp_mfrc522_read_reg(BSP_MFRC522_REG_CRC_RESULT_M);
}

/**
 * The function bsp_mfrc522_cs sets the state of the CS pin on the MFRC522 module.
 *
 * @param bit The "bit" parameter is a uint8_t type variable that represents the value to be written to
 * the MFRC522 chip's CS (Chip Select) pin. It can have a value of either 0 or 1, where 0 represents a
 * logic low (GPIO_PIN_RESET) and
 *
 * @return a value of type "bsp_mfrc522_status_t".
 */
static bsp_mfrc522_status_t bsp_mfrc522_cs(uint8_t bit)
{
  bsp_gpio_write_pin(BSP_MFRC522_CS_PORT, BSP_MFRC522_CS_PIN, (bit ? GPIO_PIN_SET : GPIO_PIN_RESET));
  return BSP_MFRC522_OK;
}

/**
 * The function `bsp_mfrc522_write_reg` writes data to a specified register in the MFRC522 RFID module.
 *
 * @param reg The "reg" parameter is the register address where the data will be written to in the
 * MFRC522 RFID module.
 * @param data The "data" parameter is the value that you want to write to the specified register.
 *
 * @return a value of type `bsp_mfrc522_status_t`, which is likely an enumeration or a typedef for an
 * integer type. The specific value being returned is `BSP_MFRC522_OK`.
 */
static bsp_mfrc522_status_t bsp_mfrc522_write_reg(uint8_t reg, uint8_t data)
{
  uint8_t reg_value = ((reg << 1) & 0x7E);
  bsp_mfrc522_cs(0);
  bsp_mfrc522_check_hal(bsp_spi_transmit(BSP_MFRC522_SPI, &reg_value, 1, BSP_MFRC522_TIMEOUT));
  bsp_mfrc522_check_hal(bsp_spi_transmit(BSP_MFRC522_SPI, &data, 1, BSP_MFRC522_TIMEOUT));
  bsp_mfrc522_cs(1);
  return BSP_MFRC522_OK;
}

/**
 * The function `bsp_mfrc522_read_reg` reads a register value from the MFRC522 RFID module using SPI
 * communication.
 *
 * @param reg The reg parameter is the register address that you want to read from the MFRC522 RFID
 * module.
 *
 * @return the value read from the specified register.
 */
static uint8_t bsp_mfrc522_read_reg(uint8_t reg)
{
  uint8_t reg_value = (((reg << 1) & 0x7E) | 0x80);
  uint8_t value = 0;
  bsp_mfrc522_cs(0);
  bsp_spi_transmit(BSP_MFRC522_SPI, &reg_value, 1, BSP_MFRC522_TIMEOUT);
  bsp_spi_receive(BSP_MFRC522_SPI, &value, 1, BSP_MFRC522_TIMEOUT);
  bsp_mfrc522_cs(1);
  return value;
}

/**
 * The function sets a specific bit in a register of the MFRC522 RFID module.
 *
 * @param reg The reg parameter is a uint8_t type, which means it is an 8-bit unsigned integer. It
 * represents the register address of the MFRC522 module that needs to be accessed or modified.
 * @param bit The "bit" parameter is a uint8_t type, which means it is an 8-bit unsigned integer. It
 * represents a specific bit position in a register that needs to be set.
 *
 * @return a value of type "bsp_mfrc522_status_t".
 */
static bsp_mfrc522_status_t bsp_mfrc522_set_bit_reg(uint8_t reg, uint8_t bit)
{
  return bsp_mfrc522_write_reg(reg, bsp_mfrc522_read_reg(reg) | bit);
}

/**
 * The function clears a specific bit in a register of the MFRC522 RFID module.
 *
 * @param reg The reg parameter is a uint8_t type, which means it is an 8-bit unsigned integer. It
 * represents the register address of the MFRC522 module that needs to be accessed.
 * @param bit The "bit" parameter is the specific bit position that needs to be cleared in the
 * register.
 *
 * @return a value of type `bsp_mfrc522_status_t`.
 */
static bsp_mfrc522_status_t bsp_mfrc522_clear_bit_reg(uint8_t reg, uint8_t bit)
{
  return bsp_mfrc522_write_reg(reg, bsp_mfrc522_read_reg(reg) & (~bit));
}

/**
 * The function `bsp_mfrc522_to_card` is used to send commands and data to an MFRC522 RFID card reader
 * and receive the response.
 *
 * @param command The command to be executed by the MFRC522 module. It can be either BSP_PCD_AUTHENT or
 * BSP_PCD_TRANSCEIVE.
 * @param send_data A pointer to the data that needs to be sent to the card.
 * @param send_len The parameter "send_len" is the length of the data to be sent to the card. It
 * specifies the number of bytes in the "send_data" array that should be sent to the card.
 * @param back_data A pointer to an array where the received data will be stored.
 * @param back_len A pointer to a variable that will store the length of the data received from the
 * card.
 *
 * @return a value of type `bsp_mfrc522_status_t`.
 */
static bsp_mfrc522_status_t bsp_mfrc522_to_card(uint8_t command,
                                                uint8_t *send_data,
                                                uint8_t send_len,
                                                uint8_t *back_data,
                                                uint16_t *back_len)
{
  bsp_mfrc522_status_t status = BSP_MFRC522_ERROR;
  uint8_t irqEn = 0x00;
  uint8_t waitIRq = 0x00;
  uint8_t lastBits;
  uint8_t n;
  uint16_t i;

  switch (command)
  {
  case BSP_PCD_AUTHENT:
  {
    irqEn = 0x12;
    waitIRq = 0x10;
    break;
  }
  case BSP_PCD_TRANSCEIVE:
  {
    irqEn = 0x77;
    waitIRq = 0x30;
    break;
  }
  default: break;
  }

  /* enable interrupt */
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_COMM_I_EN, irqEn | 0x80);
  /* clear interrupt */
  bsp_mfrc522_clear_bit_reg(BSP_MFRC522_REG_COMM_IRQ, 0x80);
  /* clear FIFO */
  bsp_mfrc522_set_bit_reg(BSP_MFRC522_REG_FIFO_LEVEL, 0x80);

  bsp_mfrc522_write_reg(BSP_MFRC522_REG_COMMAND, BSP_PCD_IDLE);

  // Writing data to the FIFO
  for (i = 0; i < send_len; i++)
  {
    bsp_mfrc522_write_reg(BSP_MFRC522_REG_FIFO_DATA, send_data[i]);
  }

  // Execute the command
  bsp_mfrc522_write_reg(BSP_MFRC522_REG_COMMAND, command);
  if (command == BSP_PCD_TRANSCEIVE)
  {
    bsp_mfrc522_set_bit_reg(BSP_MFRC522_REG_BIT_FRAMING, 0x80);
  }

  i = 2000;
  do
  {
    /* Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq */
    n = bsp_mfrc522_read_reg(BSP_MFRC522_REG_COMM_IRQ);
    i--;
  } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

  bsp_mfrc522_clear_bit_reg(BSP_MFRC522_REG_BIT_FRAMING, 0x80);

  if (i != 0)
  {
    if (!(bsp_mfrc522_read_reg(BSP_MFRC522_REG_ERROR) & 0x1B))
    {
      status = BSP_MFRC522_OK;
      if (n & irqEn & 0x01)
      {
        status = BSP_MFRC522_ERROR;
      }

      if (command == BSP_PCD_TRANSCEIVE)
      {
        n = bsp_mfrc522_read_reg(BSP_MFRC522_REG_FIFO_LEVEL);
        lastBits = bsp_mfrc522_read_reg(BSP_MFRC522_REG_CONTROL) & 0x07;
        if (lastBits)
        {
          *back_len = (n - 1) * 8 + lastBits;
        }
        else
        {
          *back_len = n * 8;
        }

        if (n == 0)
        {
          n = 1;
        }
        if (n > BSP_MFRC522_MAX_LEN)
        {
          n = BSP_MFRC522_MAX_LEN;
        }

        /* reading the received data in FIFO */
        for (i = 0; i < n; i++)
        {
          back_data[i] = bsp_mfrc522_read_reg(BSP_MFRC522_REG_FIFO_DATA);
        }
      }
    }
    else
    {
      status = BSP_MFRC522_ERROR;
    }
  }
  return status;
}

/* End of file -------------------------------------------------------- */
