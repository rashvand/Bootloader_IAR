#include "main.h"
#include "w25qxx.h"


uint16_t W25QXX_TYPE = 0 ;	 // default is W25Q256



__STATIC_FORCEINLINE uint8_t SPI2_ReadWriteByte (uint8_t byte_s){
  
  SPI2->CR1 |= SPI_CR1_CSTART;
  //	while ((SPI2->SR & SPI_SR_TXP) == 0){}
  *((__IO uint8_t *)&SPI2->TXDR) = byte_s;
  //	while ((SPI2->SR & SPI_FLAG_TXC) == 0){}
  while ((SPI2->SR & SPI_SR_RXPLVL) == LL_SPI_RX_FIFO_0PACKET){}
  return (*((__IO uint8_t *)&SPI2->RXDR));
}

/**
* @brief 
* @param
* @param 
* @retval None
*/													 
uint8_t W25QXX_Init(void)
{ 
  uint8_t temp;
  
  W25QXX_TYPE = W25QXX_ReadID ();	         // Read FLASH ID.
  switch(W25QXX_TYPE)
  {
  case W25Q80 :
    {
      //printf("W25Q80 \r\n");
      temp = W25Q_OK;
      break;
    }
  case W25Q16 :
    {
      //printf("W25Q16 \r\n");
      temp = W25Q_OK;
      break;
    }
  case W25Q32 :
    {
      //printf("W25Q32 \r\n");
      temp = W25Q_OK;
      break;
    }
  case W25Q64 :
    {
      //printf("W25Q64 \r\n");
      temp = W25Q_OK;
      break;
    }
  case W25Q128 :
    {
      ////printf("W25Q128 \r\n");
      temp = W25Q_OK;
      break;
    }
  case W25Q256 :
    {
      //printf("W25Q256 \r\n");
      temp= W25QXX_Read_SR ( 3 );             // Read status register 3, judge address mode
      if ((temp& 0X01 )== 0 )			           // If it is not in 4-byte address mode, enter 4-byte address mode
      {
        W25QXX_CS_LOW; 			             // Select
        SPI2_ReadWriteByte (W25X_Enable4ByteAddr); // Send command to enter 4-byte address mode   
        W25QXX_CS_HIGH;       		         // Cancel chip selection   
      }
      temp = W25Q_OK;
      break;
    }
  default:
    temp = W25Q_ERROR;
    break;
  }
  return temp;
}  

// Read the status register of W25QXX, W25QXX has 3 status registers
// status register 1:
// BIT7 6 5 4 3 2 1 0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
// SPR: default 0, status register protection bit, used with WP
// TB, BP2, BP1, BP0: FLASH area write protection setting
// WEL: write enable lock
// BUSY: busy flag (1, busy; 0, free)
// Default: 0x00
// status register 2:
// BIT7 6 5 4 3 2 1 0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
// status register 3:
// BIT7 6 5 4 3 2 1 0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
// regno: status register number, range: 1~3
// Return value: status register value
uint8_t W25QXX_Read_SR(uint8_t regno)   
{  
  uint8_t byte=0,command=0; 
  switch(regno)
  {
  case 1:
    command=W25X_ReadStatusReg1;     // Read status register 1 command
    break;
  case 2:
    command=W25X_ReadStatusReg2;     // Read status register 2 command
    break;
  case  3 :
    command=W25X_ReadStatusReg3;     // Read status register 3 command
    break;
  default:
    command=W25X_ReadStatusReg1;    
    break;
  }    
  W25QXX_CS_LOW;                            // Enable the device   
  SPI2_ReadWriteByte (command);             // Send read status register command    
  byte= SPI2_ReadWriteByte ( 0Xff );           // read a byte  
  W25QXX_CS_HIGH;                           // Cancel chip selection     
  return byte;   
} 

// write W25QXX status register
void W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
{   
  uint8_t command=0;
  switch(regno)
  {
  case 1:
    command=W25X_WriteStatusReg1;     // write status register 1 command
    break;
  case 2:
    command=W25X_WriteStatusReg2;     // write status register 2 command
    break;
  case  3 :
    command=W25X_WriteStatusReg3;     // write status register 3 command
    break;
  default:
    command=W25X_WriteStatusReg1;    
    break;
  }   
  W25QXX_CS_LOW;                             // Enable the device   
  SPI2_ReadWriteByte (command);             // Send the command to write the status register    
  SPI2_ReadWriteByte (sr);                  // write a byte  
  W25QXX_CS_HIGH;                              // Cancel chip selection     	      
}   
// W25QXX write enable	
// set WEL   
void W25QXX_Write_Enable(void)   
{
  W25QXX_CS_LOW;                             // Enable the device   
  SPI2_ReadWriteByte (W25X_WriteEnable);    // Send write enable  
  W25QXX_CS_HIGH;                              // Cancel chip selection     	      
} 
// W25QXX write inhibit	
// Clear WEL to zero  
void W25QXX_Write_Disable(void)   
{  
  W25QXX_CS_LOW;                             // Enable the device   
  SPI2_ReadWriteByte (W25X_WriteDisable);   // Send write disable command    
  W25QXX_CS_HIGH;                              // Cancel chip selection     	      
}

