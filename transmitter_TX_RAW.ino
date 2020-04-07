/*
  Project Name:   transmitter
  Developer:      Eric Klein Jr. (temp2@ericklein.com)
  Description:    Simple communication between 900mhz radios

  See README.md for target information, revision history, feature requests, etc.
*/

#define DEBUG       // Output to the serial port
//#define EXT_DEBUG   // Output to external LCD display

#include <SPI.h>
#include <RH_RF69.h>

#ifdef EXT_DEBUG
  #include <Adafruit_LiquidCrystal.h>
  #include <Wire.h>
  // LCD connection via i2c, default address #0 (A0-A2 not jumpered)
  Adafruit_LiquidCrystal lcd(0);
#endif

// Radio setup
// 400-[433]-600 for LoRa radio, 850-[868,915]-950 for 900mhz, must match transmitter
#define RF69_FREQ 915.0

// Conditional code compiles

#if defined(ARDUINO_SAMD_FEATHER_M0)
  #define RFM69_CS      8
  #define RFM69_INT     3
  #define RFM69_RST     4
#endif

#if defined (__AVR_ATmega328P__)
  #define RFM69_INT     3  // 
  #define RFM69_CS      4  //
  #define RFM69_RST     2  // "A"
#endif

// Instantiate radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// packet counter, incremented per transmission
int16_t packetnum = 0;

void setup() 
{
  #ifdef DEBUG
    Serial.begin(115200);
    while (!Serial) ;
  #endif

  #ifdef EXT_DEBUG
    //LCD setup
    lcd.begin(16, 2);
    lcd.setBacklight(HIGH);
  #endif

  // radio setup
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

   // radio reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  if (!rf69.init())
  {
    #ifdef DEBUG
      Serial.println("RFM69 radio init failed");
    #endif
    #ifdef EXT_DEBUG
      displayMessage(0, "Radio init failed");
    #endif
    while (1);
  }
  
  // Default after radio init is 434.0MHz
  if (!rf69.setFrequency(RF69_FREQ))
  {
    #ifdef DEBUG
      Serial.println("RFM69 set frequency failed");
    #endif
    #ifdef EXT_DEBUG
      displayMessage(0, "Can't set frequency");
    #endif
    while (1);
  }
  
  // Defaults after radio init are modulation GFSK_Rb250Fd250, +13dbM (for low power module), no encryption
  // For RFM69HW, 14-20dbm for power, 2nd arg must be true for 69HCW
  rf69.setTxPower(20, true);

  // The encryption key has to match the transmitter
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  
  // radio successfully set up
  #ifdef DEBUG
    Serial.print("RFM69 radio OK @ ");
    Serial.print((int)RF69_FREQ);
    Serial.println("MHz. This node is transmitter.");
  #endif
  #ifdef EXT_DEBUG
    displayMessage(0, "Radio OK, =>receiver");
    delay(2000);
  #endif
}

void loop() 
{
  delay(1000);  // Wait 1 second between transmits, could also 'sleep' here!

  char radiopacket[20] = "Hello World #";
  itoa(packetnum++, radiopacket+13, 10);
  #ifdef DEBUG
    Serial.print("Sending: ");
    Serial.println(radiopacket);
  #endif
  
  // Send a message
  rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
  rf69.waitPacketSent();

  // Now wait for a reply
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf69.waitAvailableTimeout(500))
  { 
    // Should be a reply message for us now   
    if (rf69.recv(buf, &len))
    {
      #ifdef DEBUG
          Serial.print("Received: ");
          Serial.println((char*)buf);
          Serial.print("RSSI:");
          Serial.println(rf69.lastRssi(), DEC);
      #endif
      #ifdef EXT_DEBUG
          displayMessage(0, (char*)buf);
          //displayMessage(1,(int) rf69.lastRssi());
          String rssi_Message = "RSSI: ";
          rssi_Message += rf69.lastRssi();
          displayMessage(1, rssi_Message);
          //displayMessage(1, (char*)rf69.lastRssi());
      #endif
    }
    else
    {
      #ifdef DEBUG
        Serial.println("Receive failed");
      #endif
      #ifdef EXT_DEBUG
        displayMessage(0,"No message received");
      #endif
    }
  } 
  else
  {
    #ifdef DEBUG
      Serial.println("No reply, is another RFM69 listening?");
    #endif
  }
}

#ifdef EXT_DEBUG
  void displayMessage(int row, String message)
  // first row is zero
  {
    lcd.setCursor(0, row);
    lcd.print(message);
  }
#endif