// IMPORTANT: LCDWIKI_KBV LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.

//This program is a demo of how to show a bmp picture from SD card.

//Set the pins to the correct ones for your development shield or breakout board.
//when using the BREAKOUT BOARD only and using these 8 data lines to the LCD,
//pin usage as follow:
//             CS  CD  WR  RD  RST  D0  D1  D2  D3  D4  D5  D6  D7
//Arduino Uno  A3  A2  A1  A0  A4   8   9   2   3   4   5   6   7

//Remember to set the pins to suit your display module!

#include <SD.h>
#include <SPI.h>
#include <Encoder.h>     //encoder library
#include <Vector.h>
#include <TouchScreen.h> //touch library
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

#define YP A3  // must be an analog pin, use "An" notation!--------------------------------A3 to A6
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//param calibration from kbv
#define TS_MINX 906
#define TS_MAXX 116

#define TS_MINY 92 
#define TS_MAXY 952

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Encoder encod(20, 19);

//the definiens of 8bit mode as follow:
LCDWIKI_KBV my_lcd(320,480,A3,A2,A1,A0,A4);//width,height,cs,cd,wr,rd,reset

#define  BLACK   0x0000
#define  DGREY   0x3186
#define  GREY    0x52AA
#define  LGREY   0x8430
#define  WHITE   0xFFFF

#define PIXEL_NUMBER  100
#define FILE_NUMBER 6
#define FILE_NAME_SIZE_MAX 20

uint32_t bmp_offset = 0;
uint16_t s_width = my_lcd.Get_Display_Width();  
uint16_t s_heigh = my_lcd.Get_Display_Height();
//int16_t PIXEL_NUMBER;

char file_name[FILE_NUMBER][FILE_NAME_SIZE_MAX];

uint16_t read_16(File fp)
{
    uint8_t low;
    uint16_t high;
    low = fp.read();
    high = fp.read();
    return (high<<8)|low;
}

uint32_t read_32(File fp)
{
    uint16_t low;
    uint32_t high;
    low = read_16(fp);
    high = read_16(fp);
    return (high<<8)|low;   
 }
 
bool analysis_bpm_header(File fp)
{
    if(read_16(fp) != 0x4D42)
    {
      return false;  
    }
    //get bpm size
    read_32(fp);
    //get creator information
    read_32(fp);
    //get offset information
    bmp_offset = read_32(fp);
    //get DIB infomation
    read_32(fp);
    //get width and heigh information
    uint32_t bpm_width = read_32(fp);
    uint32_t bpm_heigh = read_32(fp);
    if((bpm_width != s_width) || (bpm_heigh != s_heigh))
    {
      return false; 
    }
    if(read_16(fp) != 1)
    {
        return false;
    }
    read_16(fp);
    if(read_32(fp) != 0)
    {
      return false; 
     }
     return true;
}


//This is for showing the splashscreen
void draw_bmp_picture(File fp, int off_x, int off_y)
{
  uint16_t i,j,k,l,m=0;
  uint8_t bpm_data[PIXEL_NUMBER*3] = {0};
  uint16_t bpm_color;
  fp.seek(bmp_offset);
  for(i = 0;i < 100;i++)
  {
      m = 0;
      fp.read(bpm_data,PIXEL_NUMBER*3);
      for(k = 0;k<PIXEL_NUMBER;k++)
      {
        
        bpm_color= my_lcd.Color_To_565(bpm_data[m+2], bpm_data[m+1], bpm_data[m+0]);
        m +=3;
        
        if(bpm_color != BLACK)
        {
          my_lcd.Set_Draw_color(bpm_color);
          my_lcd.Draw_Pixel((k) + off_y,i + off_x);
        }
      } 
     
   }    
}


//This is for the smaller images on the screen
void draw_bmp_picture_s(File fp)
{
  int wide = (my_lcd.Get_Display_Width()/4);
  uint16_t i,j,k,l,m=0;
  uint8_t bpm_data[wide*3] = {0};
  uint16_t bpm_color;
  
  fp.seek(bmp_offset);
  for(i = 0;i < s_heigh;i++)
  {
    for(j = 0;j<s_width/wide;j++)
    {
      m = 0;
      fp.read(bpm_data,wide*3);
      for(k = 0;k<wide;k++)
      {
        bpm_color= my_lcd.Color_To_565(bpm_data[m+2], bpm_data[m+1], bpm_data[m+0]); //change to 565
        m +=3;
          
        if(bpm_color != BLACK)
        {
          my_lcd.Set_Draw_color(bpm_color);
          my_lcd.Draw_Pixel(j*wide+k,i);
        }
      }    
     }
   }    
}

