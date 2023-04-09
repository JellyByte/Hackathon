/* test bluetooth module HC-05
 *  Zach Martin
 *  4/5/23
 *  
 *  check out https://howtomechatronics.com/tutorials/arduino/arduino-and-hc-05-bluetooth-module-tutorial/
 */

/* SoftwareSerial is a UART in software. It allows us to create a virtual secondary UART to connect with the 
 *  HC-05, while freeing up the primary hardware UART for program/debug
 *  
 *  It cannot send and receive simultaneously
 */
#include <SoftwareSerial.h> 

/****************************************************************/
/* Define i/o pins */
// SoftwareSerial pins
#define rxd 2 
#define txd 3

#define ledPin 13 // built-in LED indicator

// HC05 Module pins
#define enable 4
#define power 5

#define pushbutton 6
#define pushbutton2 7


/****************************************************************/
/*relevant HC05 commands:
 * query name     "AT+NAME?\r\n"
 * set name       "AT+NAME=<newname>\r\n"
 * query pin      "AT+PSWD?\r\n"
 * set pin        "AT+PSWD=<newpin>\r\n"
 * 
 */

/****************************************************************/
/* function prototypes */
void bt_to_usb(void); // send all bt input to USB

SoftwareSerial bt_serial(rxd, txd); // intitialize softwareserial

/****************************************************************/
// functions


// setup, run once
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(power, OUTPUT);
  pinMode(pushbutton, INPUT);
  
  digitalWrite(ledPin, LOW);
  
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  
  Serial.println("Program begin");

  // put HC05 into AT mode
  bt_serial.begin(38400);
  digitalWrite(enable, HIGH); // for AT mode
  digitalWrite(power, HIGH); // boot device

  delay(5000); // wait for bluetooth to boot
  digitalWrite(ledPin, HIGH);       // display AT cmd beginning


  /* Set name */
  Serial.println("\r\nSetting new name. Old name is");
  bt_serial.write("AT+NAME?\r\n");  // get current name
  bt_to_usb();
  bt_serial.write("AT+NAME=HC-10\r\n");  // Set new name
  bt_to_usb();
  Serial.println("New name is:");
  bt_serial.write("AT+NAME?\r\n");  // get current name
  bt_to_usb();


  /* Set password */
  Serial.println("\r\nSetting new pin. Old pin is");
  bt_serial.write("AT+PSWD?\r\n");  // get current pin
  bt_to_usb();
  bt_serial.write("AT+PSWD=0000\r\n");  // Set new password
  bt_to_usb();
  Serial.println("New pin is:");
  bt_serial.write("AT+PSWD?\r\n");  // get current password
  bt_to_usb();

  // Return to regular serial mode
  digitalWrite(power, LOW); // reboot device
  digitalWrite(enable, LOW); // get out of AT mode

  // go back to default baud rate
  bt_serial.end();
  bt_serial.begin(9600); 
  
  delay(10);
  
  digitalWrite(power, HIGH); // bring device back up
  delay(5000); // wait for bluetooth to boot
  
  Serial.println("\r\nNow in normal user mode");
}

// ye olde superloooop
void loop() {
  char input;
  char x = 'A';
  bool  button_state = read_button();


  if(button_state)
  {
      bt_serial.println("pressed");
  }
  bool  button_state2 = read_button2();


  if(button_state2)
  {
      bt_serial.println("pressed");
  }
  

  // this just copies the characters from BT -> USB or USB->BT for debugging
//  if(bt_serial.available())
//  {
//    input = bt_serial.read(); // this will only read 1 byte I think
//    bt_serial.write(input); // loop back over bluetooth
//    Serial.print("\r\nbt: ");
//    Serial.println(input); // transmit over USB-serial
//    Serial.print("\r\n");
//  }
//
//  if(Serial.available())
//  {
//    input = Serial.read(); // this will only read 1 byte I think
//    Serial.write(input); // loop back over bluetooth
//    bt_serial.print("\r\nusb: ");
//    bt_serial.println(input); // transmit over USB-serial
//    bt_serial.print("\r\n");
//  }  
}

void bt_to_usb(void)
{
  int i = 100;

  // wait for input or else quit after 100ms
  while(i)
  {
    if(bt_serial.available()){
      Serial.write(bt_serial.read()); 
      digitalWrite(ledPin, !digitalRead(ledPin));
      i = 10;
    }
    else
    {
      i--;
      delay(1);
    }
  }
}

// read button, debounced
// the button logic is inverted (pull-up)
bool read_button(void)
{
  static bool buttonState = true;
  static int lastDebounceTime=millis();
  
  bool reading = digitalRead(pushbutton);
  
  if((millis() - lastDebounceTime) > 50 && reading != buttonState) { // 50ms
    lastDebounceTime = millis();
    buttonState = reading;
  }
  
  return !buttonState;
}
bool read_button2(void)
{
  static bool buttonState = true;
  static int lastDebounceTime=millis();
  
  bool reading = digitalRead(pushbutton2);
  
  if((millis() - lastDebounceTime) > 50 && reading != buttonState) { // 50ms
    lastDebounceTime = millis();
    buttonState = reading;
  }
  
  return !buttonState;
}
