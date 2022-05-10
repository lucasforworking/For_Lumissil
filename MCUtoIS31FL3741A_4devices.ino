#include<Wire.h> 
#include<avr/pgmspace.h> 
 
#define Addr_GND 0x60 
#define Addr_SCL 0x62
#define Addr_SDA 0x64 
#define Addr_VCC 0x66 


#define R 0xFF 
#define G 0x00 
#define B 0xFF 
uint8_t i,j,k; 

boolean state = false;
boolean buttonUp = true;
 
const PROGMEM byte PWM_Gama64[64]= 
{ 
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07, 
    0x08,0x09,0x0b,0x0d,0x0f,0x11,0x13,0x16, 
    0x1a,0x1c,0x1d,0x1f,0x22,0x25,0x28,0x2e, 
    0x34,0x38,0x3c,0x40,0x44,0x48,0x4b,0x4f, 
    0x55,0x5a,0x5f,0x64,0x69,0x6d,0x72,0x77, 
    0x7d,0x80,0x88,0x8d,0x94,0x9a,0xa0,0xa7, 
    0xac,0xb0,0xb9,0xbf,0xc6,0xcb,0xcf,0xd6, 
    0xe1,0xe9,0xed,0xf1,0xf6,0xfa,0xfe,0xff 
}; 
 
void setup() { 

  // init btn 7
  pinMode(7, HIGH);
  digitalWrite(7, HIGH);


    Wire.begin(); 
    Wire.setClock(1000000);//I2C 1MHz
// Wire.setClock(400000);//I2C 400kHz 
// pinMode(4,OUTPUT);//SDB
// digitalWrite(4,HIGH);//SDB_HIGH  
 for(k=0x60;k<=0x66;k+=2) 
  Init_3741A(k, R, G, B);  
    //Init_3741A(Addr_GND, R, G, B); 
  //Init_3741A(Addr_SCL, R, G, B); 
  //Init_3741A(Addr_SDA, R, G, B); 
  //Init_3741A(Addr_VCC, R, G, B);  
  
} 
 
void loop() { 


  for(k=0x60;k<=0x66;k+=2) 
  {
  IS31FL3741A_Test_mode_addr(k);
  }
  
  
  
  delay(1000); //delay 1s
    
    
} 
 
void IS_IIC_WriteByte(uint8_t Dev_Add,uint8_t Reg_Add,uint8_t Reg_Dat) 
{ 
    Wire.beginTransmission(Dev_Add/2); // transmit to device IS31FL373x 
    Wire.write(Reg_Add); // sends regaddress 
    Wire.write(Reg_Dat); // sends regaddress 
    Wire.endTransmission(); // stop transmitting 
} 
 
void Init_3741A(uint8_t Addr, uint8_t Rdata, uint8_t Gdata, uint8_t Bdata) 
{ 
    IS_IIC_WriteByte(Addr,0xfe,0xc5);//unlock 
    IS_IIC_WriteByte(Addr,0xfD,0x02);//write page 2 
    for(i=2;i<0xB4;i+=3) 
    { 
        IS_IIC_WriteByte(Addr,i,Rdata);//R LED Scaling 
    } 
    for(i=1;i<0xB4;i+=3) 
    { 
        IS_IIC_WriteByte(Addr,i,Gdata);//G LED Scaling 
    } 
    for(i=0;i<0xB4;i+=3) 
    { 
        IS_IIC_WriteByte(Addr,i,Bdata);//B LED Scaling 
    } 
    IS_IIC_WriteByte(Addr,0xfe,0xc5);//unlock 
    IS_IIC_WriteByte(Addr,0xfD,0x03);//write page 3 
    for(i=2;i<0xAB;i+=3) 
    { 
        IS_IIC_WriteByte(Addr,i,Rdata);//R LED Scaling 
    } 
    for(i=1;i<0xAB;i+=3) 
    { 
        IS_IIC_WriteByte(Addr,i,Gdata);//G LED Scaling 
    } 
    for(i=0;i<0xAB;i+=3) 
    { 
        IS_IIC_WriteByte(Addr,i,Bdata);//B LED Scaling 
    } 
 
    IS_IIC_WriteByte(Addr,0xfe,0xc5);//unlock 
    IS_IIC_WriteByte(Addr,0xfD,0x00);//write page 0 
    for(i=0;i<0xB4;i++) 
    { 
        IS_IIC_WriteByte(Addr,i,0x00);//write all PWM set 0x00 
    } 
    IS_IIC_WriteByte(Addr,0xfe,0xc5);//unlock 
    IS_IIC_WriteByte(Addr,0xfD,0x01);//write page 1 
    for(i=0;i<0xAB;i++) 
    { 
        IS_IIC_WriteByte(Addr,i,0x00);//write all PWM set 0x00 
    } //init all the PWM data to 0 
   
    IS_IIC_WriteByte(Addr,0xfe,0xc5);//unlock 
    IS_IIC_WriteByte(Addr,0xfD,0x04);//write page 4 
  
    IS_IIC_WriteByte(Addr,0x01,0x7F);//global current 
    IS_IIC_WriteByte(Addr,0x36,0x0B);//PWM frequency setting(900Hz)
    IS_IIC_WriteByte(Addr,0x00,0x01);//normal operation   
  //normal operation regiter
  //bit0 SSD  Software Shutdown Control (1  Normal operation)
  //bit2:1 SSD  OSDE  Open Short Detection Enable  (00 Disable open/short detection)
  //bit3 H/L Logic (0 1.4V/0.4V )
  //bit7:4 SWS  SWx Setting (0000  n=9, SW1~SW9, 1/9)

} 


