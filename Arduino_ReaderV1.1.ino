
  // board TX3 <-----> Reader TTL RX
  // board RX3 <-----> Reader TTL TX
  // board GND <-----> Reader GND
  // Then the reader needs to be set to CustomProtocol (Use the ReaderSoft, in SpecialParam, open Protocol, and select Protocol1=1,0x02 Head HexID 0x03 0x0D 0x0A end)

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);   //Connect to PC
    Serial3.begin(115200);  //Connect to Reader, the reader baudrate default is 115200
    Serial.println("Reader System Start\r\n");
}

//Check if the rev char is Legal tag data
unsigned char CheckIsHexChar(unsigned char bChar)
{
    if(bChar >= '0' && bChar <= '9') return 1;
    else if(bChar >= 'A' && bChar <= 'F') return 1;
    else if(bChar >= 'a' && bChar <= 'f') return 1;
    else return 0;
}

char rxdata = 0;
char rxstate = 0;
unsigned char bReadLength = 0;
String strTagID = "";
void loop() {
    // put your main code here, to run repeatedly:
    while(Serial3.available()>0)
    {
        rxdata = Serial3.read(); 
        switch(rxstate)
        {
            case 0:  //get head
               if(rxdata == 0x02)
               {
                  rxstate = 1;
                  strTagID = "";
                  bReadLength = 0;
               }
               break;
            case 1:  //Get TagID
               if(rxdata != 0x0D) //Not End
               {
                   if(CheckIsHexChar(rxdata) == 1) //check legal char
                   {
                       strTagID = strTagID + rxdata;
                       bReadLength++;
                   }
                   else 
                   {
                      strTagID = "";
                      rxstate = 0;
                   }
               }
               else
               {
                   if((bReadLength) % 2 == 0) //TagLength(HEX data) is odd 
                   {
                       if(strTagID != "")
                       {
                           Serial.print("TagID: ");
                           Serial.println(strTagID);
                       }
                   }
                   else
                   {
                       Serial.print("Unlegal Data\r\n");
                   }
                   strTagID = "";rxstate = 0;bReadLength = 0;
               }
               break;
        }
        delay(10);
    }
}
