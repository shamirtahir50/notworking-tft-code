#include <dummy_rp2040.h>

#include <TFT_eSPI.h>       

// FONTS
#include "Fonts.h"

// IMAGES
#include "Screen1logotitle.h"
#include "logo.h"
#include "battary.h"
#include "signal.h"
#include "background.h"
#include "bottom.h" 
#include "dateback.h"
#include "timeback.h"
#include "triback.h" 
#include "pointimg.h" 
#include "s3_yesnoback.h" 
#include "flowvalueback.h" 
#include "flowrateback.h" 
#include "timehourback.h"
#include "volumeback.h"
#include "s5_setflowback.h" 
#include "setflowbackcc.h" 
#include "midscreenback.h" 
#include "minback.h" 
#include "hrback.h"
#include "secback.h"
#include "dayback.h" 
#include "monback.h"
#include "yearback.h"
#include "celback.h"
#include "fahback.h"
#include "battaryback.h"
#include "battaryimages.h"
#include "motionback.h"
#include "tempback.h"
#include "tempunitback.h"
#include "humback.h"
#include "motionback2.h"
#include "tempback2.h"
#include "tempunitback2.h"
#include "humback2.h"
#include "bpback.h"
#include "bpunit.h"
#include "screen14flowunit.h"
#include "delaytimeback.h"
#include "runtimeback.h"

// Colors
#define TFT_CYAN3 0x028B
#define TFT_GREY3 0x5269
#define TFT_CYAN1 0x871A


// Defination
TFT_eSPI tft = TFT_eSPI();  

// BUTTONS
const byte button1_pin = 26;
const byte button2_pin = 27;
const byte button3_pin = 28;

// BOOLEAN
volatile boolean Screen1 = true;
volatile boolean Screen2 = false;
volatile boolean Screen2_up = false;
volatile boolean Screen2_down = false;
volatile boolean Screenup = false;
volatile boolean Screendown = false;
volatile boolean Screenenter = false;
volatile boolean Screen2_enter = false;
volatile boolean Screen8_enter = false;
volatile boolean Screen3_yes = false;
volatile boolean Screen3_no = false;
volatile boolean Screen3 = false;
volatile boolean Screen3_1 = false;
volatile boolean Screen3_1_1 = false;
volatile boolean Screen3_2 = false;
volatile boolean Screen3_2_1 = false;
volatile boolean Screen3_T0  = false;
volatile boolean Screen3_T0_1 = false;
volatile boolean Screen3_T1 = false;
volatile boolean Screen3_T2 = false;
volatile boolean Screen4 = false;
volatile boolean Screen5 = false;
volatile boolean Screen5_T0 = false;
volatile boolean Screen6 = false;
volatile boolean Screen6_T0 = false;
volatile boolean Screen7 = false;
volatile boolean Screen7from3_2_1 = false;
volatile boolean Screen8 = false;
volatile boolean Screen9 = false;
volatile boolean Screen10 = false;
volatile boolean Screen10_h = false;
volatile boolean Screen10_m = false;
volatile boolean Screen10_s = false;
volatile boolean Screen10_d = false;
volatile boolean Screen10_mon = false;
volatile boolean Screen10_y = false;
volatile boolean Screen11 = false;
volatile boolean Screen11_1 = false;
volatile boolean Screen11_2 = false;
volatile boolean Screen11_3 = false;
volatile boolean Screen12 = false;
volatile boolean Screen13 = false;
volatile boolean Screen14 = false;
volatile boolean PERMISSIONCALIBRATE_yes = true;
volatile boolean PERMISSIONCALIBRATE_no = true;
volatile boolean PERMISSIONRUNTIME_yes = true;
volatile boolean PERMISSIONRUNTIME_no = true;
double flow;
volatile float flowrate = 00.00;
volatile int pressed = 0;

float number = 00.00;

int time_hours = 26 , volume = 70;
volatile int transition = -1;
volatile int settings_transition = -1;
volatile int td_transition = 1;
volatile int screen11_transition = -1;

void my()
{
    if(Screen2 == true){
        transition++;
       if(transition >= 5){
       transition = 0;
      }
      
      Serial.println(transition);
    }
    Screendown = true;
      Screenup = false;
      Screenenter = false;
    if(Screen8 == true){
      settings_transition++;
    if(settings_transition >= 6){
     settings_transition = 0;
    }
     Serial.println(settings_transition);
    }
  
     if(Screen11 == true){
      screen11_transition++;
    if(screen11_transition >= 4){
     screen11_transition = 0;
    }
     Serial.println(screen11_transition);
    }
}

