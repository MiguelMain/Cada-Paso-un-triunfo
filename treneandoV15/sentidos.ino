/*
                         ######  ######## ##    ## ######## #### ########   #######
                        ##    ## ##       ###   ##    ##     ##  ##     ## ##     ##
                        ##       ##       ####  ##    ##     ##  ##     ## ##     ##
                         ######  ######   ## ## ##    ##     ##  ##     ## ##     ##
                              ## ##       ##  ####    ##     ##  ##     ## ##     ##
                        ##    ## ##       ##   ###    ##     ##  ##     ## ##     ##
                         ######  ######## ##    ##    ##    #### ########   #######
*/
void sentidos()
{
  if (juntitos == true)
  {
    sentidoUnido();
    return;
  }
  int iniciocambio = velocidad_old;
  lcd.setCursor(0, 0);
  lcd.print("SENT. 1 CAMBIADO");
  delay (1000);

  for (iniciocambio ; iniciocambio > 0; iniciocambio --)
  {
    analogWrite(L298_ENA, iniciocambio);
    delay(40);
    muestro_semaforo(1, iniciocambio);
    velocidad_old = iniciocambio;
    mostrar_menu_velocidad();
    reloj = map(iniciocambio, 0, 254, 0, 180);
    reloj = 180 - reloj;
    Velocimetro.write(reloj);
  }
  count[0] = 0;      //iniciomarcha;
  velocidad_old = 0; //= iniciomarcha;
  miSemaforo.anula();

  if (sentido == false)
  {
    digitalWrite(L298_IN1, HIGH);
    digitalWrite(L298_IN2, LOW);
  }
  if (sentido == true)
  {
    digitalWrite(L298_IN1, LOW);
    digitalWrite(L298_IN2, HIGH);
  }
  sentido = !sentido;
#ifdef PANTALLA
  Serial.println("sentido uno cambiado");
#endif
  mostrar_menu_velocidad();
}
void sentidos2()
{
  if (juntitos == true)
  {
    sentidoUnido();
    return;
  }
  int iniciocambio = velocidad_old1;
  lcd.setCursor(0, 1);
  lcd.print("SENT. 2 CAMBIADO");
  delay (1000);
  for (iniciocambio ; iniciocambio > 0; iniciocambio --)
  {
    analogWrite(L298_ENB, iniciocambio);
    delay(40);
    velocidad_old1 = iniciocambio;
    muestro_semaforo(2, iniciocambio);
    reloj = map(iniciocambio, 0, 254, 0, 180);
    reloj = 180 - reloj;
    Velocimetro1.write(reloj);
    mostrar_menu_velocidad();
  }
  count[1] = 0;       //iniciomarcha;
  velocidad_old1 = 0; // iniciomarcha;
  miSemaforo1.anula();

  if (sentido2 == false) {
    digitalWrite(L298_IN3, HIGH);
    digitalWrite(L298_IN4, LOW);
  }
  if (sentido2 == true) {
    digitalWrite(L298_IN3, LOW);
    digitalWrite(L298_IN4, HIGH);
  }
  sentido2 = !sentido2;
#ifdef PANTALLA
  Serial.println("sentido dos cambiado");
#endif
  mostrar_menu_velocidad();
}

void sentidoUnido()
{
#ifdef PANTALLA
  Serial.println("cambiado sentido a los dos");
#endif
  lcd.setCursor(0, 0);
  lcd.print("SENTIDOS");
  lcd.setCursor(0, 1);
  lcd.print("CAMBIADOS");
  delay (1000);

  for (int maximo = 254 ; maximo < 0; maximo--)
  {
    analogWrite(L298_ENA, velocidad_old - maximo);
    delay(40);
    reloj = map(maximo, 0, 254, 0, 180);
    reloj = 180 - reloj;
    Velocimetro.write(reloj);
    muestro_semaforo(3, maximo);
    Velocimetro1.write(reloj);
    analogWrite(L298_ENB, velocidad_old1 - maximo);
    delay(40);
    count[1] = velocidad_old1;
  }
  analogWrite(L298_ENA, 0);
  analogWrite(L298_ENB, 0);
  count[0] = 0;// iniciomarcha;
  count[1] = 0;//iniciomarcha;
  velocidad_old = 0; // iniciomarcha;
  velocidad_old1 = 0;//iniciomarcha;
  miSemaforo.anula();
  miSemaforo1.anula();
  if (sentidojuntitos == true)
  {
    digitalWrite(L298_IN1, HIGH);
    digitalWrite(L298_IN3, HIGH);
    digitalWrite(L298_IN2, LOW);
    digitalWrite(L298_IN4, LOW);
  }
  else
  {
    digitalWrite(L298_IN2, HIGH);
    digitalWrite(L298_IN4, HIGH);
    digitalWrite(L298_IN1, LOW);
    digitalWrite(L298_IN3, LOW);
  }
  mostrar_menu_velocidad();
  sentidojuntitos = !sentidojuntitos;
}