void IS31FL3741A_Test_mode1(void)// 
{ 
    IS_IIC_WriteByte(Addr_GND,0xFD,0x00);//write frist frame to page0
    for (j=0;j<64;j++)//all LED ramping up 
    {   
        IS_IIC_WriteByte(Addr_GND,0xfe,0xc5);//unlock 
        IS_IIC_WriteByte(Addr_GND,0xfD,0x00);//write page 0 
        for(i=0;i<0xB4;i++) 
        { 
            IS_IIC_WriteByte(Addr_GND,i,pgm_read_byte_near(&PWM_Gama64[j]));//set all PWM 
        } 
        IS_IIC_WriteByte(Addr_GND,0xfe,0xc5);//unlock 
        IS_IIC_WriteByte(Addr_GND,0xfD,0x01);//write page 1 
        for(i=0;i<0xAB;i++) 
        { 
            IS_IIC_WriteByte(Addr_GND,i,pgm_read_byte_near(&PWM_Gama64[j]));//set all PWM 
        } 
        delay(10);//10ms 
    } 
    delay(1000); //keep on 1s 
     
    for (j=63;j>0;j--)//all LED ramping down 
    { 
        IS_IIC_WriteByte(Addr_GND,0xfe,0xc5);//unlock 
        IS_IIC_WriteByte(Addr_GND,0xfD,0x00);//write page 0 
        for(i=0;i<0xB4;i++) 
        { 
            IS_IIC_WriteByte(Addr_GND,i,pgm_read_byte_near(&PWM_Gama64[j-1]));//set all PWM 
        } 
        IS_IIC_WriteByte(Addr_GND,0xfe,0xc5);//unlock 
        IS_IIC_WriteByte(Addr_GND,0xfD,0x01);//write page 1 
        for(i=0;i<0xAB;i++) 
        { 
            IS_IIC_WriteByte(Addr_GND,i,pgm_read_byte_near(&PWM_Gama64[j-1]));//set all PWM 
        } 
        delay(10);//10ms 
    }   
    delay(500); //keep off 0.5s 
} 
void IS31FL3741A_Test_mode_addr(uint8_t Addr)// 
{ 
    IS_IIC_WriteByte(Addr,0xFD,0x00);//write frist frame to page0
    for (j=0;j<64;j++)//all LED ramping up 
    {   
        IS_IIC_WriteByte(Addr,0xfe,0xc5);//unlock 
        IS_IIC_WriteByte(Addr,0xfD,0x00);//write page 0 
        for(i=0;i<0xB4;i++) 
        { 
            IS_IIC_WriteByte(Addr,i,pgm_read_byte_near(&PWM_Gama64[j]));//set all PWM 
        } 
        IS_IIC_WriteByte(Addr,0xfe,0xc5);//unlock 
        IS_IIC_WriteByte(Addr,0xfD,0x01);//write page 1 
        for(i=0;i<0xAB;i++) 
        { 
            IS_IIC_WriteByte(Addr,i,pgm_read_byte_near(&PWM_Gama64[j]));//set all PWM 
        } 
        delay(10);//10ms 
    } 
    delay(1000); //keep on 1s 
     
    for (j=63;j>0;j--)//all LED ramping down 
    { 
        IS_IIC_WriteByte(Addr,0xfe,0xc5);//unlock 
        IS_IIC_WriteByte(Addr,0xfD,0x00);//write page 0 
        for(i=0;i<0xB4;i++) 
        { 
            IS_IIC_WriteByte(Addr,i,pgm_read_byte_near(&PWM_Gama64[j-1]));//set all PWM 
        } 
        IS_IIC_WriteByte(Addr,0xfe,0xc5);//unlock 
        IS_IIC_WriteByte(Addr,0xfD,0x01);//write page 1 
        for(i=0;i<0xAB;i++) 
        { 
            IS_IIC_WriteByte(Addr,i,pgm_read_byte_near(&PWM_Gama64[j-1]));//set all PWM 
        } 
        delay(10);//10ms 
    }   
    delay(500); //keep off 0.5s 
} 
