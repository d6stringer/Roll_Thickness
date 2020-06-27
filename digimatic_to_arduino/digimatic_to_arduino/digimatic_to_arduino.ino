int req = 5; //mic REQ line goes to pin 5 through q1 (arduino high pulls request line low)
int dat = 2; //mic Data line goes to pin 2
int clk = 3; //mic Clock line goes to pin 3
int i = 0; int j = 0; int k = 0;
int sign = 0;
int decimal;
int units;
int error = 0;
int v1;
int v2;
int v3;
int v4;
int v5;
int v6;

byte mydata[14];

void setup()
{
  Serial.begin(19200);
  pinMode(req, OUTPUT);
  pinMode(clk, INPUT);
  pinMode(dat, INPUT);
  digitalWrite(clk, HIGH); // enable internal pull ups
  digitalWrite(dat, HIGH); // enable internal pull ups
  digitalWrite(req, LOW); // set request at high
}



void loop()
{ digitalWrite(req, HIGH); // generate set request
  delay(10);
  digitalWrite(req, LOW);
  for ( i = 0; i < 13; i++ ) {
    k = 0;
    for (j = 0; j < 4; j++) {
      while ( digitalRead(clk) == LOW) { } // hold until clock is high
      while ( digitalRead(clk) == HIGH) { } // hold until clock is low
      bitWrite(k, j, (digitalRead(dat) & 0x1)); // read data bits, and reverse order )
    }

    mydata[i] = k;
    sign = mydata[4];
    v1 = mydata[5];
    v2 = mydata[6];
    v3 = mydata[7];
    v4 = mydata[8];
    v5 = mydata[9];
    v6 = mydata[10];
    decimal = mydata[11];
    units = mydata[12];


  }
  long num;
  char buf[7];
  buf[0] = v1 + '0';
  buf[1] = v2 + '0';
  buf[2] = v3 + '0';
  buf[3] = v4 + '0';
  buf[4] = v5 + '0';
  buf[5] = v6 + '0';
  buf[6] = 0;
  num = atol(buf);

  if (units != 0) error = 1;
  if (sign != 0) error = 2;
  if (decimal != 3) error = 3;



  if (error != 0)
  {
    Serial.print ("error ");
    Serial.println (error);
  }
  else
  {
    Serial.println(num);
  }


  digitalWrite(req, LOW);
  error = 0;
  delay(100);
}
