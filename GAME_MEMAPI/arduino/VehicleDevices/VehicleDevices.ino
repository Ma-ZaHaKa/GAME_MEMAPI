#include <ArduinoJson.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <LCD_1602_RUS.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей
LCD_1602_RUS lcd_ru(0x27,16,2); // присваиваем имя LCD для дисплея

const int JSON_BUFFER_SIZE = 256;
String PROJ_CODE = "veh_device";

void setup()
{
  Serial.begin(9600);
  lcd.init();                     
  lcd.backlight();
  
  lcd_ru.init();
  lcd_ru.backlight();
}

/*void PrintLCD(const char* text, int row = 0, int col = 0)
{
  lcd.clear();
  lcd.setCursor(col, row);
  lcd.print(text);
}

void PrintLCD_RU(const char* text, int row = 0, int col = 0)
{
  lcd_ru.clear();
  lcd_ru.setCursor(col, row);
  lcd_ru.print(text);
}*/

void PrintLCD(String text, int row = 0, int col = 0)
{
  lcd.clear();
  lcd.setCursor(col, row);
  lcd.print(text);
}

void PrintLCD_RU(String text, int row = 0, int col = 0)
{
  lcd_ru.clear();
  lcd_ru.setCursor(col, row);
  lcd_ru.print(text);
}

void PrintErrorJson(String error_msg)
{
    DynamicJsonDocument responseDoc(JSON_BUFFER_SIZE);
   responseDoc["error"] = error_msg;
   serializeJson(responseDoc, Serial);
   Serial.println();
}

void PrintDataJson(String message)
{
    DynamicJsonDocument responseDoc(JSON_BUFFER_SIZE);
   responseDoc["data"] = message;
   serializeJson(responseDoc, Serial);
   Serial.println();
}

void loop()
{
  if (Serial.available())
  {
    String inputString = Serial.readStringUntil('\n');

    // Попытка разобрать JSON
    DynamicJsonDocument jsonDoc(JSON_BUFFER_SIZE);
    DeserializationError error = deserializeJson(jsonDoc, inputString);

    if (!error)
    {
      if (jsonDoc.containsKey("mode"))
      {
        if(jsonDoc["mode"] == "print" && jsonDoc.containsKey("value"))
        {
          String value = jsonDoc["value"].as<String>();
          PrintLCD_RU(value);
          PrintDataJson("OK");
        }
        
        else if(jsonDoc["mode"] == "print_wstr" && jsonDoc.containsKey("value") && jsonDoc.containsKey("descr"))
        { // {"mode":"print_wstr","value":"123","string":"пвапав","row":"1"}
          String string = jsonDoc["string"].as<String>();
          String value = jsonDoc["value"].as<String>();
          int row = 0;
          if(jsonDoc.containsKey("row")) { row = jsonDoc["row"].as<String>().toInt(); if((row < 0) || (row > 1)){ row = 0; } }
          /*int a = 42;
          float b = 3.14;
          String result = String(a) + ": " + String(b);*/
          String out = string + ": " + value;
          PrintLCD_RU(out, row);
          PrintDataJson("OK");
        }
              
        else if (jsonDoc["mode"] == "hello") { PrintDataJson(PROJ_CODE); }
        
        else { PrintErrorJson("Error find mode"); }
      }
      else { PrintErrorJson("Error key mode"); }
    }
    
    else if (inputString == "hello") { PrintDataJson(PROJ_CODE); }
    else { PrintErrorJson("Error Deserealization"); }
    
  }
}
