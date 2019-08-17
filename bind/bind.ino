#define NUM_BIND_PULSES 9
#define PIN_RX_BIND 11

unsigned long count;

void spektrum_send_pulses (char num)
{
  char i;
  
  for (i=0; i<num; i++)
  {
    digitalWrite(PIN_RX_BIND, LOW);
    delayMicroseconds(100);
    digitalWrite(PIN_RX_BIND, HIGH);
    delayMicroseconds(100);
  }
}
/***************************************************************************/
void setup()
{
  pinMode(PIN_RX_BIND, OUTPUT); 
  delay(100); 
  spektrum_send_pulses(NUM_BIND_PULSES);
}
/***************************************************************************/
void loop()
{
}
