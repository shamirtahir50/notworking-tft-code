#include "Screenfunctions.h"
#include <EEPROM.h>

// define the number of bytes you want to access
#define EEPROM_SIZE 50
// 0,1 location are resserved for flow


double calibrate = 0.00;
int runtimehours = 8;
int runtimeminutes = 0;
int timedelayhour=0;
int timedelayminutes=0;
int hour = 1,minute  = 1,sec  = 1,day  = 1,month  = 1, year  = 2000;
int updates;


int battarystrength = 0;
int signalstr = 0; 
boolean StartSampaling_yes = false ;
boolean StartSampaling_no = false;
boolean StopSampaling_yes = false ;
boolean StopSampaling_no = false;
boolean timesend = false;
boolean startsamplesend = false;
boolean stopsamplesend = false;
boolean startcalibrate = false;
boolean stopcalibrate = false;


String Displayflow = "15.00"; 
String DisplayBattary = "15.00"; 
String DisplayTemp  =  "50" ,DisplayHumidity = "32" ,DisplayMotion = "76";
String DisplayDate = "11, Sep 2022",DisplayYear;
String DisplayTime = "21:13";
String DisplayBackPressure = "6.9";
String TemperatureUnits="F",Flowrateunits="L/M",BackPressureUnits = "KPA";
String DisplayVolume="1150",DisplayTimeHours="4:50";

int tempunitflag,flowunitflag,bpunitflag;

String StartSamplingSend;
String StopSamplingSend;
String SetTimeandDate;
String StartCalibrationSend;
String StopCalibrationSend;

const unsigned long DisplayInterval = 1000;
unsigned long DisplaypreviousTime = 0;


int readflow(double f){
  double store1,store2;
  double store;
  store1 = EEPROM.read(0);
  store2 = EEPROM.read(1);
  double deci = (store2)/100;
  f = store1 + deci + 0.01;
  Serial.print("Flow from EEPROM: "); Serial.println(f);
  return f;
}
void writeflow(double flow){
    int intpart = (int)flow;
    double decpart = flow - intpart;
    decpart = decpart * 100;
    EEPROM.write(0,intpart);
    EEPROM.write(1,decpart);
    EEPROM.commit();
    Serial.print("Flow written to EEPROM: "); Serial.println(flow);
}

int readcal(double f){
  double store1,store2;
  double store;
  store1 = EEPROM.read(2);
  store2 = EEPROM.read(3);
  double deci = (store2)/100;
  f = store1 + deci + 0.01;
  Serial.print("Flow from EEPROM: "); Serial.println(f);
  return f;
}

void writecal(double flow){
    int intpart = (int)flow;
    double decpart = flow - intpart;
    decpart = decpart * 100;
    EEPROM.write(2,intpart);
    EEPROM.write(3,decpart);
    EEPROM.commit();
    Serial.print("Flow written to EEPROM: "); Serial.println(flow);
}
int readruntimehour(int h,int mem){
 
  h = EEPROM.read(mem);

  Serial.print("Flow from EEPROM: "); Serial.println(h);
  return h;
}
void writeruntimehour(int h,int mem){
    EEPROM.write(mem,h);
    EEPROM.commit();
    Serial.print("Flow written to EEPROM: "); Serial.println(h);
}

void battarycharging(int battarystrength){
    if(battarystrength == 120){
      battarystrength = 20;
    }
    battary(battarystrength);
}
String displaystring(int h,int m){
      String hours;
      String minutes;
      String fulltime;
      if(h < 10){
          Serial.print("runtimehours: "); Serial.println(h);
          hours = String(0) + String(h); 
          Serial.println(hours);
      }
      if(m < 10){
          minutes = "0" + String(m); 
      }
      if(h >= 10){
          hours = String(h); 
      }
      if(m >= 10){
          minutes = String(m); 
      }
      fulltime = hours + ":" + minutes;
      return fulltime;
}

