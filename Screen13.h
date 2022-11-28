// Screen 13
void Screen_13Setwebdataoption(){
   tft.pushImage(0,60,320, 150, midscreenback);
  tft.setFreeFont(FSSB9);  // FreeSans9
  tft.setTextColor(TFT_CYAN3);
  tft.setCursor(70,90);
  tft.setTextSize(1);
  tft.print("WEB DATA OPTIONS");
}
void Screen_13webdataoptionT1(){
  tft.setFreeFont(FSS9);  // FreeSans9
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(140,130);
  tft.fillRoundRect(100,115,120, 20, 1 , TFT_GREY3);
  tft.setTextSize(1);
  tft.print("YES");
  
  tft.pushImage(100,145,120, 20, fahback);
  tft.setFreeFont(FSS9);  // FreeSans9
  tft.setTextColor(TFT_GREY3);
  tft.setCursor(145,160);
  tft.setTextSize(1);
  tft.print("NO");
}
void Screen_13webdataoptionT2(){
  tft.setFreeFont(FSS9);  // FreeSans9
  tft.setTextColor(TFT_GREY3);
  tft.setCursor(140,130);
  tft.pushImage(100,115,120, 20, celback);

  tft.setTextSize(1);
  tft.print("YES");
  tft.fillRoundRect(100,145,120, 20, 1 , TFT_GREY3);
  tft.setFreeFont(FSS9);  // FreeSans9
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(145,160);
  tft.setTextSize(1);
  tft.print("NO");
}
void DisplayScreen_13(){
//  if(Screen8_enter == true && settings_transition == 4 && Screen8 ==  true){
//    Screen2_enter = false;
//    Screen8_enter = false;
//    Screen2_down = false;
//    Screen2_up = false;
//    Screen2 = false;  
//    Screen3 = false;
//    Screen5 = false;
//    Screen6 = false;
//    Screen7 = false;
//    Screen8 = false;
//    Screen9 = false;
//    Screen10 = false;
//    Screen11 = false;
//    Screen12 = false;
//    Screen13 = true;
//    Screen_13Setwebdataoption();
////    flowrate = 00.00;
////    Screen_10SetTimenDate(flowrate);
//  }
//  if(Screen13 == true && Screen2_up == true){
//      Screen2_up = false;
//      Screen2_down = false;
//      flowrate = flowrate + 0.10;
//      Screen_10SetTimenDate(flowrate);
//    }
//  if(Screen13 == true && Screen2_down == true){
//      Screen2_up = false;
//      Screen2_down = false;
//      flowrate = flowrate - 0.10;
//      Screen_10SetTimenDate(flowrate);
//   }
//  if(Screen8_enter == true && Screen12 == true){
//    Screen8_enter = false;
//    Screen2 = false;
//    Screen8 = true;
//    Screen9 = false;
//    Screen11 = false;
//    Screen10 = false;
//    Screen12 = false;
//    Screen13 = false;
//    Screen_8T0();
//  }
  Screen_13Setwebdataoption();
  Screen_13webdataoptionT1();
  delay(1000);
  Screen_13webdataoptionT2();
  delay(1000);
}