int circle(int slot, bool col)
{
   Serial.println(slot);

   if(col)
   {
      my_lcd.Set_Draw_color(LGREY);
   }
   else
   {
      my_lcd.Set_Draw_color(BLACK);
   }

   //void Draw_Round_Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t radius);

   if(slot == 1)
   {
      //my_lcd.Fill_Circle(300,135,8); //1
      my_lcd.Draw_Round_Rectangle( 180, 20, 300, 140, 15);
      my_lcd.Draw_Round_Rectangle( 178, 18, 302, 142, 15);
      my_lcd.Draw_Round_Rectangle( 176, 16, 304, 144, 15);
   }
   else if(slot == 2)
   {
      //my_lcd.Fill_Circle(300,303,8); //2
      my_lcd.Draw_Round_Rectangle(180, 180, 300, 300, 15);
      my_lcd.Draw_Round_Rectangle(178, 178, 302, 302, 15);
      my_lcd.Draw_Round_Rectangle(176, 176, 304, 304, 15);
   }
   else if(slot == 3)
   {
      //my_lcd.Fill_Circle(300,460,8); //3
      my_lcd.Draw_Round_Rectangle(180, 340, 300, 460, 15);
      my_lcd.Draw_Round_Rectangle(178, 338, 302, 462, 15);
      my_lcd.Draw_Round_Rectangle(176, 336, 304, 464, 15);
   }
   else if(slot == 4)
   {
      //my_lcd.Fill_Circle(140,135,8); //4
      my_lcd.Draw_Round_Rectangle(20, 20, 140, 140, 15);
      my_lcd.Draw_Round_Rectangle(18, 18, 142, 142, 15);
      my_lcd.Draw_Round_Rectangle(16, 16, 144, 144, 15);
   }
   else if(slot == 5)
   {
      //my_lcd.Fill_Circle(140,303,8); //5
      my_lcd.Draw_Round_Rectangle(20, 180, 140, 300, 15);
      my_lcd.Draw_Round_Rectangle(18, 178, 142, 302, 15);
      my_lcd.Draw_Round_Rectangle(16, 176, 144, 304, 15);
   }
   else
   {
      //my_lcd.Fill_Circle(140,460,8); //6
      my_lcd.Draw_Round_Rectangle(20, 340, 140, 460, 15);
      my_lcd.Draw_Round_Rectangle(18, 338, 142, 462, 15);
      my_lcd.Draw_Round_Rectangle(16, 336, 144, 464, 15);
   }
 
   return(slot);
}

int touch(TSPoint p, int sel)
{
      p.x = map(p.x, TS_MINX, TS_MAXX, my_lcd.Get_Display_Width(), 0);
      p.y = map(p.y, TS_MINY, TS_MAXY, my_lcd.Get_Display_Height(),0);
      
      if(p.x > 180 )
      {
        if(p.y < 160)
        {
          sel = 1;
        }
        else if(p.y > 160 && p.y < 320)
        {
          sel = 2;
        }
        else
        { 
          sel = 3;
        }
      }
      else
      {
        if(p.y < 160)
        {
          sel = 4;
        }
        else if(p.y > 160 && p.y < 320)
        {
          sel = 5;
        }
        else
        {
          sel = 6;
        }
      }

      return sel;
}

//we are working on this
//-----------------------------------------------------------------------------------
void Img_recieve(int off_x, int off_y)
{ 
  uint8_t pad;
  uint16_t bpm_color;
  uint8_t colorR [2];
  
  Serial.println('s'); //this is a "start" indicator

  //fp.seek(bmp_offset);
  for(int i = 0; i < 100; i++)
  {
    
    for(int j = 0; j < 100; j++)
    {
      
      while(Serial.available() < 2)
      {
      }

      colorR[0] = Serial.read();
      colorR[1] = Serial.read();

      bpm_color = (( colorR[1] << 8) | colorR[0]);

      if(bpm_color != BLACK)
      {
        my_lcd.Set_Draw_color(bpm_color);
        my_lcd.Draw_Pixel(j + off_y, i + off_x);
      }
    }
  }
}
//-----------------------------------------------------------------------------------

void Back_recieve()
{ 
  uint8_t pad;
  uint16_t bpm_color;
  uint8_t colorR [2];
  
  Serial.println('s'); //this is a "start" indicator

  //fp.seek(bmp_offset);
  for(int i = 0; i < 480; i++)
  {
    
    for(int j = 0; j < 320; j++)
    {
      
      while(Serial.available() < 2)
      {
      }

      colorR[0] = Serial.read();
      colorR[1] = Serial.read();

      bpm_color = (( colorR[1] << 8) | colorR[0]);

      if(bpm_color != BLACK)
      {
        my_lcd.Set_Draw_color(bpm_color);
        my_lcd.Draw_Pixel(j, i);
      }
    }
  }
}

