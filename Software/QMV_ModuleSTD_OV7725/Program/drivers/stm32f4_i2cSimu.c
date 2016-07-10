/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_i2cSimu.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define I2CxS_SCL_PIN         SimuI2C_Init->PinSCL
#define I2CxS_SCL_GPIO_PORT   SimuI2C_Init->PortSCL
#define I2CxS_SCL_L()         __GPIO_RST(I2CxS_SCL_GPIO_PORT, I2CxS_SCL_PIN)
#define I2CxS_SCL_H()         __GPIO_SET(I2CxS_SCL_GPIO_PORT, I2CxS_SCL_PIN)

#define I2CxS_SDA_PIN         SimuI2C_Init->PinSDA
#define I2CxS_SDA_GPIO_PORT   SimuI2C_Init->PortSDA
#define I2CxS_SDA_L()         __GPIO_RST(I2CxS_SDA_GPIO_PORT, I2CxS_SDA_PIN)
#define I2CxS_SDA_H()         __GPIO_SET(I2CxS_SDA_GPIO_PORT, I2CxS_SDA_PIN)
#define I2CxS_SDA_READ()      (__GPIO_READ(I2CxS_SDA_GPIO_PORT, I2CxS_SDA_PIN) == I2CxS_SDA_PIN)
/*====================================================================================================*/
/*====================================================================================================*/
static void    SimuI2C_Delay( SimuI2C_InitTypeDef *SimuI2C_Init );
static void    SimuI2C_Start( SimuI2C_InitTypeDef *SimuI2C_Init );
static void    SimuI2C_Stop( SimuI2C_InitTypeDef *SimuI2C_Init );
static void    SimuI2C_SendACK( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t sendACK );
static uint8_t SimuI2C_RecvACK( SimuI2C_InitTypeDef *SimuI2C_Init );
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_Config
**功能 : SimuI2C Config
**輸入 : None
**輸出 : None
**使用 : SimuI2C_Config();
**====================================================================================================*/
/*====================================================================================================*/
void SimuI2C_Config( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  GPIO_InitTypeDef  GPIO_InitStruct; 

  /* GPIO Init *************************************************************/
  GPIO_InitStruct.GPIO_Pin   = SimuI2C_Init->PinSCL;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(SimuI2C_Init->PortSCL, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = SimuI2C_Init->PinSDA;
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(SimuI2C_Init->PortSDA, &GPIO_InitStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_Delay
**功能 : SimuI2C Delay
**輸入 : None
**輸出 : None
**使用 : SimuI2C_Delay(SimuI2C_Init);
**====================================================================================================*/
/*====================================================================================================*/
static void SimuI2C_Delay( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  __IO uint32_t i = SimuI2C_Init->Freguency;

  while(i--);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_Start
**功能 : I2C Start Signal
**輸入 : *SimuI2C_Init
**輸出 : None
**使用 : SimuI2C_Start(SimuI2C_Init);
**====================================================================================================*/
/*====================================================================================================*/
static void SimuI2C_Start( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  I2CxS_SDA_H();
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SCL_H();
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SDA_L();
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SCL_L();
  SimuI2C_Delay(SimuI2C_Init);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_Stop
**功能 : I2C Stop Signal
**輸入 : *SimuI2C_Init
**輸出 : None
**使用 : SimuI2C_Stop(SimuI2C_Init);
**====================================================================================================*/
/*====================================================================================================*/
static void SimuI2C_Stop( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  I2CxS_SDA_L();
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SCL_H();
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SDA_H();
  SimuI2C_Delay(SimuI2C_Init);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_SendACK
**功能 : I2C Send Acknowledge Signal
**輸入 : *SimuI2C_Init, sendACK
**輸出 : None
**使用 : SimuI2C_SendACK(SimuI2C_Init, 1);
**====================================================================================================*/
/*====================================================================================================*/
static void SimuI2C_SendACK( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t sendACK )
{
  I2CxS_SCL_L();
  if(sendACK)
    I2CxS_SDA_H();
  else
    I2CxS_SDA_L();
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SCL_H();
  SimuI2C_Delay(SimuI2C_Init);
  I2CxS_SCL_L();
  SimuI2C_Delay(SimuI2C_Init);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_RecvACK
**功能 : I2C Recv Acknowledge Signal
**輸入 : *SimuI2C_Init
**輸出 : recvACK
**使用 : recvACK = SimuI2C_RecvACK(SimuI2C_Init);
**====================================================================================================*/
/*====================================================================================================*/
static uint8_t SimuI2C_RecvACK( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  uint8_t recvACK = 0;

//  I2CxS_SCL_L();
//  I2CxS_SDA_H();
//  SimuI2C_Delay(SimuI2C_Init);
	I2CxS_SCL_H();
	SimuI2C_Delay(SimuI2C_Init);
  recvACK = I2CxS_SDA_READ();
//  recvACK = I2CxS_SDA_READ() ? ERROR : SUCCESS;
	I2CxS_SCL_L();
	SimuI2C_Delay(SimuI2C_Init);

  return recvACK;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_SendByte
**功能 : I2C Send Byte
**輸入 : *SimuI2C_Init, sendByte
**輸出 : None
**使用 : SimuI2C_SendByte(SimuI2C_Init, sendByte);
**====================================================================================================*/
/*====================================================================================================*/
void SimuI2C_SendByte( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t sendByte )
{
  for(uint8_t i = 8; i > 0; i--) {
    if(sendByte & 0x80) // MSB First
      I2CxS_SDA_H();
    else 
      I2CxS_SDA_L();
    sendByte = sendByte << 1;
    SimuI2C_Delay(SimuI2C_Init);
    I2CxS_SCL_H();
    SimuI2C_Delay(SimuI2C_Init);
    I2CxS_SCL_L();
    SimuI2C_Delay(SimuI2C_Init);
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_RecvByte
**功能 : I2C Recv Byte
**輸入 : *SimuI2C_Init
**輸出 : recvByte
**使用 : recvByte = SimuI2C_RecvByte(SimuI2C_Init);
**====================================================================================================*/
/*====================================================================================================*/
uint8_t SimuI2C_RecvByte( SimuI2C_InitTypeDef *SimuI2C_Init )
{
  uint8_t recvByte = 0;

  for(uint8_t i = 8; i > 0; i--) {
    I2CxS_SCL_H();
    SimuI2C_Delay(SimuI2C_Init);
    recvByte = recvByte << 1;
    if(I2CxS_SDA_READ()) {
      recvByte |= 0x01;
    }
    I2CxS_SCL_L();
    SimuI2C_Delay(SimuI2C_Init);
  }

  return recvByte;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_WriteReg
**功能 : I2C Write Register
**輸入 : *SimuI2C_Init, slaveAddr, registerAddr, writeData
**輸出 : None
**使用 : SimuI2C_WriteReg(SimuI2C_Init, slaveAddr, registerAddr, writeData);
**====================================================================================================*/
/*====================================================================================================*/
uint8_t SimuI2C_WriteReg( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t slaveAddr, uint8_t registerAddr, uint8_t writeData )
{
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr);
  if(SimuI2C_RecvACK(SimuI2C_Init) != 0) {
    SimuI2C_Stop(SimuI2C_Init);
    return ERROR;
  }
  SimuI2C_SendByte(SimuI2C_Init, registerAddr);
  if(SimuI2C_RecvACK(SimuI2C_Init) != 0) {
    SimuI2C_Stop(SimuI2C_Init);
    return ERROR;
  }
  SimuI2C_SendByte(SimuI2C_Init, writeData);
  if(SimuI2C_RecvACK(SimuI2C_Init) != 0) {
    SimuI2C_Stop(SimuI2C_Init);
    return ERROR;
  }
  SimuI2C_Stop(SimuI2C_Init);

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_ReadReg
**功能 : I2C Read Register
**輸入 : *SimuI2C_Init, slaveAddr, registerAddr, *readData
**輸出 : None
**使用 : SimuI2C_ReadReg(SimuI2C_Init, slaveAddr, registerAddr, &readData);
**====================================================================================================*/
/*====================================================================================================*/
uint8_t SimuI2C_ReadReg( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t slaveAddr, uint8_t registerAddr, uint8_t *readData )
{
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr);
  if(SimuI2C_RecvACK(SimuI2C_Init) != 0) {
    SimuI2C_Stop(SimuI2C_Init);
    return ERROR;
  }
  SimuI2C_SendByte(SimuI2C_Init, registerAddr);
  if(SimuI2C_RecvACK(SimuI2C_Init) != 0) {
    SimuI2C_Stop(SimuI2C_Init);
    return ERROR;
  }
  SimuI2C_Stop(SimuI2C_Init);
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr | 0x01);
  if(SimuI2C_RecvACK(SimuI2C_Init) != 0) {
    SimuI2C_Stop(SimuI2C_Init);
    return ERROR;
  }
  *readData = SimuI2C_RecvByte(SimuI2C_Init);
  SimuI2C_SendACK(SimuI2C_Init, 1);
  SimuI2C_Stop(SimuI2C_Init);

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_WriteRegs
**功能 : I2C Write Multiple Registers
**輸入 : *SimuI2C_Init, slaveAddr, registerAddr, writeData, lens
**輸出 : None
**使用 : SimuI2C_WriteRegs(SimuI2C_Init, slaveAddr, registerAddr, writeData, lens);
**====================================================================================================*/
/*====================================================================================================*/
void SimuI2C_WriteRegs( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t slaveAddr, uint8_t registerAddr, uint8_t *writeData, uint8_t lens )
{
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, registerAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  for(uint8_t i = 0; i < lens; i++) {
    SimuI2C_SendByte(SimuI2C_Init, writeData[i]);
    SimuI2C_RecvACK(SimuI2C_Init);
  }
  SimuI2C_Stop(SimuI2C_Init);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : SimuI2C_ReadRegs
**功能 : I2C Read Multiple Registers
**輸入 : *SimuI2C_Init, slaveAddr, registerAddr, *readData, lens
**輸出 : None
**使用 : SimuI2C_ReadRegs(SimuI2C_Init, slaveAddr, registerAddr, readData, lens);
**====================================================================================================*/
/*====================================================================================================*/
void SimuI2C_ReadRegs( SimuI2C_InitTypeDef *SimuI2C_Init, uint8_t slaveAddr, uint8_t registerAddr, uint8_t *readData, uint8_t lens )
{
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, registerAddr);
  SimuI2C_RecvACK(SimuI2C_Init);
  SimuI2C_Stop(SimuI2C_Init);
  SimuI2C_Start(SimuI2C_Init);
  SimuI2C_SendByte(SimuI2C_Init, slaveAddr | 0x01);
  SimuI2C_RecvACK(SimuI2C_Init);
  for(uint8_t i = 0; i < lens; i++) {
    readData[i] = SimuI2C_RecvByte(SimuI2C_Init);
    if(i == lens - 1)
      SimuI2C_SendACK(SimuI2C_Init, 1);
    else
      SimuI2C_SendACK(SimuI2C_Init, 0);
  }
  SimuI2C_Stop(SimuI2C_Init);
}
/*====================================================================================================*/
/*====================================================================================================*/
