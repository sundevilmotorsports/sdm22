int uwu = 0;
int state = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  uwu++;
  if(uwu % 50 == 0){
    Serial1.println("hehe xd");
    Serial.println("hehe xd");
  }
  else{
    Serial1.println("owo xd");
    Serial.println("owo xd");
  }
  if(uwu == 10000) uwu = 0;

  // check state
  if(Serial1.
}
