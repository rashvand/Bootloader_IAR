




FILINFO Finfo;
static FRESULT scan_files (
                           char* path,		/* Pointer to the path name working buffer */
                           UINT* n_dir,
                           UINT* n_file,
                           QWORD* sz_file
                             )
{
  DIR dirs;
  FRESULT res;
  BYTE i;
  
  
  if ((res = f_opendir(&dirs, path)) == FR_OK) {
    i = strlen(path);
    while (((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
      if (Finfo.fattrib & AM_DIR) {
        (*n_dir)++;
        *(path+i) = '/'; strcpy(path+i+1, Finfo.fname);
        res = scan_files(path, n_dir, n_file, sz_file);
        *(path+i) = '\0';
        if (res != FR_OK) break;
      } else {
        //printf("%s/%s\n", path, fn);
        (*n_file)++;
        *sz_file += Finfo.fsize;
      }
    }
  }
  
  return res;
}



///////////////////////////////////////////



FATFS FlashFatFs, PSRamFatFs;   /* File system object for each logical drive */
FIL   FlashFile, PSRamFile;     /* File objects */
DIR   FlashDir, PSRamDir;       /* Directory object */










FRESULT fr; 
  
  fr = f_mount(&FlashFatFs, "/flash", 0);
  if(fr == FR_OK){
    printf("Flash mount ok!\r\n");
  }
  else{
    printf("Flash mount error, error code:%d.\r\n",fr);
  }
  
  fr = f_mount(&PSRamFatFs, "/psram", 0);
  if(fr == FR_OK){
    printf("PSRAM mount ok!\r\n");
  }
  else{
    printf("PSRAM mount error, error code:%d.\r\n",fr);
  }
  
#if(0)  // Flash
  uint16_t write_num = 0;
  uint8_t write_dat[] = "This is STM32 working with FatFs using W25QXX as storage medium.\n";
  
  //---------S: First time ---------
  //W25QXX_Erase_Chip();
  
  MKFS_PARM mkfs_parm = { .fmt = FM_ANY, .n_fat = 2, .align = 1 };
  fr = f_mkfs("/flash", &mkfs_parm, NULL, FF_MAX_SS);
  if (fr == FR_OK){
    printf("f_mkfs Ok!\r\n");
  }
  else{
    printf("f_mkfs Error, error code:%d.\r\n",fr);
  }
    
  fr = f_open(&FlashFile, "/flash/test.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if(fr == FR_OK){
    printf("open file \"%s\" ok! \r\n", "test.txt");
  }
  else{
    printf("open file \"%s\" error : %d\r\n", "test.txt", fr);
  }
  
  fr = f_write(&FlashFile, write_dat, sizeof(write_dat), (void *)&write_num);
  if(fr == FR_OK){
    printf("write %d dat to file \"%s\" ok,dat is \"%s\".\r\n", write_num, "test.txt", write_dat);
  }
  else{
    printf("write dat to file \"%s\" error,error code is:%d\r\n", "test.txt", fr);
  }
  
  fr = f_close(&FlashFile);
  if(fr == FR_OK){
    printf("close file \"%s\" ok!\r\n", "test.txt");
  }
  else{
    printf("close file \"%s\" error, error code is:%d.\r\n", "test.txt", fr);
  }
  //---------E: First time ---------
#endif  
  
#if(1)  // PSRAM
  uint16_t write_num = 0;
  uint8_t write_dat[] = "This is STM32 working with FatFs using PSRAM as storage medium.\n";
  
  //---------S: First time ---------  
  MKFS_PARM mkfs_parm = { .fmt = FM_FAT, .n_fat = 2, .align = 1 };
  fr = f_mkfs("/psram", &mkfs_parm, NULL, FF_MIN_SS);
  if (fr == FR_OK){
    printf("f_mkfs Ok!\r\n");
  }
  else{
    printf("f_mkfs Error, error code:%d.\r\n",fr);
  }
    
  fr = f_open(&PSRamFile, "/psram/test.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if(fr == FR_OK){
    printf("open file \"%s\" ok! \r\n", "test.txt");
  }
  else{
    printf("open file \"%s\" error : %d\r\n", "test.txt", fr);
  }
  
  fr = f_write(&PSRamFile, write_dat, sizeof(write_dat), (void *)&write_num);
  if(fr == FR_OK){
    printf("write %d dat to file \"%s\" ok,dat is \"%s\".\r\n", write_num, "test.txt", write_dat);
  }
  else{
    printf("write dat to file \"%s\" error,error code is:%d\r\n", "test.txt", fr);
  }
  
  fr = f_close(&PSRamFile);
  if(fr == FR_OK){
    printf("close file \"%s\" ok!\r\n", "test.txt");
  }
  else{
    printf("close file \"%s\" error, error code is:%d.\r\n", "test.txt", fr);
  }
  //---------E: First time ---------
#endif    
  
#if(0)  // FLASH 
  uint16_t read_num = 0;
  uint8_t read_dat[100] = "";
  
  //---------S: General Use ---------
  fr = f_open(&FlashFile, "/flash/test.txt", FA_READ);
  if(fr == FR_OK){
    printf("open file \"%s\" ok! \r\n", "test.txt");
  }
  else{
    printf("open file \"%s\" error : %d\r\n", "test.txt", fr);
  }
  
  fr = f_read(&FlashFile, read_dat, sizeof(read_dat), (void *)&read_num);
  if(fr == FR_OK){
    printf("write %d dat to file \"%s\" ok,dat is \"%s\".\r\n", read_num, "test.txt", read_dat);
  }
  else{
    printf("write dat to file \"%s\" error,error code is:%d\r\n", "test.txt", fr);
  }
  
  fr = f_close(&FlashFile);
  if(fr == FR_OK){
    printf("close file \"%s\" ok!\r\n", "test.txt");
  }
  else{
    printf("close file \"%s\" error, error code is:%d.\r\n", "test.txt", fr);
  }
  //---------E: General Use ---------
#endif  
  
  
#if(1)  // PSRAM 
  uint16_t read_num = 0;
  uint8_t read_dat[100] = "";
  
  //---------S: General Use ---------
  fr = f_open(&PSRamFile, "/psram/test.txt", FA_READ);
  if(fr == FR_OK){
    printf("open file \"%s\" ok!, Size: %d byte\r\n", "test.txt", f_size(&PSRamFile));
  }
  else{
    printf("open file \"%s\" error : %d\r\n", "test.txt", fr);
  }
  
  fr = f_read(&PSRamFile, read_dat, sizeof(read_dat), (void *)&read_num);
  if(fr == FR_OK){
    printf("write %d dat to file \"%s\" ok,dat is \"%s\".\r\n", read_num, "test.txt", read_dat);
  }
  else{
    printf("write dat to file \"%s\" error,error code is:%d\r\n", "test.txt", fr);
  }
  
  fr = f_close(&PSRamFile);
  if(fr == FR_OK){
    printf("close file \"%s\" ok!\r\n", "test.txt");
  }
  else{
    printf("close file \"%s\" error, error code is:%d.\r\n", "test.txt", fr);
  }
  //---------E: General Use ---------
#endif
  
  
///////////////////////////////////////////////////



  
  
  
  
  
  