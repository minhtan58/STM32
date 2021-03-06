/*
 * W5500HardwareDriver.h
 *
 *  Created on: Nov 4, 2020
 *      Author: Admin
 */

#ifndef INC_W5500HARDWAREDRIVER_H_
#define INC_W5500HARDWAREDRIVER_H_

#define W5500_SPI_CLOCK_SPEED

#define W5500_SPI				SPI1
#define W5500_SPI_RCC 			RCC_APB2Periph_SPI1
#define W5500_GPIO_RCC			RCC_APB2Periph_GPIOA||RCC_APB2Periph_GPIOB

#define W5500_RESET_PORT		GPIOA
#define W5500_RESET_PIN			GPIO_Pin_0//GPIO_Pin_10

#define W5500_INT_PORT 			GPIOB//GPIOD
#define W5500_INT_PIN			GPIO_Pin_3//GPIO_Pin_11

#define W5500_CS_PORT			GPIOD
#define W5500_CS_PIN			GPIO_Pin_2

#define W5500_CLK_PORT			GPIOA
#define W5500_CLK_PIN			GPIO_Pin_5

#define W5500_MISO_PORT			GPIOA
#define W5500_MISO_PIN			GPIO_Pin_6

#define W5500_MOSI_PORT			GPIOA
#define W5500_MOSI_PIN			GPIO_Pin_7

void W5500HardwareInitilize(void);
void W5500Initialze(void);
void W5500HardwareReset(void);
void W5500WriteByte(unsigned char byte);
unsigned char W5500ReadByte(void);
void W5500Select(void);
void W5500DeSelect(void);
uint8_t wizchip_read(void);
void  wizchip_write(uint8_t wb);


#endif /* INC_W5500HARDWAREDRIVER_H_ */
