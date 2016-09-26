 /******************************************************************************
  * @file       USER/spi_app.c
  * @author  cjw
  * @date     2016.9.22
  * @brief     This file contains the source of the Spi_app.
  ******************************************************************************/

#include "spi_app.h"
#include "crc.h"
#include "gpio.h"

/**
  * @brief  This function is Init SPI0.
  * @param  None
  * @retval None
  */

void SPI_APP_Init(void)
{
    SPI_ConfigType sSPIConfig = {0};
        /* initialize SPI0 as master    */
    sSPIConfig.u32BitRate = SPI_BIT_RATE;
    sSPIConfig.u32BusClkHz = BUS_CLK_HZ;
    sSPIConfig.sSettings.bModuleEn             = 1;
    sSPIConfig.sSettings.bMasterMode           = 1;
    sSPIConfig.sSettings.bClkPhase1            = 1;    //CPHA = 1
    sSPIConfig.sSettings.bMasterAutoDriveSS    = 1;
    sSPIConfig.sSettings.bClkPolarityLow = 0;    //CPOL = 0

    SPI_Init(SPI0, &sSPIConfig);
    GPIO_Init(GPIOA, GPIO_PTC5_MASK, GPIO_PinOutput);
}

/**
  * @brief  This function is set low SPI_En.
  * @param  None
  * @retval None
  */
  
void SPI_Dis(void)
{
    GPIO_PinClear(GPIO_PTC5);
}

/**
  * @brief  This function is set high SPI_En.
  * @param  None
  * @retval None
  */
  
void SPI_En(void)
{
    GPIO_PinSet(GPIO_PTC5);
}

/**
  * @brief  This function is read a 16 bit data from SPI slave(AN49506).
  * @param  uint8_t devadd,uint8_t regadd,uint16_t* data
  * @retval 0 is success , 1 is error
  */
  
uint8_t SPI_Read(uint8_t devadd,uint8_t regadd,uint16_t* data)
{
    SPI_WidthType transdata[3] = {0};
    SPI_WidthType receivedata[3] = {0};
    uint8_t crc = 0;

    SPI_En();
    transdata[0] = devadd+1 ;    //settle the transdata
    transdata[1] = regadd<<1 ;
    transdata[2] = CRC_Calc(2, transdata);

    SPI_TransferWait(SPI0, receivedata, transdata, 3);    //trans data

    memset(receivedata,0x00,3);
    memset(transdata,0xff,3);

    SPI_TransferWait(SPI0, receivedata, transdata, 3);    //receive data
    SPI_Dis();

    crc = CRC_Calc(2,receivedata) ;
    if( crc != receivedata[2])
    {
        return 1;
    }
    *data = ( receivedata[0] <<8 ) | receivedata[1];
    return 0;
}

/**
  * @brief  This function is write a 16 bit data to SPI slave(AN49506).
  * @param  uint8_t devadd,uint8_t regadd,uint16_t data
  * @retval  0 is success , 1 is error
  */
  
uint8_t SPI_Write(uint8_t devadd,uint8_t regadd,uint16_t data)
{
    SPI_WidthType transdata[5] = {0};
    SPI_WidthType receivedata[5] = {0};
    uint8_t crc = 0;
    uint16_t data_temp=0;
    SPI_En();

    transdata[0] = devadd ;    //settle the transdata
    transdata[1] = regadd<<1 ;
    transdata[2] = data>>8 ;
    transdata[3] = data & 0xff ;
    transdata[4] = CRC_Calc(4, transdata);

    SPI_TransferWait(SPI0, receivedata, transdata, 5);    //trans data
    
    SPI_Dis();

    if(SPI_Read(devadd,regadd,&data_temp))
    {
        return 1;
    }
    else if( data_temp != data )
    {
        return 1;
    }
    return 0;
}

/**
  * @brief  This function is read a 16*time data from SPI slave(AN49506).
  * @param  uint8_t devadd,uint8_t regadd,uint8_t time
  * @retval  0 is success , 1 is error
  */
  
uint8_t SPI_Continue_Read(uint8_t devadd,uint8_t regadd, uint8_t time)
{
    SPI_WidthType transdata[AN49503_READ_CNT*2+1] = {0};
    SPI_WidthType receivedata[4] = {0};
    uint8_t crc = 0;

    SPI_En();
    transdata[0] = devadd + 1;    //settle the transdata
    transdata[1] = (regadd << 1)+1;
    transdata[2] = (time & 0x7f) -1;
    transdata[3] = CRC_Calc(3, transdata);

    SPI_TransferWait(SPI0, receivedata, transdata, 4);    //trans data
    
    memset(transdata,0xff,AN49503_READ_CNT*2+1);
    SPI_TransferWait(SPI0, SPI_Conti_RecvData, transdata, (time & 0x7f)*2+1); // 8bit*2 + CRC
    SPI_Dis();

    crc = CRC_Calc((time & 0x7f)*2,SPI_Conti_RecvData);
    if( crc != SPI_Conti_RecvData[(time & 0x7f)*2])
    {
        return 1;
    }
    return 0;
}




