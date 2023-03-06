//=================================================
//
//    Dual 4-1 Mux Controller
//    
//    Controller for the m0xpd Dual 4:1 Multiplexer 
//    
//    with:
//      limits on cycle length to
//      2, 3 & 4 steps 
//      (by pressing button1 AND buttonx, x=2,3 or 4)
//    and
//      reset input, 
//      which resets to channel 1
//      on receipt of +ve going pulse on the CVin input 
//      after style of Doepfer A151 
//
//    Published under Creative Commons CC-BY-SA License
//    see https://github.com/m0xpd/4-1-Mux-for-Eurorack
//
//    m0xpd
//    v2.0
//    Feb 2023
//    Last modified 28/2/23  
//   
//==================================================
//
// Hardware Constants
// Pin Allocations
const int  clockPin = 2;          // Digital Input; clock input
const int  channel1Button = 11;   // Digital Input; channel 1 select button
const int  channel2Button = 9;    // Digital Input; channel 2 select button
const int  channel3Button = 7;    // Digital Input; channel 3 select button
const int  channel4Button = 5;    // Digital Input; channel 4 select button
const int  channel1LED = 10;      // Digital Output; channel 1 LED
const int  channel2LED = 8;       // Digital Output; channel 2 LED
const int  channel3LED = 6;       // Digital Output; channel 3 LED
const int  channel4LED = 4;       // Digital Output; channel 4 LED
const int  selA = 12;             // Digital Output; channel select line A
const int  selB = 13;             // Digital Output; channel select line B
const int  clockedIn = A2;        // Digital Input; toggle switch for clocked or free-run
const int  shuffleIn = A4;        // Digital Input; toggle switch for sequential or random
const int  cvcontrolIn = A3;      // Digital Input; toggle switch for CV control
const int  clkSourceIn = A1;      // Digital Input; toggle switch for internal or external clk source
const int  clkDivide = A0;        // Analog Input;  potentiometer for clk divider (ext clock mode) or clk rate (internal clock mode)
const int  cvIn = A7;             // Analog Input;  socket for CV Input (ext control mode) or Reset (cycle mode)

// Variables
int newOCR1A=0;
int newDivide=1;
int scan=1;
int scan2=0;              // second scan variable used otherScan() to read other channel buttons
int channelRequest=0;
int cvchannelRequest = 0;
int channel=1;
int topChannel=4;         // highest channel currently allowed (to limit step number)
int clkCount=0;
int clkMax=1;
int olddivspeed;
int ldivspeed;
int cv=0;
boolean shuffle=false;
boolean cvControl = false;
boolean cvSense = false;
boolean clocked = false;
boolean extclk = false;
boolean resetIn = false;
boolean oldresetIn = false;
boolean diagnostics=false;
boolean resetFlag = false;



void setup() {
setupHardware();
if(diagnostics){
Serial.begin(115200);
}
clocked = digitalRead(clockedIn);
shuffle=!digitalRead(shuffleIn); 
extclk=digitalRead(clkSourceIn);
cvSense=!digitalRead(cvcontrolIn);
if(cvSense){
  cv=analogRead(cvIn);
}
else{
  oldresetIn=digitalRead(cvIn);
}
 // set up internal clock...
  cli();                      
  TCCR1A = 0;                 // Reset entire TCCR1A to 0 
  TCCR1B = 0;                 // Reset entire TCCR1B to 0  
  TCCR1B |= B00000101;        //Set CS12 to 1 -> prescalar 1024   
  TIMSK1 |= B00000010;        //Set OCIE1A to 1 -> compare match A 
  OCR1A = 31250;              // this will be changed as the speed potentiometer is read in readclkdivide()
  sei();  
  readclkdivide();
}

void loop() {
clocked = digitalRead(clockedIn);
shuffle=!digitalRead(shuffleIn); 
extclk=digitalRead(clkSourceIn);
cvSense=!digitalRead(cvcontrolIn);

if(cvSense==false){
  checkReset();
}


scan=digitalRead(channel1Button);
if(scan==0){
  otherScan();
  if(scan2>0){
    topChannel=scan2;
  }
  channelRequest=1;
  changechannel();
}
scan=digitalRead(channel2Button);
if(scan==0){
  channelRequest=2;
  changechannel();
}
scan=digitalRead(channel3Button);
if(scan==0){
  if(topChannel>2){
  channelRequest=3;
  changechannel();
  }
}
scan=digitalRead(channel4Button);
if(scan==0){
  if(topChannel>3){
  channelRequest=4;
  changechannel();
  }
}


if(cvSense){
cv=analogRead(cvIn);  
cvchannelRequest=constrain(floor(cv/256)+1,1,topChannel); 
}
else{
  checkReset();
}

if(clocked){
if(clkCount>=clkMax){
  if(diagnostics){
  Serial.println(clocked); 
  Serial.println(cvSense);
  Serial.println(cvchannelRequest);
  }
  if(cvSense){
    channelRequest=cvchannelRequest;
  }
  else{
    if(shuffle==false){
      channelRequest=channel+1;
        if(channelRequest==topChannel+1){
          channelRequest=1;
      }
    }
    else{
      channelRequest=round(random(1,topChannel+1));
      if(diagnostics){
      Serial.println(channelRequest);
      }
    }
  }
  if(resetFlag==false){
  changechannel();
  }
  else{
  resetFlag=false;
  clkCount=clkMax-1;  
  }
  clkCount=0;
}

// end of clocked
}
else{
// we're not clocked, so clear the clock variables...
clkCount=0;  
if(cvSense){
   channelRequest=cvchannelRequest;
   changechannel(); 
}
if(cvSense==false){
  checkReset();
}

// end of free
}


readclkdivide();
// end of loop
}


