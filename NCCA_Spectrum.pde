import processing.serial.*;

//import de.looksgood.ani.*;


Serial myPort; // The serial port

int Y_AXIS = 1;
int X_AXIS = 2;
color b1, b2, c1, c2;



String rawData;
byte[] Data = new byte[128];
void setup() {
  fullScreen();
  frameRate(40);
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[9], 115200);
  
  //Ani.init(this);

  c1 = color(168, 16, 84);
  c2 = color(154, 244, 223); 
}


void dots(byte[] Data) {
  

  //printArray(Data);
  //line(65,height-400,width-30,0);
  int b;
  blendMode(BLEND);
  for (int i = 0; i < 128; i++) {
    //if (Data[i] < 70) Data[i] = 0;
    //print(Data[i]);
    //fill(r,g,b);
    //fill(Data[i]+100);
//    rect(10 * i + 65, height - (Data[i] + 400), 3, Data[i]);
    b = Data[i];
    if (b<0) b = -b;
    //filter( BLUR, 6 );
    
    rect(10 * i + 2, 500 - b  , 9, b *2 + 5);
    //Ani.to(this, 0.5, "b", 0);
    //Ani.to(this, 1.0, "b", mouseX, Ani.BOUNCE_OUT);
    
  }
  textAlign(CENTER);
  fill(255);
  textSize(50);
  text("СПЕКТР ЧАСТОТЫ СИГНАЛА ",650, 100);
  
  blendMode(SUBTRACT);
  setGradient(0, 0, width, height, c1, c2, Y_AXIS);
  setGradient(0, 0, width, height, c2, c1, X_AXIS);   

  //rect(50, 50, 50, 50);
}

byte[] getData() {
  byte[] Data = new byte[128];
  while (true) 
  {
     myPort.clear();  
     myPort.write(65);
     //delay(20);  
    if (myPort.available()>0)
     {
       rawData = myPort.readStringUntil(10);
       if (rawData != null)
          break;
     }     
/*     if (myPort.available()>0)
     {
       rawData = myPort.readString();
       if (rawData != null)
          break;
     }     
*/
}
  int sz = (rawData.length() -2 ) / 2;
  if (sz>128)  sz = 256;
  //println(rawData);
  //println("size = ", sz);
  int c = 0;
  String el;
  for(int i =0; i<sz; i++)
  {
     el = rawData.substring(i*2, i*2+ 2);
  //   println(i, c, "[",el, "]");
     if (el !="")
     {
       Data[c] = (byte)unhex(el.toUpperCase());
       c++;
     }  
  }   
  while (c<128) 
  {
      Data[c] = 0;
      c++;
  }    
      
  return Data;
}

void draw() {

  noStroke();
  background(0);
  Data = getData();
  dots(Data);
  
  

   
}

void setGradient(int x, int y, float w, float h, color c1, color c2, int axis ) {

  //noFill();
  if (axis == X_AXIS) {  // Left to right gradient
    for (int i = x; i <= x+w; i++) {
      float inter = map(i, x, x+w, 0, 1);
      color c = lerpColor(c1, c2, inter);
      stroke(c);
      line(i, y, i, y+h);
    }
  }
}