//setup
//***********************************************************************************
void setup() 
{
  String test;
  
   //start screen and serial communication
   Serial.begin(115200);
   Serial.println('C');
   my_lcd.Init_LCD();
   my_lcd.Fill_Screen(BLACK);


    //Init SD_Card
    pinMode(53, OUTPUT);
    SD.begin(53);


    //splash screen
    File bmp_file;
    
    bmp_file = SD.open("vol.bmp");
    analysis_bpm_header(bmp_file);
    draw_bmp_picture_s(bmp_file);
    bmp_file.close(); 

    my_lcd.Fill_Screen(BLACK);

    bmp_file = SD.open("B2.bmp");
    analysis_bpm_header(bmp_file);
    //draw_bmp_picture_s(bmp_file);
    bmp_file.close(); 

    Back_recieve();

    //sets draw color and draws grid
    
    my_lcd.Set_Draw_color(WHITE);

    //my_lcd.Draw_Fast_HLine(0, 155, my_lcd.Get_Display_Width());
    //my_lcd.Draw_Fast_HLine(0, 325, my_lcd.Get_Display_Width());
    //my_lcd.Draw_Fast_VLine(160, 0, 480);

    Img_recieve(30,190);
    Img_recieve(190,190);
    Img_recieve(350,190);
    Img_recieve(30,30);
    Img_recieve(190,30);
    Img_recieve(350,30);

    circle(1, false);
    circle(2, false);
    circle(3, false);
    circle(4, false);
    circle(5, false);
    circle(6, false);

    //String receivedString = Serial.readStringUntil('\n');
    

}
//*************************************************************************************


int iMain = 5;
bool first = true;

int sel = 1;
int selO = 0;

long pEncod[7]  = {-999,-999,-999,-999,-999,-999, -999};
long newEncod[7] = {130,130,220,220,220,220,220};



//main
//--------------------------------------------------------------------------------------
void loop() 
{
    //sets up the varables and pins
    TSPoint p = ts.getPoint();
    pinMode(YP,OUTPUT);
    pinMode(XM,OUTPUT);

    if(first & Serial.available() > 0)
    {
      int startingVol = Serial.read();

      for(int i = 0; i < 2;i++)
      {
        newEncod[i] += startingVol;
      }
      first = false;
    }

    /*
    File bmp_file;

    //reads the pictures if they are new
    //------------------------------------------------
    if(iMain < 1){
      strcpy(file_name[2],"01.bmp");
      strcpy(file_name[1],"02.bmp");
      strcpy(file_name[0],"03.bmp");
      strcpy(file_name[3],"04.bmp");
      strcpy(file_name[4],"05.bmp");
      strcpy(file_name[5],"06.bmp");

      digitalWrite(13, HIGH);
      digitalWrite(13, LOW);
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
    }
    //------------------------------------------------

    
    //draws the pictures on the screen
    //************************************************
    for(0;iMain<FILE_NUMBER;iMain++){
        bmp_file = SD.open(file_name[iMain]);
        analysis_bpm_header(bmp_file);
        if(iMain == 0){
          draw_bmp_picture(bmp_file, 30,190);
        }
        else if(iMain == 1){
          draw_bmp_picture(bmp_file, 190,190);
        }
        else if(iMain == 2){
          draw_bmp_picture(bmp_file, 350,190);
        }
        else if(iMain == 3){
          draw_bmp_picture(bmp_file, 30,30);
        }
        else if(iMain == 4){
          draw_bmp_picture(bmp_file, 190,30);
        }
        else if(iMain == 5){
          draw_bmp_picture(bmp_file, 350,30);
        }
         bmp_file.close();   
     }
     //************************************************
     */
    
    //touch screen part
    //------------------------------------------------
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE){
      sel = touch(p,sel);
    }
    else if(sel != selO){
      selO = circle(selO, false);
      selO = circle(sel, true);
    }
    //------------------------------------------------

    //encoder stuff
    //******************************************************************************
    //detects turn
    if(newEncod[sel] + encod.read() >= 120 && newEncod[sel] + encod.read() <= 240){
      newEncod[sel] = newEncod[sel] + encod.read();
    }
    else{
      encod.write(0);
    }

    //updates volume 
    if (newEncod[sel] != pEncod[sel]){    
      int out = ((1000 * sel) + newEncod[sel]);
      Serial.println(out);
      pEncod[sel] = newEncod[sel];
      encod.write(0);
    }
    //******************************************************************************

}
//--------------------------------------------------------------------------------------