void checkReset(){
// Read the cvIn input, 
// which is used as a RESET Input 
// when in local step control (cvSense=false)
 resetIn=bitRead(analogRead(cvIn),9);
 if(resetIn&&(oldresetIn==false)){
  channelRequest=1;
  changechannel();    // just change the channel immediately - even in clocked mode
  resetFlag=true;     // set flag to indicate that the reset operation has been asserted
  clkCount=0;
  oldresetIn=true;
 }
 else{
  oldresetIn=resetIn;
 }
}

void otherScan(){
// Scan the other Buttons (2:4) to support
// multiple button push   
  scan2=0;
  if(digitalRead(channel4Button)==0){
    scan2=4;
  }
  if(digitalRead(channel3Button)==0){
    scan2=3;
  }  
  if(digitalRead(channel2Button)==0){
    scan2=2;
  } 
}


void readclkdivide(){
// Read the rate/divide potentiometer
int divspeed=analogRead(clkDivide);
if (divspeed!=olddivspeed){
  ldivspeed=round(102.4*log(divspeed)/log(2));
  newOCR1A=(1024+20-ldivspeed)*58;
  newDivide=floor(divspeed/64)+1;
  if(extclk==true){
  clkMax=newDivide;
  }
  else{
    clkMax=1;
  }
}
if(diagnostics){
  Serial.println(OCR1A);
  Serial.println(clkMax);
}
olddivspeed=divspeed;  
}


void changechannel(){
// Change the channel  
digitalWrite(channel1LED,LOW);
digitalWrite(channel2LED,LOW);
digitalWrite(channel3LED,LOW);
digitalWrite(channel4LED,LOW); 
switch (channelRequest){
case 1:
digitalWrite(channel1LED,HIGH);
digitalWrite(selA,LOW);
digitalWrite(selB,LOW);
channel=1;
break;
case 2:
digitalWrite(channel2LED,HIGH);
digitalWrite(selA,HIGH);
digitalWrite(selB,LOW);
channel=2;
break;
case 3:
digitalWrite(channel3LED,HIGH);
digitalWrite(selA,LOW);
digitalWrite(selB,HIGH);
channel=3;
break;
case 4:
digitalWrite(channel4LED,HIGH);
digitalWrite(selA,HIGH);
digitalWrite(selB,HIGH);
channel=4;
break;
}
scan=1;
}

void pin_ISR() {
// respond to external clock input interrupts  
  cli();
  if(extclk==true){
  clkCount=clkCount+1;
  clkMax=newDivide;
  }
  sei();
}


ISR(TIMER1_COMPA_vect){
// respond to timer1 interrupts  
  cli();
  OCR1A=newOCR1A;
  TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
  if(extclk==false){
  clkCount=clkCount+1;
  clkMax=1;
  }
  sei();
}


void setupHardware(){
// hide all the hardware setup stuff here  
  pinMode(clockPin,INPUT);
  attachInterrupt(digitalPinToInterrupt(clockPin), pin_ISR, RISING); // (this to accord with the polarity of the switched clock signals fed to the system)
pinMode(selA, OUTPUT);
pinMode(selB, OUTPUT);
pinMode(channel1LED, OUTPUT);
pinMode(channel2LED, OUTPUT);
pinMode(channel3LED, OUTPUT);
pinMode(channel4LED, OUTPUT);
pinMode(channel1Button,INPUT_PULLUP);
pinMode(channel2Button,INPUT_PULLUP);
pinMode(channel3Button,INPUT_PULLUP);
pinMode(channel4Button,INPUT_PULLUP);
pinMode(clockedIn,INPUT_PULLUP);
pinMode(shuffleIn,INPUT_PULLUP);
pinMode(cvcontrolIn,INPUT_PULLUP);
pinMode(clkSourceIn,INPUT_PULLUP);
pinMode(clkDivide,INPUT);
pinMode(cvIn,INPUT);
// start on channel 1:
// set initial values of LEDs
digitalWrite(channel1LED,HIGH);
digitalWrite(channel2LED,LOW);
digitalWrite(channel3LED,LOW);
digitalWrite(channel4LED,LOW);
// set initial value of select lines
digitalWrite(selA,LOW);
digitalWrite(selB,LOW);   
}
