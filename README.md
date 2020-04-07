# transmitter
## What is receiver? 
Simple packet communication between 900mhz radios. reciever is half of the solution, the other half is in the transmitter application.

### Purpose
Validate simple package transmission and reception for inclusion in other solutions

### Software Dependencies
  - uses Radiohead http://www.airspayce.com/mikem/arduino/RadioHead/RadioHead-1.100.zip
    - adfruit maintains a GitHub branch at https://github.com/adafruit/RadioHead
  - uses Adafruit_LiquidCrystal https://github.com/adafruit/Adafruit_LiquidCrystal

### BOM
  - 1X: Adafruit Arduino Pro Mini 328 - 5V (part 2378) 
  - 1X: Adafruit FRM69HCW Radio Breakout (part 3070)
  - 1X: Adafruit FTDI Friend (part 284)
  - [optional]1X: 16x2 LCD panel
  - [optional]1X: Adafruit i2c character LCD backpack (part 292)
  - 1 protoboard
  - wire

### Pinouts
  - see Fritzing diagram
  - Pro A4 (SDA) for LCD backpack
  - Pro A5 (SLC) for LCD backpack

### Information Sources
    - Adafruit RFM69HCW (SPI): https://learn.adafruit.com/adafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts?view=all
    - Sparkfun RFM69HCW: https://learn.sparkfun.com/tutorials/rfm69hcw-hookup-guide/all
    - Adafruit i2c/SPI LCD Backpack: https://learn.adafruit.com/i2c-spi-lcd-backpack/
    - Radiohead main page: https://www.airspayce.com/mikem/arduino/RadioHead/index.html
    - 16x2 LCD panels: http://oomlout.com/parts/LCDD-01-guide.pdf

### Issues
  - 041719: code will shit a brick after 7 digit packet #
  - 040620: displayMessage will leave visual artifacts if message string decreases in size


### Questions
    - 041719: Why am I counting packets?
    - 041719: surprised iota isn't having a problem with arg3
    - 041719: do I buffer overrun or truncate when inbound packet is larger than defined buffer len?
    - 041719: looks like the code only waits 500ms (move to variable?) then just pushes another packet, should code replies with receiver.packetpush# to see if they packets fall far out of sync?
    - 041719: see datagram example, one of these is wrong? Why are we constantly resetting buf, len in loop?

### Learnings

### Feature Requests
 
### Revisions
    - 010318: first version, adapted from adapted from RadioHead69_RawDemo_TX
    - 040620: [FR 010318] move serial debug code to conditional compile
    - 040620: [FR 010318] display error conditions on LCD display
    - 040620: Github checkin