#include <Arduino.h>
#include <SPI.h>
#include <DAC8814.h>
#include <SPIFIFO.h>
#include <LTC2326_16.h>

int dt;

// initialzing 
int CS_DAC = 20;

// initialzing 
int Z_axis;  // the axis will be used in pi controal loop
int x_axis;
int y_axis;

// funcution 
void data_acquisition(); // data acquisiton is for data
void PI_CONTOAL(); // siginal input using pi control loop 
// pi contoal loopn will use DAC and scanner probl for z axis
void Piezo_Driver();
void PowerSupply();


/*
All GNDs -> GND
+5V -> Vin
~CS_DAC -> Teensy pin 20
~LDAC -> pin 17
~CS_ADC -> pin 21
CNV -> pin 4
BUSY -> pin 3
MISO_ADC -> pin 12
MOSI_DAC -> pin 11
SCK_ADC and SCK_DAC -> pin 13.
*/

void checkSerial()
{
  
  String serialString;
  
  if(Serial.available() > 0)
  {
    for(int i = 0; i < 2; i++)  // Accepts 2 char commands
    {
      delay(1); // This seems to be necessary, not sure why
      char inChar = Serial.read();
      serialString += inChar;
    }
  }
  serialCommand(serialString);
  serialString = "";
}
void setup() 
{
  
}

void loop() {
  // put your main code here, to run repeatedly:
}

void serialCommand(String str)
{
  
  String command = "00";
  
  if (str.length() > 0)
  {     
      // Get the first 2 characters of str and store them in command:
      for(int i = 0; i < 2; i++)
      {
        command[i] = str[i];
      }
      
      
      if(command == "SE") // Enable serial communications
      {
        Serial.begin(115200);
        Serial.println("SE");
        serialEnabled = true;
        digitalWriteFast(SERIAL_LED, HIGH);
      }
      
      
      else if(command == "SD") // Disable serial communications
      {
        serialEnabled = false;
        Serial.flush();
        Serial.end();
        digitalWriteFast(SERIAL_LED, LOW);
      } 
      
      
      else if(command == "SS") // Scan size in LSBs
      {
        boolean scanningEnabledOnCommand = scanningEnabled;
        int new_scanSize = Serial.parseInt();
        int xNew, yNew;
        
        // Calculate position to move to:
        xNew = (int)(((float)(x - xo) * (float)new_scanSize) / (float)scanSize) + xo;
        yNew = (int)(((float)(y - yo) * (float)new_scanSize) / (float)scanSize) + yo;
        
        scanningEnabled = false; // Pause the scan
        
        if(new_scanSize > scanSize) // Update scanSize, then move
        {
          scanSize = new_scanSize;
          moveTip(xNew, yNew);
        }
        else // Move, then update scanSize
        {
          moveTip(xNew, yNew);
          scanSize = new_scanSize;
        }
        if(scanningEnabledOnCommand) scanningEnabled = true; // Resume scanning
      }
      
      
      else if(command == "IP") // Image pixels
      {        
        boolean scanningEnabledOnCommand = scanningEnabled;
        pixelsPerLine = Serial.parseInt() * 2;
        resetScan();
        if(scanningEnabledOnCommand) scanningEnabled = true;
      }
      
      
      else if(command == "LR") // Line rate in Hz
      {
        boolean scanningEnabledOnCommand = scanningEnabled;
        lineRate = (float)Serial.parseInt() / 100.0f; // Line rate is multiplied by 100 for the transmission
        while(pixelCounter != 0); // Wait for the scanner to finish scanning a line
        scanningEnabled = false; // Pause the scan
        updateStepSizes();
        if(scanningEnabledOnCommand) scanningEnabled = true; // Resume scan
      }
     
      
      else if(command == "XO") // X-offset
      {
        boolean scanningEnabledOnCommand = scanningEnabled;
        int previous_xo = xo;
        int new_xo = Serial.parseInt();
        scanningEnabled = false; // Pause the scan
        xo = new_xo;
        moveTip(x - previous_xo + xo, y); // Move over by (xo, yo)
        if(scanningEnabledOnCommand) scanningEnabled = true; // Resume scan
      } 
      
      
      else if(command == "YO") // Y-offset
      {
        boolean scanningEnabledOnCommand = scanningEnabled;
        int previous_yo = yo;
        int new_yo = Serial.parseInt();
        scanningEnabled = false;
        yo = new_yo;
        moveTip(x, y - previous_yo + yo);
        if(scanningEnabledOnCommand) scanningEnabled = true;
      } 
      
      
      else if(command == "SP") // Setpoint in LSBs
      {
        setpoint = Serial.parseInt();
        setpointLog = logTable[abs(setpoint)];       
      } 
      
      
      else if(command == "SB") // Sample bias in LSBs
      {
        bias = Serial.parseInt();
        noInterrupts();
        dac.setOutput((uint16_t)(bias + 32768), DAC_CH_BIAS); // Set the sample bias
        interrupts();          
      }
      
      
      else if(command == "KP") // P gain
      {
        Kp = Serial.parseInt();          
      } 
      
      
      else if(command == "KI") // I gain
      {
        Ki = Serial.parseInt() * dt;        
      }
      
      
      else if(command == "EN") // Enable scanning
      {
        resetScan();
        scanningEnabled = true; 
      } 
      
      
      else if(command == "DL") // Disable scanning
      {
        scanningEnabled = false;          
      } 
      
      
      else if(command == "TE") // Tip engage
      {        
        engage();          
      } 
      
      
      else if(command == "TR") // Tip retract
      {        
        retract();          
      }
  }
}