// d{date},{time},{flow},{temp},{Hum},{Accel} Recieving 
// DisplayScreen_14(String m,String t,String h,String f,String bp,String Tempunit,String bpunit,String flowunit)
void setup()
{
  
  Startup();
   EEPROM.begin(EEPROM_SIZE);
  flow = readflow(flow);
  calibrate = readcal(calibrate);
  runtimehours = readruntimehour(runtimehours,4);
  runtimeminutes = readruntimehour(runtimeminutes,5);
  timedelayhour = readruntimehour(timedelayhour,6);
  timedelayminutes = readruntimehour(timedelayminutes,7);
  tempunitflag = readruntimehour(tempunitflag,8);
  flowunitflag = readruntimehour(flowunitflag,9);
  bpunitflag = readruntimehour(bpunitflag,10);
      if(tempunitflag == 0){
        TemperatureUnits = "C";
      }
      if(tempunitflag == 1){
        TemperatureUnits = "F";
        double buff = DisplayTemp.toInt();
        buff = (buff * 9/5) + 32;
        int buff1 = round(buff);
        DisplayTemp = String(buff1);
      }
      if(flowunitflag == 0){
        Flowrateunits = "CC/M";
        int buff = Displayflow.toInt();
        buff = buff * 1000;
        Displayflow = String(buff);
      }
      if(flowunitflag == 1){
        Flowrateunits = "L/M";
      }
      if(bpunitflag == 0){
        BackPressureUnits = "KPA";
      }
      if(bpunitflag == 1){
        BackPressureUnits = "in.H20";
        float buff = DisplayBackPressure.toFloat();
        buff = (4.01865 * buff);
        int buff1 = round(buff);
        DisplayBackPressure = String(buff1); 
      }
  pinMode(26,INPUT_PULLUP);
  pinMode(27,INPUT_PULLUP);
  pinMode(28,INPUT_PULLUP);
  attachInterrupt(26,ISR_1,CHANGE);
  attachInterrupt(27,ISR_2,CHANGE);
  attachInterrupt(28,ISR_3,CHANGE);
  DisplayScreen_1();
  
  delay(3000);
//  DisplayScreen_14(DisplayMotion,DisplayTemp,DisplayHumidity,Displayflow);tempunitflag,flowunitflag,bpunitflag;
}
void loop()
{ 
//     if (Serial1.available()){
//  //  String ESP2to1 = Serial1.readString();
//      char st=Serial1.read();
//      if (st=='d'){
//      String receivedValues=Serial1.readStringUntil('\n');
//      Serial.println(receivedValues);
//      char charBuf[100];
//      receivedValues.toCharArray(charBuf, 100);
//     
//      // DisplayDate = strtok(charBuf, ",");
//      // DisplayTime = strtok(NULL, ",");
//      Displayflow = strtok(NULL, ",");
//      DisplayTemp = strtok(NULL, ",");
//      DisplayHumidity = strtok(NULL, ",");
//      DisplayMotion = strtok(NULL, ",");
//      }
//      if (st=='h'){
//      String receivedValues=Serial1.readStringUntil('\n');
//      Serial.println(receivedValues);
//      char charBuf[100];
//      receivedValues.toCharArray(charBuf, 100);
//     
//      DisplayDate = strtok(charBuf, ",");
//      DisplayYear = strtok(charBuf, ",");
//      DisplayTime = strtok(NULL, ",");
//     
//      }
//       if (st=='r'){
//     
//      }
//      if (st=='e'){
//        Screen_15Error();
//      }
//      if(timesend==true){
//         timesend=false;
//         SetTimeandDate = "t" + String(hour) + "," + String(minute) + "," +  String(day) + "," + String(month) + "," + String(year);
//        Serial1.println(SetTimeandDate);
//        Serial.println(SetTimeandDate);
//                  
//      }
//       if(startsamplesend==true){
//         startsamplesend=false;
//         StartSamplingSend = "a" + String(flow) + "," + String(calibrate) + ","  + String(runtimehours) + "," + String(runtimeminutes) + ","+ String(timedelayhour) + ":" +String(timedelayminutes) + "," + String(updates);
//         Serial1.println(StartSamplingSend);         
//      }
//       if(startcalibrate==true){
//         startcalibrate=false;
//         StartCalibrationSend = "c" + String(flow) + "," + String(calibrate);
//        Serial1.println(StartCalibrationSend);        
//      }
//       if(stopcalibrate==true){
//         stopcalibrate=false;
//         StopCalibrationSend = "s";
//        Serial1.println(StopCalibrationSend);     
//      }
//}
//      unsigned long DisplaycurrentTime = millis();
//      if (DisplaycurrentTime - DisplaypreviousTime >= DisplayInterval){
//      
//      timendate(DisplayTime,DisplayDate);
//      battarycharging(battarystrength);
//      battarystrength = battarystrength + 20;
//      Serial.print("DisplayDate "); Serial.println(DisplayDate);
//      Serial.print("DisplayTime "); Serial.println(DisplayTime);
//      Serial.print("Displayflow "); Serial.println(Displayflow);
//      Serial.print("DisplayTemp "); Serial.println(DisplayTemp);
//      Serial.print("DisplayHumidity "); Serial.println(DisplayHumidity);
//      Serial.print("DisplayMotion "); Serial.println(DisplayMotion);
//      DisplaypreviousTime = DisplaycurrentTime;
//    } 
 
  
  // Screen 2
  if(Screen1 == true){
    Screen1 = false;
    Screen2 = true;
    background_display();
//    timendate(DisplayTime,DisplayDate);
    headerunderline();
    footer();
    Screen_2T0();
    Screen_2T1();
    transition = 0;
    Serial.println("Screen_2T0()");
  }
if((Screendown == true || Screenup == true) && transition == 0 && Screen2 == true ){
      Screendown = false;
      Screenup = false;
      Screenenter = false;
      Screen_2T1();
      Serial.print("runtimehours: "); Serial.println(runtimehours);
      Serial.println("Draw 1");
    }
    if((Screendown == true || Screenup == true) && transition == 1 && Screen2 == true){
      Screendown = false;
      Screenup = false;
      Screenenter = false;
      Screen_2T2();
      Serial.println("Draw 2");
    }
    if((Screendown == true || Screenup == true) && transition == 2 && Screen2 == true ){
      Screendown = false;
      Screenup = false;
      Screenenter = false;
      Screen_2T3();
      Serial.println("Draw 3");
    }
    if((Screendown == true || Screenup == true) && transition == 3 && Screen2 == true){
      Screendown = false;
      Screenup = false;
      Screenenter = false;
      Screen_2T4();
      Serial.println("Draw 4");
    }
    if((Screendown == true || Screenup == true) && transition == 4 && Screen2 == true ){
      Screendown = false;
      Screenup = false;
      Screenenter = false;
      Screen_2T5();
      Serial.println("Draw 5");
    }
    // Screen 3TemperatureUnits="",Flowrateunits="",BackPressureUnits = " tempunitflag,flowunitflag,bpunitflag;
    if(Screenenter == true && transition == 0 && Screen2 == true){
      Screenenter = false;
      Screen2 = false;
      Screen3 = true;
      // Screen_3STARTSAMPLING();
      // Screen_3T1();
//      if(Flowrateunits[0] == 'C'){
//        flow = flow * 1000;
//      }
      flow = 5.00;
      Screen_5Setflow(Flowrateunits);
      Screen_5Setflowvalue(flow,Flowrateunits);
      Serial.println("Screen_3T1");
    }
    if(Screenup == true && Screen3 == true){
      Screenup = false;
      if(Flowrateunits[0]=='L'){
        flow = flow + 0.05;
        flow = constrain(flow, 5.00, 15.00);
      }
//      else if(Flowrateunits[0]=='C'){
//        flow = flow + 50;
//        flow = constrain(flow, 5000, 15000);
//      }
      Screen_5Setflowvalue(flow,Flowrateunits);
      Serial.println("Screen_3yes");
    }
    if(Screendown == true && Screen3 == true){
      Screendown = false;
      if(Flowrateunits[0]=='L'){
        flow = flow - 0.05;
        flow = constrain(flow, 5.00, 15.00);
      }
//      else if(Flowrateunits[0]=='C'){
//        flow = flow - 50;
//        flow = constrain(flow, 5000, 15000);
//      }
      Screen_5Setflowvalue(flow,Flowrateunits);
      Serial.println("Screen_3no");
    }
    if(Screenenter == true && Screen3 == true){
      Screenenter = false;
      Screen3 = false;
      Screen3_1 = true;
      // Screen2 = true;
//      if(Flowrateunits[0] == 'C'){
//        flow = flow / 1000;
//      }
      writeflow(flow);
      // Screen_2T0();
      // Screen_2T1();
      Screen_5PERMISSIONCALIBRATE();
      Screen_5PERMISSIONCALIBRATET1();
      PERMISSIONCALIBRATE_no = true;
      // transition = 0;
      Serial.println("Screen_5PERMISSIONCALIBRATET1");
    }
    if(Screenup == true && Screen3_1 == true && PERMISSIONCALIBRATE_no == true){
      Screenup = false;
      PERMISSIONCALIBRATE_yes = true;
      PERMISSIONCALIBRATE_no = false;
      Screen_5PERMISSIONCALIBRATET2();
      Serial.println("Screen_5PERMISSIONCALIBRATET2");
    }
    if(Screendown == true && Screen3_1 == true && PERMISSIONCALIBRATE_yes == true){
      Screendown = false;
      PERMISSIONCALIBRATE_no = true;
      PERMISSIONCALIBRATE_yes = false;
      Screen_5PERMISSIONCALIBRATET1();
      Serial.println("Screen_5PERMISSIONCALIBRATET1");
    }
    if(Screenenter == true && Screen3_1 == true  && PERMISSIONCALIBRATE_no == true){
      Screenenter = false;
      Screen3_1 = false;
      PERMISSIONCALIBRATE_no = false;
      PERMISSIONCALIBRATE_yes = false;
      Screen2 = true;
      Screen_2T0();
      Screen_2T1();
      transition = 0;
      
      Serial.println("Screen_5PERMISSIONCALIBRATET1");
    }
    if(Screenenter == true && Screen3_1 == true && PERMISSIONCALIBRATE_yes == true){
      Screenenter = false;
      Screen2 = false;
      Screen3_1 = false;
      Screen3_1_1 = true;
      Screen_6Calibrate();
      if(Flowrateunits[0] == 'C'){
        flow = flow * 1000;
      }
      Screen_5Setflowvalue(flow,Flowrateunits);
      startcalibrate = true;
      writecal(calibrate);
      // Screen_5Setflow();
      // Screen_5Setflowvalue(flow);
      Serial.println("Screen_5_0");
    }
    if(Screenup == true && Screen3_1_1 == true){
      Screenup = false;
      calibrate = calibrate + 0.05;
      Serial.println(calibrate);
    }
    if(Screendown == true && Screen3_1_1 == true){
      Screendown = false;
      calibrate = calibrate - 0.05;
      Serial.println(calibrate);
    }
    if(Screenenter == true && Screen3_1_1 == true){
      Screenenter = false;
      Screen3_1 = false;
      Screen3_2 = true;
      Screen3_1_1 = false;
      if(Flowrateunits[0] == 'C'){
        flow = flow / 1000;
      }
      writecal(calibrate);
      PERMISSIONRUNTIME_yes = false;
      PERMISSIONCALIBRATE_yes = false;
      Screen_5PERMISSIONRUNTIME();
      Screen_5PERMISSIONRUNTIMET1();
      PERMISSIONRUNTIME_no = true;
      Serial.println("Screen_2T0");
    }
    if(Screenup == true && Screen3_2 == true){
      Screenup = false;
      PERMISSIONRUNTIME_yes = true;
      PERMISSIONRUNTIME_no = false;
      Screen_5PERMISSIONRUNTIMET2();
      Serial.println("Screen_5PERMISSIONRUNTIMET2");
    }
    if(Screendown == true && Screen3_2 == true){
      Screendown = false;
      PERMISSIONRUNTIME_yes = false;
      PERMISSIONRUNTIME_no = true;
     Screen_5PERMISSIONRUNTIMET1();
      Serial.println("Screen_5PERMISSIONRUNTIMET1");
    }
    if(Screenenter == true && Screen3_2 == true  && PERMISSIONRUNTIME_no == true){
      Screenenter = false;
      Screen3_2 = false;
      Screen3_2_1 = false;
      PERMISSIONCALIBRATE_no = false;
      Screen2 = true;
      Screen_2T0();
      Screen_2T1();
      transition = 0;
      Serial.println("Screen_5PERMISSIONRUNTIMET1");
    }
     if(Screenenter == true && Screen3_2 == true && PERMISSIONRUNTIME_yes == true){
      Screenenter = false;
      Screen2 = false;
       Screen3_2 = false;
      Screen3_2_1 = true;
      PERMISSIONRUNTIME_yes = false;
      Screen_7Runtime();
      Screen_7SetRuntime(displaystring(runtimehours,runtimeminutes));
      Serial.println("Screen_7_0");
    }
    if(Screenup == true && Screen3_2_1 == true){
      Screenup = false;
      String hours;
      String minutes;
      runtimeminutes = runtimeminutes + 1;
      if(runtimeminutes == 60){
        runtimehours = runtimehours + 1;
         runtimeminutes = 0;
        if(runtimehours == 24){
          runtimehours = 0;
         }
      }
      Screen_7SetRuntime(displaystring(runtimehours,runtimeminutes));
      Serial.println("Screen_7yes");
    }
    if(Screendown == true && Screen3_2_1 == true){
      Screendown = false;
       String hours;
      String minutes;
       runtimeminutes = runtimeminutes - 1;
      if(runtimeminutes == -1){
        runtimehours = runtimehours - 1;
        runtimeminutes = 59;
         if(runtimehours == -1){
          runtimehours = 0;
          runtimeminutes = 0;
         }
        
      }
      Screen_7SetRuntime(displaystring(runtimehours,runtimeminutes));
      Serial.println("Screen_7no");
    }
    if(Screenenter == true && Screen3_2_1 == true){
      Screenenter = false;
      Screen3_2_1 = false;
//      Screen2 = true;
      PERMISSIONCALIBRATE_yes = false;
//      writeruntimehour(runtimehours,4);
//      writeruntimehour(runtimeminutes,5);
      Screenenter = false;
      Screen2 = false;
      Screen7from3_2_1 = true;
      Screen7 = true;
      Screen_3STARTSAMPLING();
      Screen_3T1();
      StartSampaling_no = true;
      Serial.println("Screen_2T0");
    }
   // Screen 5
   if(Screenenter == true && transition == 1 && Screen2 == true){
      Screenenter = false;
      Screen2 = false;
      Screen5 = true;
       if(Flowrateunits[0] == 'C'){
        flow = flow * 1000;
      }
      Screen_6Calibrate();
      Screen_5Setflowvalue(flow,Flowrateunits);
      startcalibrate = true;
      writecal(calibrate);
      
      // Screen_5Setflow();
      // Screen_5Setflowvalue(flow);
      Serial.println("Screen_5_0");
    }
    if(Screenup == true && Screen5 == true){
      Screenup = false;
      calibrate = calibrate + 0.05;
      Serial.println("Screen_6yes");
    }
    if(Screendown == true && Screen5 == true){
      Screendown = false;
      calibrate = calibrate - 0.05;
      Serial.println("Screen_6no");
    }
    if(Screenenter == true && Screen5 == true){
      Screenenter = false;
//      Screen5 = false;
      Screen2 = true;
      if(Flowrateunits[0] == 'C'){
        flow = flow / 1000;
      }
//      confirmscreen(Screenenter,Screenup,Screendown,"H",enterflag);
      writecal(calibrate);
      Screen_2T0();
      Screen_2T2();
     
      stopcalibrate = true;
      transition = 1;
      Serial.println("Screen_2T0");
    }
  // Screen 6 
    if(Screenenter == true && transition == 2 && Screen2 == true){
      Screenenter = false;
      Screen2 = false;
      Screen6 = true;
      
      Screen_7Runtime();
      Screen_7SetRuntime(displaystring(runtimehours,runtimeminutes));
      Serial.println("Screen_7_0");
    }
    if(Screenup == true && Screen6 == true){
      Screenup = false;
      String hours;
      String minutes;
      runtimeminutes = runtimeminutes + 1;
      if(runtimeminutes == 60){
        runtimehours = runtimehours + 1;
         runtimeminutes = 0;
        if(runtimehours == 24){
          runtimehours = 0;
         }
      }
      Screen_7SetRuntime(displaystring(runtimehours,runtimeminutes));
      Serial.println("Screen_7yes");
    }
    if(Screendown == true && Screen6 == true){
      Screendown = false;
       String hours;
      String minutes;
       runtimeminutes = runtimeminutes - 1;
      if(runtimeminutes == -1){
        runtimehours = runtimehours - 1;
        runtimeminutes = 59;
         if(runtimehours == -1){
          runtimehours = 0;
          runtimeminutes = 0;
         }
        
      }
    Screen_7SetRuntime(displaystring(runtimehours,runtimeminutes));
    }
    if(Screenenter == true && Screen6 == true){
      Screenenter = false;
      Screen6 = false;
      Screen2 = true;
//      writeruntimehour(runtimehours,4);
//      writeruntimehour(runtimeminutes,5);
      Screen_2T0();
      Screen_2T3();
      transition = 2;
      Serial.println("Screen_2T0");
    }
 
    // Screen 7 
    if(Screenenter == true && transition == 3 && Screen2 == true){
      Screenenter = false;
      Screen2 = false;
      Screen7 = true;
      Screen_3STARTSAMPLING();
      Screen_3T1();
      StartSampaling_no = true;
      Serial.println("Screen_6_0");
    }
    if(Screenup == true && Screen7 == true){
      Screenup = false;
      StartSampaling_yes = true;
      StartSampaling_no = false;
      Screen_3T2();
      Serial.println("Screen_3yes");
    }
    if(Screendown == true && Screen7 == true){
      Screendown = false;
      StartSampaling_yes = false;
      StartSampaling_no = true;
      Screen_3T1();
      Serial.println("Screen_3no");
    }
     if(Screenenter == true && Screen7from3_2_1 == true  && StartSampaling_no == true){
      Screenenter = false;
      Screen7from3_2_1 = false;
      Screen7 = false;
      StartSampaling_no = false;
      Screen2 = true;
      Screen_2T0();
      Screen_2T1();
      transition = 0;
      Serial.println("Screen_2T0");
    }
    if(Screenenter == true && Screen7 == true  && StartSampaling_no == true && Screen7from3_2_1 == false){
      Screenenter = false;
      Screen7 = false;
      StartSampaling_no = false;
      Screen2 = true;
      Screen_2T0();
      Screen_2T1();
      transition = 3;
      Serial.println("Screen_2T0");
    }
    if(Screenenter == true && Screen7 == true && StartSampaling_yes == true){
      Screenenter = false;
      Screen7 = false;
      StartSampaling_yes = false;
      Screen14 = true;
      startsamplesend = true;
//      Screen_15DelayScreen();
//      delay(5000);
      DisplayScreen_14(DisplayMotion,DisplayTemp,DisplayHumidity,Displayflow,DisplayBackPressure,TemperatureUnits,BackPressureUnits,Flowrateunits,DisplayVolume,DisplayTimeHours);
      unsigned long DisplaycurrentTime = millis();
      DisplaypreviousTime = DisplaycurrentTime - 1000;
      if (DisplaycurrentTime - DisplaypreviousTime >= DisplayInterval){
      
      DisplayScreen_14_Update(DisplayMotion,DisplayTemp,DisplayHumidity,Displayflow,DisplayBackPressure,TemperatureUnits,BackPressureUnits,Flowrateunits,DisplayVolume,DisplayTimeHours);
      timendate(DisplayTime,DisplayDate);
//      Serial.print("DisplayDate "); Serial.println(DisplayDate);
//      Serial.print("DisplayTime "); Serial.println(DisplayTime);
//      Serial.print("Displayflow "); Serial.println(Displayflow);
//      Serial.print("DisplayTemp "); Serial.println(DisplayTemp);
//      Serial.print("DisplayHumidity "); Serial.println(DisplayHumidity);
//      Serial.print("DisplayMotion "); Serial.println(DisplayMotion);
    DisplaypreviousTime = DisplaycurrentTime;
    }
      Serial.println("Screen_2T0");
    }
     if(Screenenter == true && Screen14 == true){
      Screenenter = false;
      Screen7 = false;
      Screen14 = false;
      Screen4 = true;
      Screen_4STOPSAMPLING();
      Screen_4T1();
      StopSampaling_no = true;
      Serial.println("Screen_2T0");
    }
    if(Screenup == true && Screen4 == true){
      Screenup = false;
      StopSampaling_yes = true;
      StopSampaling_no = false;
      Screen_4T2();
      Serial.println("Screen_3yes");
    }
    if(Screendown == true && Screen4 == true){
      Screendown = false;
      StopSampaling_yes = false;
      StopSampaling_no = true;
      Screen_4T1();
      Serial.println("Screen_3no");
    }
    if(Screenenter == true && Screen4 == true  && StopSampaling_no == true){
      Screenenter = false;
      Screen4 = false;
      Screen7 = false;
      StopSampaling_no = false;
      Screen14 = true; 
      DisplayScreen_14(DisplayMotion,DisplayTemp,DisplayHumidity,Displayflow,DisplayBackPressure,TemperatureUnits,BackPressureUnits,Flowrateunits,DisplayVolume,DisplayTimeHours);
      Serial.println("Screen_2T0");
    }
    if(Screenenter == true && Screen4 == true && StopSampaling_yes == true){
      Screenenter = false;
      Screen4 = false;
      Screen2 = true;
      Screen7 = false;
      StopSampaling_yes = false;
      stopsamplesend=true;
      Screen_2T0();
      Screen_2T1();
      transition = 0;
      Serial.println("Screen_2T0");
    }
      // Screen 8
    if(Screenenter == true && transition == 4 && Screen2 == true){
      Screenenter = false;
      Screen1 = false;
      Screen2 = false;
      Screen3 = false;
      Screen4 = false;
      Screen5 = false;
      Screen6 = false;
      Screen7 = false;
      Screen8 = true;
      Screen_8T0();
      Screen_8T1();
      settings_transition = 0;
      Serial.println("Screen_8_0");
    }
     if((Screendown == true || Screenup == true) && settings_transition == 0 && Screen8 == true){
      Screendown = false;
      Screenup = false;
      Screen_8T1();
      Serial.println("S Draw 1");
    }
    if((Screendown == true || Screenup == true) && settings_transition == 1 && Screen8 == true){
      Screendown = false;
      Screenup = false;
      Screen_8T2();
      Serial.println("S Draw 2");
    }
    if((Screendown == true || Screenup == true) && settings_transition == 2 && Screen8 == true){
      Screendown = false;
      Screenup = false;
      Screen_8T3();
      Serial.println("S Draw 3");
    }
    if((Screendown == true || Screenup == true) && settings_transition == 3 && Screen8 == true){
      Screendown = false;
      Screenup = false;
      Screen_8T4();
      Serial.println("S Draw 4");
    }
    if((Screendown == true || Screenup == true) && settings_transition == 4 && Screen8 == true){
      Screendown = false;
      Screenup = false;
      Screen_8T5();
      Serial.println("S Draw 5");
    }
    if((Screendown == true || Screenup == true) && settings_transition == 5 && Screen8 == true){
      Screendown = false;
      Screenup = false;
      Screen_8T6();
      Serial.println("S Draw 6");
    }
     if(Screenenter == true && settings_transition == 5 && Screen8 == true){
      Screenenter = false;
      Screen8 = false;
      Screen2 = true;
      Screen_2T0();
      Screen_2T5();
      transition = 4;
      Serial.println("Screen_2T0()");
    }
    // Screen 9
      if(Screenenter == true && settings_transition == 0 && Screen8 == true){
      Screenenter = false;
      Screen8 = false;
      Screen9 = true;
      Screen_9Timedelay();
      Screen_7SetRuntime(displaystring(timedelayhour,timedelayminutes));
      Serial.println("Screen_9_0");
    }
    if(Screenup == true && Screen9 == true){
      Screenup = false;
      timedelayminutes = timedelayminutes + 1;
      if(timedelayminutes == 60){
        timedelayhour = timedelayhour + 1;
         timedelayminutes = 0;
        if(timedelayhour == 24){
          timedelayhour = 0;
         }
      }
      Screen_7SetRuntime(displaystring(timedelayhour,timedelayminutes));
      Serial.println("Screen_9yes");
    }
    if(Screendown == true && Screen9 == true){
      Screendown = false;
       timedelayminutes = timedelayminutes - 1;
      if(timedelayminutes == -1){
        timedelayhour = timedelayhour - 1;
        timedelayminutes = 59;
         if(timedelayhour == -1){
          timedelayhour = 0;
          timedelayminutes = 0;
         }
        
      }
     Screen_7SetRuntime(displaystring(timedelayhour,timedelayminutes));
      Serial.println("Screen_7no");
    }
    if(Screenenter == true && Screen9 == true){
      Screenenter = false;
      Screen9 = false;
      Screen8 = true;
      writeruntimehour(timedelayhour,6);
      writeruntimehour(timedelayminutes,7);
      Screen_8T0();
      Screen_8T1();
      settings_transition = 0;
      Serial.println("Screen_8T0");
    }
    // Screen 10
      if(Screenenter == true && settings_transition == 1 && Screen8 == true){
      td_transition = 1;
      Screenenter = false;
      Screen8 = false;
      Screen10 = true;
      Screen10_h = true;
      Screen_10TnD();
      Screen_10Time();
      hour = constrain(hour, 0, 23);
      minute = constrain(minute, 0, 59);
      day = constrain(day, 1, 31);
      month = constrain(month, 1, 12);
      year = constrain(year, 2000, 2100);
      Screen_10TimevalueT1(hour,minute,sec); //hour,minute,sec,day,month,year
      Screen_10Date();
      Screen_10Datevalue(day,month,year);
      Serial.println("Screen_10_0");
    }
    if(Screenup == true && Screen10_h == true){
      Screenup = false;
      if(hour == 23){
        hour = 0;
      }
      else{
         hour = hour + 1;
         hour = constrain(hour, 0, 23);
      }
      Screen_10TimevalueT1(hour,minute,sec);
      Serial.println("Screen_10TimevalueT1");
    }
     if(Screendown == true && Screen10_h == true){
      Screendown = false;
       if(hour == 0){
        hour = 23;
      }
      else{
         hour = hour - 1;
         hour = constrain(hour, 0, 23);
      }
      Screen_10TimevalueT1(hour,minute,sec);
      Serial.println("Screen_10TimevalueT1");
    }    
    if(Screenenter == true && Screen10_h == true && td_transition == 2){
      Screenenter = false;
      Screen10_h = false;
      Screen10_m = true;
      Screen_10TimevalueT2(hour,minute,sec);
      Serial.println("Screen_10yes");
    }
     if(Screenup == true && Screen10_m == true){
      Screenup = false;
      if(minute == 59){
        minute = 0;
      }
      else{
        minute = minute + 1;
      minute = constrain(minute, 0, 59);
      }
      Screen_10TimevalueT2(hour,minute,sec);
      Serial.println("Screen_10TimevalueT1");
    }
     if(Screendown == true && Screen10_m == true){
      Screendown = false;
      if(minute == 0){
        minute = 59;
      }
      else{
        minute = minute - 1;
      minute = constrain(minute, 0, 59);
      }
      Screen_10TimevalueT2(hour,minute,sec);
      Serial.println("Screen_10TimevalueT1");
    }    
     if(Screenenter == true && Screen10_m == true && td_transition == 3){
      Screenenter = false;
      Screen10_h = false;
      Screen10_m = false;
      Screen10_d = true;
      hour = constrain(hour, 0, 23);
      minute = constrain(minute, 0, 59);
      day = constrain(day, 1, 31);
      month = constrain(month, 1, 12);
      year = constrain(year, 2000, 2100);
      Screen_10Timevalue(hour,minute,sec);
      Screen_10DatevalueT1(day,month,year);
      Serial.println("Screen_10yes");
    }
     if(Screenup == true && Screen10_d == true){
      Screenup = false;
      if(day == 31){
        day = 1;
      }
      else{
        day = day + 1;
      day = constrain(day, 1, 31);
      }
     Screen_10DatevalueT1(day,month,year);
      Serial.println("Screen_10TimevalueT1");
    }
     if(Screendown == true && Screen10_d == true){
      Screendown = false;
      if(day == 1){
        day = 31;
      }
       else{
        day = day - 1;
      day = constrain(day, 1, 31);
      }
     Screen_10DatevalueT1(day,month,year);
      Serial.println("Screen_10TimevalueT1");
    }
    if(Screenenter == true && Screen10_d == true && td_transition == 4){
      Screenenter = false;
      Screen10_d = false;
      Screen10_mon = true;
     Screen_10DatevalueT2(day,month,year);
      Serial.println("Screen_10DatevalueT2");
    }
     if(Screenup == true && Screen10_mon == true){
      Screenup = false;
      if(month == 12){
        month = 1;
      }
      else{
        month = month + 1;
        month = constrain(month, 1, 12);
      }
     Screen_10DatevalueT2(day,month,year);
      Serial.println("Screen_10DatevalueT2");
    }
     if(Screendown == true && Screen10_mon == true){
      Screendown = false;
      if(month == 1){
        month = 12;
      }
      else{
        month = month - 1;
        month = constrain(month, 1, 12);
      }
     Screen_10DatevalueT2(day,month,year);
      Serial.println("Screen_10TimevalueT1");
    }        
    if(Screenenter == true && Screen10_mon == true && td_transition == 5){
      Screenenter = false;
      Screen10_mon = false;
      Screen10_y = true;
     Screen_10DatevalueT3(day,month,year);
     Serial.println("Screen_10yes");
    }
     if(Screenup == true && Screen10_y == true){
      Screenup = false;
      if(year == 2100){
        year = 2000;
      }
      else{
        year = year + 1;
        year = constrain(year, 2000, 2100);
      }
       Screen_10DatevalueT3(day,month,year);
      Serial.println("Screen_10DatevalueT3");
    }
     if(Screendown == true && Screen10_y == true){
      Screendown = false;
     
      if(year == 2000){
        year = 2000;
      }
      else{
         year = year - 1;
        year = constrain(year, 2000, 2100);
      }
       Screen_10DatevalueT3(day,month,year);
      Serial.println("Screen_10DatevalueT3");
    }   
    if(Screenenter == true && Screen10_y == true && td_transition == 6){
      Screenenter = false;
      Screen10 = false;
      Screen10_y = false;
      Screen8 = true;
      timesend = true;
      Screen_8T0();
      Screen_8T1();
      td_transition = 1;
      settings_transition = 0;
      Serial.println("Screen_8T0");
    }                     
//    // Screen 11
      if(Screenenter == true && settings_transition == 2 && Screen8 == true){
      Screenenter = false;
      Screen8 = false;
      Screen11 = true;
      Screen_11Setunits();
      Screen_11SetunitsT1();
      screen11_transition = 0;
      Serial.println("Screen_11SetunitsT1");
    }
    
    //
     
    if((Screenup == true || Screendown == true ) && Screen11 == true && screen11_transition == 0){
      Screenup = false;
      Screendown = false;
      Screen_11SetunitsT1();
      Serial.println("Screen_11SetunitsT1");
    }
     if(Screenenter == true && Screen11 == true && screen11_transition == 0){
      Screenenter = false;
      Screen11 = false;
      Screen11_1 = true;
      if(tempunitflag == 0){
        Screen_11tempunitsoptionT1();
      }
      if(tempunitflag == 1){
        Screen_11tempunitsoptionT2();
      }
      Serial.println("Screen_11tempunitsoptionT1");
    }
     if((Screenup == true || Screendown == true ) && Screen11 == true && screen11_transition == 1){
      Screenup = false;
      Screendown = false;
      Screen_11SetunitsT2();
      Serial.println("Screen_11SetunitsT2");
    }
     if(Screenenter == true && Screen11 == true && screen11_transition == 1){
      Screenenter = false;
      Screen11 = false;
      Screen11_2 = true;
      if(flowunitflag == 0){
        Screen_11flowunitsoptionT1();
      }
      if(flowunitflag == 1){
        Screen_11flowunitsoptionT2();
      }
      Serial.println("Screen_11flowunitsoptionT1");
    }
     if((Screenup == true || Screendown == true ) && Screen11 == true && screen11_transition == 2){
      Screenup = false;
      Screendown = false;
      Screen_11SetunitsT3();
      Serial.println("Screen_11SetunitsT2");
    }
    if(Screenenter == true && Screen11 == true && screen11_transition == 2){
      Screenenter = false;
      Screen11 = false;
      Screen11_3 = true;
      if(bpunitflag == 0)
      {
        Screen_11bpunitsoptionT1();
      }
      if(bpunitflag == 1)
      {
        Screen_11bpunitsoptionT2();
      }
      Serial.println("Screen_8T0");
    }
     if((Screenup == true || Screendown == true ) && Screen11 == true && screen11_transition == 3){
      Screenup = false;
      Screendown = false;
      Screen_11SetunitsT4();
      Serial.println("Screen_11SetunitsT2");
    }
    if(Screenenter == true && Screen11 == true && screen11_transition == 3){
      Screenenter = false;
      Screen11 = false;
      Screen8 = true;
      Screen_8T0();
      Screen_8T3();
      screen11_transition = 0;
      Serial.println("Screen_8T0");
    }
    //
    if(Screendown == true && Screen11_1 == true && screen11_transition == 0){
      Screendown = false;
      if(tempunitflag == 1){
        Screen_11tempunitsoptionT1();
        tempunitflag = 0;
        TemperatureUnits = "C";
      }
      else{
        Screen_11tempunitsoptionT2();
        tempunitflag = 1;
        TemperatureUnits = "F";
      }
      Serial.println("Screen_11tempunitsoptionT2");
    }
     if(Screenup == true && Screen11_1 == true && screen11_transition == 0){
      Screenup = false;
      if(tempunitflag == 0){
        Screen_11tempunitsoptionT2();
        tempunitflag = 1;
        TemperatureUnits = "F";
      }
      else{
        Screen_11tempunitsoptionT1();
        tempunitflag = 0;
        TemperatureUnits = "C";
      }
      Serial.println("Screen_11tempunitsoptionT1");
    }

    if(Screenenter == true && Screen11_1 == true){
      Screenenter = false;
      Screen11_1 = false;
      Screen11 = true;
      writeruntimehour(tempunitflag,8);
      Screen_11Setunits();
      Screen_11SetunitsT1();
      screen11_transition = 0;
      Serial.println("Screen_11SetunitsT1");
    }
    
    //flowunitflag,bpunitflag;

     if(Screendown == true && Screen11_2 == true && screen11_transition == 1){
      Screendown = false;
      if(flowunitflag == 1){
        Screen_11flowunitsoptionT1();
        Flowrateunits = "CC/M";
        flowunitflag = 0;
      }
      Screen_11flowunitsoptionT2();
      Flowrateunits = "L/M";
      flowunitflag = 1;
      Serial.println("Screen_11flowunitsoptionT2");
    }
     if(Screenup == true && Screen11_2 == true && screen11_transition == 1){
      Screenup = false;
      if(flowunitflag == 0){
        Screen_11flowunitsoptionT2();
        Flowrateunits = "L/M";
        flowunitflag = 1;
      }
      Screen_11flowunitsoptionT1();
      Flowrateunits = "CC/M";
      flowunitflag = 0;
      Serial.println("Screen_11flowunitsoptionT1");
    }

    //
      if(Screendown == true && Screen11_3 == true && screen11_transition == 2){
      Screendown = false;
      if(bpunitflag == 1){
        Screen_11bpunitsoptionT1();
      BackPressureUnits = "KPA";
      bpunitflag = 0;
      }
      else{
        Screen_11bpunitsoptionT2();
      BackPressureUnits = "in.H20";
      bpunitflag = 1; 
      }
      
      Serial.println("Screen_11bpunitsoptionT2");
    }
     if(Screenup == true && Screen11_3 == true && screen11_transition == 2){
      Screenup = false;
      if(bpunitflag == 0){
        Screen_11bpunitsoptionT2();
      BackPressureUnits = "in.H20";
      bpunitflag = 1; 
      }
      else{
        Screen_11bpunitsoptionT1();
        BackPressureUnits = "KPA";
        bpunitflag = 0; 
      }
      Serial.println("Screen_11bpunitsoptionT1");
    }

     if(Screenenter == true && Screen11_2 == true){
      Screenenter = false;
      Screen11_2 = false;
      Screen11 = true;
      writeruntimehour(flowunitflag,9);
       Screen_11Setunits();
      Screen_11SetunitsT2();
      Serial.println("Screen_11SetunitsT2");
    }
    if(Screenenter == true && Screen11_3 == true){
      Screenenter = false;
      Screen11_3 = false;
      Screen11 = true;
      writeruntimehour(bpunitflag,10);
       Screen_11Setunits();
      Screen_11SetunitsT3();
      Serial.println("Screen_11SetunitsT2");
    }
    // Screen 12
      if(Screenenter == true && settings_transition == 3 && Screen8 == true){
      Screenenter = false;
      Screen8 = false;
      Screen12 = true;
      updates = constrain(updates, 0, 100000);
      Screen_12Setupdateretries();
      Screen_12Setupdateretriesvalue(updates);
      Serial.println("Screen_9_0");
    }
    if(Screenup == true && Screen12 == true){
      Screenup = false;
      updates = updates + 1;
      updates = constrain(updates, 0, 100000);
      Screen_12Setupdateretriesvalue(updates);
      Serial.println("Screen_12yes");
    }
    if(Screendown == true && Screen12 == true){
      Screendown = false;
      updates = updates - 1;
      updates = constrain(updates, 0, 100000);
      Screen_12Setupdateretriesvalue(updates);
      Serial.println("Screen_12no");
    }
    if(Screenenter == true && Screen12 == true){
      Screenenter = false;
      Screen12 = false;
      Screen8 = true;
      Screen_8T0();
      Screen_8T4();
      td_transition = 3;
      Serial.println("Screen_2T0");
    }
//    
//    // Screen 13
    if(Screenenter == true && settings_transition == 4 && Screen8 == true){
      Screenenter = false;
      Screen8 = false;
      Screen13 = true;
      Screen_13Setwebdataoption();
      Screen_13webdataoptionT1();
      Serial.println("Screen_9_0");
    }
    if(Screendown == true && Screen13 == true){
      Screendown = false;
      Screen_13webdataoptionT2();
      Serial.println("Screen_12yes");
    }
    if(Screenup == true && Screen13 == true){
      Screenup = false;
      Screen_13webdataoptionT1();
      Serial.println("Screen_12no");
    }
    if(Screenenter == true && Screen13 == true){
      Screenenter = false;
      Screen13 = false;
      Screen8 = true;
      Screen_8T0();
      Screen_8T5();
      td_transition = 4;
      Serial.println("Screen_2T0");
    }
//  delay(2000);
    delay(10);
//  delay(2000);
}