/*
                                                                                                    dddddddd                             dddddddd
  PPPPPPPPPPPPPPPPP                   lllllll                                       iiii              d::::::d                             d::::::d
  P::::::::::::::::P                  l:::::l                                      i::::i             d::::::d                             d::::::d
  P::::::PPPPPP:::::P                 l:::::l                                       iiii              d::::::d                             d::::::d
  PP:::::P     P:::::P                l:::::l                                                         d:::::d                              d:::::d
  P::::P     P:::::P  ooooooooooo    l::::l   aaaaaaaaaaaaa  rrrrr   rrrrrrrrr  iiiiiii     ddddddddd:::::d   aaaaaaaaaaaaa      ddddddddd:::::d
  P::::P     P:::::Poo:::::::::::oo  l::::l   a::::::::::::a r::::rrr:::::::::r i:::::i   dd::::::::::::::d   a::::::::::::a   dd::::::::::::::d
  P::::PPPPPP:::::Po:::::::::::::::o l::::l   aaaaaaaaa:::::ar:::::::::::::::::r i::::i  d::::::::::::::::d   aaaaaaaaa:::::a d::::::::::::::::d
  P:::::::::::::PP o:::::ooooo:::::o l::::l            a::::arr::::::rrrrr::::::ri::::i d:::::::ddddd:::::d            a::::ad:::::::ddddd:::::d
  P::::PPPPPPPPP   o::::o     o::::o l::::l     aaaaaaa:::::a r:::::r     r:::::ri::::i d::::::d    d:::::d     aaaaaaa:::::ad::::::d    d:::::d
  P::::P           o::::o     o::::o l::::l   aa::::::::::::a r:::::r     rrrrrrri::::i d:::::d     d:::::d   aa::::::::::::ad:::::d     d:::::d
  P::::P           o::::o     o::::o l::::l  a::::aaaa::::::a r:::::r            i::::i d:::::d     d:::::d  a::::aaaa::::::ad:::::d     d:::::d
  P::::P           o::::o     o::::o l::::l a::::a    a:::::a r:::::r            i::::i d:::::d     d:::::d a::::a    a:::::ad:::::d     d:::::d
  PP::::::PP         o:::::ooooo:::::ol::::::la::::a    a:::::a r:::::r           i::::::id::::::ddddd::::::dda::::a    a:::::ad::::::ddddd::::::dd
  P::::::::P         o:::::::::::::::ol::::::la:::::aaaa::::::a r:::::r           i::::::i d:::::::::::::::::da:::::aaaa::::::a d:::::::::::::::::d
  P::::::::P          oo:::::::::::oo l::::::l a::::::::::aa:::ar:::::r           i::::::i  d:::::::::ddd::::d a::::::::::aa:::a d:::::::::ddd::::d
  PPPPPPPPPP            ooooooooooo   llllllll  aaaaaaaaaa  aaaarrrrrrr           iiiiiiii   ddddddddd   ddddd  aaaaaaaaaa  aaaa  ddddddddd   ddddd

*/

void saber_polaridad()
{
  if (grabando == false)
  {
    return;
  }
  if (((digitalRead(L298_IN1) == HIGH))
      &&  ((digitalRead(L298_IN3) == HIGH)))
  {
    digitalWrite(pinUnion, HIGH);
  }
  else if (((digitalRead(L298_IN2) == HIGH))
           &&  ((digitalRead(L298_IN4) == HIGH)))
  {
    digitalWrite(pinUnion, HIGH);
  }
  else
  {
    digitalWrite(pinUnion, LOW);
  }
}