// Read chip ID
//The return value is as follows:				   
// 0XEF13, indicating that the chip model is W25Q80  
// 0XEF14, indicating that the chip model is W25Q16    
// 0XEF15, means the chip model is W25Q32  
// 0XEF16, indicating that the chip model is W25Q64
// 0XEF17, means the chip model is W25Q128 	  
// 0XEF18, means the chip model is W25Q256
uint16_t W25QXX_ReadID(void)
{
  uint16_t Temp = 0;	  
  W25QXX_CS_LOW;				    
  SPI2_ReadWriteByte ( 0x90 ); // Send read ID command	    
  SPI2_ReadWriteByte(0x00); 	    
  SPI2_ReadWriteByte(0x00); 	    
  SPI2_ReadWriteByte(0x00); 	 			   
  Temp|=SPI2_ReadWriteByte(0xFF)<<8;  
  Temp|=SPI2_ReadWriteByte(0xFF);	 
  W25QXX_CS_HIGH; 				    
  return Temp;
}   		    
// Read SPI FLASH  
// Start reading the data of the specified length at the specified address
// pBuffer: data storage area
// ReadAddr: address to start reading (24bit)
// NumByteToRead: the number of bytes to read (maximum 65535)
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
  uint16_t i;  
  
  W25QXX_CS_LOW;                             // Enable the device   
  SPI2_ReadWriteByte (W25X_ReadData);       // Send read command  
  if (W25QXX_TYPE==W25Q256)                 // If it is W25Q256, the address is 4 bytes, and the highest 8 bits should be sent
  {
    SPI2_ReadWriteByte((uint8_t)((ReadAddr)>>24));    
  }
  SPI2_ReadWriteByte (( uint8_t )((ReadAddr)>> 16 ));    // Send 24bit address    
  SPI2_ReadWriteByte((uint8_t)((ReadAddr)>>8));   
  SPI2_ReadWriteByte((uint8_t)ReadAddr);   
  for(i=0;i<NumByteToRead;i++)
  { 
    pBuffer[i]= SPI2_ReadWriteByte ( 0XFF );     // Cycle reading  
  }
  W25QXX_CS_HIGH;   				    	      
}  
// SPI writes less than 256 bytes of data in one page (0~65535)
// Start writing a maximum of 256 bytes of data at the specified address
// pBuffer: data storage area
// WriteAddr: address to start writing (24bit)
// NumByteToWrite: the number of bytes to be written (maximum 256), the number should not exceed the remaining bytes of the page!!!	 
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
  uint16_t i;  
  
  W25QXX_Write_Enable();                  //SET WEL 
  W25QXX_CS_LOW;                            // Enable the device   
  SPI2_ReadWriteByte (W25X_PageProgram);    // Send write page command   
  if (W25QXX_TYPE==W25Q256)                 // If it is W25Q256, the address is 4 bytes, and the highest 8 bits should be sent
  {
    SPI2_ReadWriteByte((uint8_t)((WriteAddr)>>24)); 
  }
  SPI2_ReadWriteByte (( uint8_t )((WriteAddr)>> 16 )); // Send 24bit address    
  SPI2_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
  SPI2_ReadWriteByte((uint8_t)WriteAddr);   
  for (i= 0 ;i<NumByteToWrite;i++) SPI2_ReadWriteByte (pBuffer[i]); // cycle write number  
  W25QXX_CS_HIGH;                              // Cancel chip selection
  W25QXX_Wait_Busy ();					    // Waiting for the end of writing
} 
// Write SPI FLASH without verification
// Must ensure that the data in the address range written is all 0XFF, otherwise the data written at non-0XFF will fail!
// With automatic page change function
// Start writing data of the specified length at the specified address, but make sure the address does not cross the boundary!
// pBuffer: data storage area
// WriteAddr: address to start writing (24bit)
// NumByteToWrite: the number of bytes to write (maximum 65535)
//CHECK OK
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
  uint16_t pageremain;	
  
  pageremain= 256 -WriteAddr% 256 ; // The remaining bytes of a single page		 	    
  if (NumByteToWrite<=pageremain)pageremain=NumByteToWrite; // No more than 256 bytes
  while(1)
  {	   
    W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
    if (NumByteToWrite==pageremain) break ; // end of writing
    else //NumByteToWrite>pageremain
    {
      pBuffer+=pageremain;
      WriteAddr+=pageremain;	
      
      NumByteToWrite-=pageremain;			   // Subtract the number of bytes already written
      if (NumByteToWrite> 256 )pageremain= 256 ; // can write 256 bytes at a time
      else pageremain=NumByteToWrite; 	   // Not enough 256 bytes
    }
  };	    
} 
// write SPI FLASH  
// Start writing data of the specified length at the specified address
// This function takes erase operation!
// pBuffer: data storage area
// WriteAddr: address to start writing (24bit)						
// NumByteToWrite: the number of bytes to write (maximum 65535)   
uint8_t W25QXX_BUFFER[4096];		 
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
  uint32_t secpos;
  uint16_t secoff;
  uint16_t secremain;	   
  uint16_t i;    
  uint8_t * W25QXX_BUF;	  
  
  W25QXX_BUF=W25QXX_BUFFER;	     
  secpos=WriteAddr/ 4096 ; // Sector address  
  secoff=WriteAddr% 4096 ; // The offset in the sector
  secremain= 4096 -secoff; // The remaining space of the sector   
  // //printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//for testing
  if (NumByteToWrite<=secremain) secremain=NumByteToWrite; // No more than 4096 bytes
  while(1) 
  {	
    W25QXX_Read (W25QXX_BUF,secpos* 4096 , 4096 ); // Read the content of the entire sector
    for (i= 0 ;i<secremain;i++) // check data
    {
      if (W25QXX_BUF[secoff+i]!= 0XFF ) break ; // need to erase  	  
    }
    if (i<secremain) // need to erase
    {
      W25QXX_Erase_Sector (secpos); // Erase this sector
      for(i=0;i<secremain;i++)	   //¸´ÖÆ
      {
        W25QXX_BUF[i+secoff]=pBuffer[i];	  
      }
      W25QXX_Write_NoCheck (W25QXX_BUF, secpos* 4096 , 4096 ); // write the whole sector  
      
    } else  W25QXX_Write_NoCheck (pBuffer,WriteAddr,secremain); // Write what has been erased, directly write to the remaining section of the sector. 				   
    if (NumByteToWrite==secremain) break ; // end of writing
    else // writing is not finished
    {
      secpos++; // Increase sector address by 1
      secoff= 0 ; // offset position is 0 	 
      
      pBuffer+=secremain;   // pointer offset
      WriteAddr+=secremain; // write address offset	   
      NumByteToWrite-=secremain;				 // The number of bytes decreases
      if (NumByteToWrite> 4096 )secremain= 4096 ;	 // The next sector still cannot be finished
      else secremain=NumByteToWrite;			 // The next sector can be written
    }	 
  } 
}
// erase the whole chip		  
// Waiting too long...
void W25QXX_Erase_Chip(void)   
{                                   
  W25QXX_Write_Enable();                     //SET WEL 
  W25QXX_Wait_Busy();   
  W25QXX_CS_LOW;                               // Enable the device   
  SPI2_ReadWriteByte (W25X_ChipErase);         // Send chip erase command  
  W25QXX_CS_HIGH;                              // Cancel chip selection     	      
  W25QXX_Wait_Busy ();   				              // Waiting for chip erase to end
}   
// erase a sector
// Dst_Addr: The sector address is set according to the actual capacity
// The minimum time to erase a sector: 150ms
void W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
  // Monitor falsh erasure, for testing   
  ////printf("fe:%x\r\n",Dst_Addr);	  
  Dst_Addr*=4096;
  W25QXX_Write_Enable();                  //SET WEL 	 
  W25QXX_Wait_Busy();   
  W25QXX_CS_LOW;                            // Enable the device   
  SPI2_ReadWriteByte (W25X_SectorErase);    // Send sector erase command
  if (W25QXX_TYPE==W25Q256)                 // If it is W25Q256, the address is 4 bytes, and the highest 8 bits should be sent
  {
    SPI2_ReadWriteByte((uint8_t)((Dst_Addr)>>24)); 
  }
  SPI2_ReadWriteByte (( uint8_t )((Dst_Addr)>> 16 ));   // Send 24bit address    
  SPI2_ReadWriteByte((uint8_t)((Dst_Addr)>>8));   
  SPI2_ReadWriteByte((uint8_t)Dst_Addr);  
  W25QXX_CS_HIGH;                            // Cancel chip selection     	      
  W25QXX_Wait_Busy ();   				            // Wait for the erase to complete
}  
// wait for idle
void W25QXX_Wait_Busy(void)   
{   
  while (( W25QXX_Read_SR ( 1 )& 0x01 )== 0x01 );    // wait for the BUSY bit to clear
}  

// Get status (1, busy; 0, free)
uint8_t W25QXX_Get_State(void)
{
  uint8_t temp;
  
  temp = W25QXX_Read_SR(1);
  temp = temp&0x01;
  return temp;
}
// Enter power-down mode
void W25QXX_PowerDown(void)   
{ 
  W25QXX_CS_LOW;                             // Enable the device   
  SPI2_ReadWriteByte (W25X_PowerDown);       // Send power down command  
  W25QXX_CS_HIGH;                            // Cancel chip selection     	      
  HAL_Delay ( 3 );                             // wait for TPD  
}   
// wake up
void W25QXX_WAKEUP(void)   
{  
  W25QXX_CS_LOW;                                 // Enable the device   
  SPI2_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
  W25QXX_CS_HIGH;                                // Cancel chip selection     	      
  HAL_Delay ( 3 );                                 // wait for TRES1
}   
