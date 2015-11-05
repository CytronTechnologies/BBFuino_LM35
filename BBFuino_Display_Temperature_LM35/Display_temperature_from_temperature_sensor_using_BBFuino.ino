#include <LiquidCrystal.h> //include Arduino LCD library
int temperature = 0;  //declare a variable to store obtained temperature 
const char analog_pin = A0;   //assign analog_pin with analog pin A0 
const char buzzer = 2;        //assign buzzer with digital pin 2 

LiquidCrystal lcd(8,9,4,5,6,7);  //Initialize the LCD with:
                                 //RS -->  digital pin 8
                                 //E -->   digital pin 9
                                 //DB4 --> digital pin 4 
                                 //DB5 --> digital pin 5 
                                 //DB6 --> digital pin 6 
                                 //DB7 --> digital pin 7
                                 //RS, E, DB4, DB5, DB6 and DB7 are pins of LCD
                                 //The connection between LCD and BBFuino can be refer to schematic provided
                                 
//Setup                           
void setup()
{
  pinMode(buzzer,OUTPUT);  //set buzzer (or pin 2) as output
  pinMode(analog_pin,INPUT); //set analog_pin (or pin A0) as input
  analogReference(INTERNAL); //configure the ADC reference voltage to 1.1V (internal built-in)
  lcd.begin(8,2);  //configure for 2 x 8 LCD (2 rows, 8 columns) operation
  lcd.setCursor(0,0);  //initialize the cursor at 1st position of the 1st line
  lcd.print("TEMP(C)"); //print "TEMP(C)" on LCD starting from position of cursor
}

//Main
void loop()
{
  temperature = get_temperature();  //get the temperature

  if(temperature>325)  //test if the temperature > 32.5 celsius
    beep();  //if yes, then activate the buzzer 
    
  lcd.setCursor(0,1);  //set the cursor at 1st position of the 2nd line
  lcd.print(temperature/10,DEC);  //Starting from position of cursor, print the round number part of temperature as decimal on LCD
  lcd.print('.');  //print a dot '.' on LCD
  lcd.print(temperature%10,DEC); //print the decimal part of temperature as decimal on LCD
  delay(300);  //short delay between acquisition of temperature
}

void beep()
{
  //buzzer sound pattern
  digitalWrite(buzzer,HIGH);  //buzzer on
  delay(100);  //short delay
  digitalWrite(buzzer,LOW);  //buzzer off
  delay(100);  //short delay
}

//Get temperature subroutine
int get_temperature()
{
  /*****get temperature subroutine*****
  -The formula to convert ADC reading to temperature is shown below:
   Temperature = 100 x ADC reading x 1.1 / 1024 x 10
  -The temperature sensor (LM35DZ or LM35DT) sensing range is between 0 - 100 celsius
  -Sensitivity --> 10mv per celsius
  -With the sensing range --> 0 - 100 celsius, the LM35DZ/DT voltage output --> 0 - 1V
  -With analog voltage reference = 1.1, maximum ADC value obtained = 1 / 1.1 * 1024 = 930. Hence, the ADC value range is between 0 - 930
  -A multipler 1.1 have to be introduced so that we can get 100 celsius when the value is 930 as 930 x 1.1 --> approximately 1024
  -Another multipler 10 also have to be introduced, so we can obtain the temperature in one decimal place instead of rounded number without decimal place
   eg:
   As temperature --> 61.5 celsius is sensed,
   -if multiplier 10 is introduced, average temperature store in a variable average_temperature is 615 instead of 61
   -the round number part is obtained using average_temperature / 10 --> 61
   -the decimal number part is obtained using average_temperature % 10 --> 5
   -By manipulating this, we can display the temperature with one decimal place
  ************************************/
  
  float adc_value = 0;  //declare a variable to store ADC value
  float total_temperature = 0;  //declare a variable to store sum of multiple temperatures
                                //for average temperature calculation
  int average_temperature = 0;  //declare a variable to store the average temperature calculated
  
  for(int count =0; count<100; count++)  //obtainning the sum of multiple temperatures (100 samples in this case) 
  {
    adc_value = analogRead(analog_pin);  //obtain ADC reading
    total_temperature += adc_value/1024*100*1.1*10;  //accumulate the sum of temperatures untill all samples are taken, same as total_temperature = total_temperature + adc_value/1024*100*1.1*10;
  }
  average_temperature = int(total_temperature / 100);  //calculate the average temperature, convert it from float to integer 
  return average_temperature;  //return average temperature to the main 
}





