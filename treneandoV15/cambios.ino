/*
                         ######     ###    ##     ## ########  ####  #######   ######
                        ##    ##   ## ##   ###   ### ##     ##  ##  ##     ## ##    ##
                        ##        ##   ##  #### #### ##     ##  ##  ##     ## ##
                        ##       ##     ## ## ### ## ########   ##  ##     ##  ######
                        ##       ######### ##     ## ##     ##  ##  ##     ##       ##
                        ##    ## ##     ## ##     ## ##     ##  ##  ##     ## ##    ##
                         ######  ##     ## ##     ## ########  ####  #######   ######
*/

void encoder_configurar()
{
  state_clk  = digitalRead(clk2);
  state_dt   = digitalRead(dt2);
  int estado_btn = digitalRead(bt_menu);
  digitalWrite (pinUnion, LOW);
  if (state_clk_old  == HIGH && state_clk  == LOW)
  {
    if (state_dt   == LOW)
    {
      rotaciones--;
    }
    else
    {
      rotaciones++;
    }
#ifdef PANTALLA
    Serial.print(" rotaciones:   ");
    Serial.print("\t" );
    Serial.print(rotaciones);
    Serial.println();
    Serial.println(iniciomarcha);
#endif
    if (rotaciones < 0) rotaciones = opciones - 1;
    if (rotaciones > opciones - 1) rotaciones = 0;
    mostrar_menu();
  }
  delay(25);
  cambio_inercia();

  if ((estado_btn_old[1]) == 0 && (estado_btn == 1))
  {
    run_option(rotaciones);
  }
  delay(15);

  state_clk_old  = state_clk ;
  if (grabando == true) {
    count[1] = min(254, max(0, count[1]));
    count[0] = min(254, max(0, count[0]));
    estado_btn_old[1] = 1;
    rotaciones = 0;
    delay(150);
  }
  else
  {
    estado_btn_old[1] = estado_btn;
  }
}

void run_option(int nuevo_valor)
{
  if (nuevo_valor == opciones - 1)
  { // he dado a salir
    salida_forzada();
    mostrar_menu_velocidad();
    return;
  }
  if (valores_posibles[nuevo_valor] == "MEDIA-1")
  {
    count[0] = 127;
    salida_forzada();
    mostrar_menu_velocidad();
    return;
  }
  if (valores_posibles[nuevo_valor] == "MEDIA-2")
  {
    count[1] = 127;
    salida_forzada();
    return;
  }
  iniciomarcha =   valores_posibles[nuevo_valor].toInt();
  grabando = true;
  mostrar_menu_cambio();
  mostrar_menu_velocidad();
}

void cambio_inercia()
{
  if ( millis() - tiempo_espera >= 500)
  {
    estadoLed = digitalRead (pinUnion);
    if ( estadoLed == HIGH ) {
      estadoLed = LOW;
    }
    else
    {
      estadoLed = HIGH;
      digitalWrite(pinUnion, estadoLed);
      tiempo_espera = millis();
    }
  }
}
void salida_forzada() 
{
  grabando = true;
  estado_btn_old[1] = 1;
  rotaciones = 0;
  delay(150);
  lcd.clear();
}