// ISR
void ISR_1(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // if (interrupt_time - last_interrupt_time > 50 && digitalRead(button1_pin) == LOW) {
  //   while(1){
  //     my();
  //     Serial.println("PRESSED LONG");
  //     // if(digitalRead(button1_pin) == LOW){
  //     //   break;
  //     // }
  //   }
  // last_interrupt_time = interrupt_time;
  // }
  if (interrupt_time - last_interrupt_time > 200 && digitalRead(button1_pin) == LOW) {
    my();
    Serial.println("PRESSED");

    last_interrupt_time = interrupt_time;
    
  }
  last_interrupt_time = interrupt_time;

}
void ISR_2(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200 && digitalRead(button2_pin) == LOW) 
  {
    if (Screen2 == true){
      transition--;
    if(transition <= -1){
     transition = 4;
    }
    
    Serial.println(transition);
    }
    Screenup = true;
    Screendown = false;
    Screenenter = false;

    if (Screen8 == true){
      settings_transition--;
    if(settings_transition <= -1){
     settings_transition = 6;
    }
    Serial.println(settings_transition);
    }
     if (Screen11 == true){
      screen11_transition--;
    if(screen11_transition <= -1){
     screen11_transition = 3;
    }
    Serial.println(screen11_transition);
    }
  }
  last_interrupt_time = interrupt_time;
}
void ISR_3(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200 && digitalRead(button3_pin) == LOW) 
  {
    Screenenter = true;
    Screenup = false;
    Screendown = false;
    Serial.println("Enter");
    if(Screen10 == true){
      td_transition++;
     if(td_transition > 6){
        td_transition = 1;
     }
     Serial.println(td_transition);
     Serial.println(Screen10);
    }
  }
  last_interrupt_time = interrupt_time;
}

// Initialize screen
void Startup(){
  Serial.begin(115200);
  // Serial1.begin(115200, SERIAL_8N1, 32, 33);
  tft.begin();
  tft.setSwapBytes(true);
  tft.setRotation(1);
}
//Background
void background_display(){
  tft.pushImage(0, 0, 320, 240, background);
}
// Screen Header
void battary(int bat){
  
  if(bat == 100){
    tft.pushImage(255, 25, 45, 20, battaryback);
    tft.pushImage(255, 25, 45, 21, battaryfullcharging);
  }
   if(bat == 80){
    tft.pushImage(255, 25, 45, 20, battaryback);
    tft.pushImage(255, 25, 45, 21, battary80charging);
  }
   if(bat == 60){
    tft.pushImage(255, 25, 45, 20, battaryback);
    tft.pushImage(255, 25, 45, 21, battary60charging);
  } if(bat == 40){
    tft.pushImage(255, 25, 45, 20, battaryback);
    tft.pushImage(255, 25, 45, 21, battary40charging);
  } if(bat == 20){
    tft.pushImage(255, 25, 45, 20, battaryback);
    tft.pushImage(255, 25, 45, 21, battarylowcharging);
  }

}
//void signalstrength(int sig){
//    if(sig == 100){
//   tft.pushImage(222, 27, 25, 20, sigback);
//   tft.pushImage(222, 27, 25, 20, signalfull);
//  }
//   if(sig == 75){
//   tft.pushImage(222, 27, 25, 20, sigback);
//    tft.pushImage(222, 27, 25, 20, signal75);
//  }
//   if(sig == 50){
//    tft.pushImage(222, 27, 25, 20, sigback);
//    tft.pushImage(222, 27, 25, 20, signal50);
//  } if(sig == 25){
//   tft.pushImage(222, 27, 25, 20, sigback);
//    tft.pushImage(222, 27, 25, 20, signal25);
//  } if(sig == 0){
//   tft.pushImage(222, 27, 25, 20, sigback);
//   tft.pushImage(222, 27, 25, 20, signalno);
//  }
//}
void displaytime(){
  tft.setFreeFont(FSS9);  
  tft.setCursor(20,30);
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREY3);
  tft.print("06:45 PM");
}
void displaydate(){
  
  // tft.setFreeFont(FSS09);
  tft.setCursor(20,45);
  tft.setTextColor(TFT_GREY3);
  tft.setTextSize(1);
  tft.print("22 Feb, 2022");
}
void headerunderline(){
    tft.drawWideLine(20,55,300,55,2,TFT_CYAN3 , TFT_CYAN3);
}
void DisplayHeader(){
  tft.fillScreen(TFT_WHITE);
  // Battery
//  battary(battarystrength);
  // Signals
//  signalstrength();
  // Time
  displaytime();
  // Date
  displaydate();
  // Underline
  headerunderline();
}
void footer(){
  tft.drawWideLine(20,215,300,215,2,TFT_CYAN3 , TFT_CYAN3);
  // tft.setFreeFont(&FreeSans7pt7b);
  tft.setCursor(105,230);
  tft.setTextColor(TFT_CYAN3);
  tft.setTextSize(1);
  tft.print("Kaon Kinetic K 51");
}


#include "Screen1.h"
#include "Screen2.h"
#include "Screen3.h"
#include "Screen4.h"
#include "Screen5.h"
#include "Screen6.h"
#include "Screen7.h"
#include "Screen8.h"
#include "Screen9.h"
#include "Screen10.h"
#include "Screen11.h"
#include "Screen12.h"
#include "Screen13.h"
#include "Screen14.h"
#include "Screen15.h"
