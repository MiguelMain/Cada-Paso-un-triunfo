/*
  ######   #######  ##    ## ######## ####  ######   ##     ## ########     ###     ######  ####  #######  ##    ##
  ##    ## ##     ## ###   ## ##        ##  ##    ##  ##     ## ##     ##   ## ##   ##    ##  ##  ##     ## ###   ##
  ##       ##     ## ####  ## ##        ##  ##        ##     ## ##     ##  ##   ##  ##        ##  ##     ## ####  ##
  ##       ##     ## ## ## ## ######    ##  ##   #### ##     ## ########  ##     ## ##        ##  ##     ## ## ## ##
  ##       ##     ## ##  #### ##        ##  ##    ##  ##     ## ##   ##   ######### ##        ##  ##     ## ##  ####
  ##    ## ##     ## ##   ### ##        ##  ##    ##  ##     ## ##    ##  ##     ## ##    ##  ##  ##     ## ##   ###
  ######   #######  ##    ## ##       ####  ######    #######  ##     ## ##     ##  ######  ####  #######  ##    ##

*/
void voidsetup()
{
  // motor
  pinMode(L298_ENA, OUTPUT);
  pinMode(L298_IN1, OUTPUT);
  pinMode(L298_IN2, OUTPUT);
  pinMode(L298_ENB, OUTPUT);
  pinMode(L298_IN3, OUTPUT);
  pinMode(L298_IN4, OUTPUT);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  pinMode(btn, INPUT );
  pinMode(clk2, INPUT);
  pinMode(dt2, INPUT);
  pinMode(btn2, INPUT);
  TCCR2B = TCCR2B & B11111000 | B00000110;
  digitalWrite(L298_IN1, HIGH);
  digitalWrite(L298_IN2, LOW);
  digitalWrite(L298_IN3, HIGH);
  digitalWrite(L298_IN4, LOW);
  pinMode(pinUnion, OUTPUT);
  pinMode(btjuntitos, INPUT_PULLUP);
  pinMode(bt_menu, INPUT);

}
void voidvelocimetros()
{
  Velocimetro.attach(22);
  Velocimetro1.attach(24);
  Velocimetro.write(180);
  Velocimetro1.write(180);
  delay(1500);
  Velocimetro.write(0);
  Velocimetro1.write(0);
  delay(1500);
  Velocimetro.write(180);
  Velocimetro1.write(180);
}
