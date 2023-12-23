/*
                        ########     ###    ########     ###    ########     ###     ######
                        ##     ##   ## ##   ##     ##   ## ##   ##     ##   ## ##   ##    ##
                        ##     ##  ##   ##  ##     ##  ##   ##  ##     ##  ##   ##  ##
                        ########  ##     ## ########  ##     ## ##     ## ##     ##  ######
                        ##        ######### ##   ##   ######### ##     ## #########       ##
                        ##        ##     ## ##    ##  ##     ## ##     ## ##     ## ##    ##
                        ##        ##     ## ##     ## ##     ## ########  ##     ##  ######
*/
void parada(int pulsador)
{
  if (juntitos == true)
  {
    pulsador = 3;
  }
  if (pulsador == 1)
  {
    analogWrite(L298_ENA, 0);
    emergencia = !emergencia;
    count[0] = 0;// iniciomarcha;
    velocidad_old = 0;//  iniciomarcha;
    Velocimetro.write(0);
#ifdef PANTALLA
    Serial.println("paro 1");
#endif
    lcd.setCursor(0, 0);
    lcd.print("PARO MANDO 1");
    delay (1000);
    mostrar_menu_velocidad();
  }
  if (pulsador == 2)
  {
    analogWrite(L298_ENB, 0);
    emergencia1 = !emergencia1;
    count[1] = 0;// iniciomarcha;
    velocidad_old1 = 0 ;//   iniciomarcha;
    Velocimetro1.write(0);
#ifdef PANTALLA
    Serial.println("paro 2");
#endif
    lcd.setCursor(0, 1);
    lcd.print("PARO MANDO 2");
    delay (1000);
    mostrar_menu_velocidad();
  }
  if (pulsador == 3)
  {
    analogWrite(L298_ENA, 0);
    analogWrite(L298_ENB, 0);
    emergencia = !emergencia;
    emergencia1 = !emergencia1;
    count[0] = 0;//iniciomarcha;
    count[1] = 0;//iniciomarcha;
    velocidad_old = 0;//iniciomarcha;
    velocidad_old1 = 0;//iniciomarcha;
    Velocimetro.write(0);
    Velocimetro1.write(0);
#ifdef PANTALLA
    Serial.println("paro los dos");
#endif
    lcd.setCursor(0, 0);
    lcd.print("PARO MANDOS ");
    lcd.setCursor(0, 1);
    lcd.print("LOS DOS");
    delay (1000);
    mostrar_menu_velocidad();
  }
}
