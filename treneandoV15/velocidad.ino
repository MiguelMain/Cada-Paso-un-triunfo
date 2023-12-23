/*
             ##     ## ######## ##        #######   ######  #### ########     ###    ########
            ##     ## ##       ##       ##     ## ##    ##  ##  ##     ##   ## ##   ##     ##
            ##     ## ##       ##       ##     ## ##        ##  ##     ##  ##   ##  ##     ##
            ##     ## ######   ##       ##     ## ##        ##  ##     ## ##     ## ##     ##
             ##   ##  ##       ##       ##     ## ##        ##  ##     ## ######### ##     ##
              ## ##   ##       ##       ##     ## ##    ##  ##  ##     ## ##     ## ##     ##
               ###    ######## ########  #######   ######  #### ########  ##     ##  ########

*/

void veloz(int potencia, int potencia2)
{
  if (grabando == false)
  {
    return;
  }

  if (millis() - demora >= tiempo_retardo_velocidad)
  {
    if (juntitos == true)
    {
      potencia2 = potencia;
    }
    demora = millis();
    reloj = map(velocidad_old, 0, 254, 0, 180);
    reloj = 180 - reloj;
    reloj1 = map(velocidad_old1, 0, 254, 0, 180);
    reloj1 = 180 - reloj1;

    if (potencia <= 10)
    {
      dato1_velocidad = false;
    }
    else
    {
      dato1_velocidad = true;
    }
    if (potencia == 0) analogWrite(L298_ENA, 0);


    if (velocidad_old < potencia)
    {
      if ((dato1_velocidad == false) && (potencia == 10))
      {
        analogWrite(L298_ENA, velocidad_old + iniciomarcha);
      }
      else
      {
        analogWrite(L298_ENA, velocidad_old );
      }
      velocidad_old++;
      pinta(potencia, velocidad_old, reloj, 1, 1);
    }

    if (velocidad_old > potencia)
    {
      analogWrite(L298_ENA, velocidad_old );
      velocidad_old--;
      pinta(potencia, velocidad_old, reloj, 0, 1);
    }

    if (velocidad_old == potencia)
    {
      velocidad_old = potencia;
    }
    //// // segundo encoder

    if (potencia2 <= 10)
    {
      dato2_velocidad = false;
    }
    else
    {
      dato2_velocidad = true;
    }
    // solo en caso que este a cero,
    if (potencia2 == 0) analogWrite(L298_ENB, 0);

    if (velocidad_old1 < potencia2)
    {
      if ((dato2_velocidad == false)
          && (potencia2 == 10))  // pongo 10 porque es lo minimo que se envia. no es un numero magico
      {
        analogWrite(L298_ENB, velocidad_old1 + iniciomarcha);
      }
      else
      {
        analogWrite(L298_ENB, velocidad_old1 );
      }
      velocidad_old1++;
      pinta(potencia2, velocidad_old1, reloj1, 1, 2);
    }
    if (velocidad_old1 > potencia2)
    {
      analogWrite(L298_ENB, velocidad_old1);
      velocidad_old1--;
      pinta(potencia2, velocidad_old1, reloj1, 0, 2);
    }
    if (velocidad_old1 == potencia2)
    {
      velocidad_old1 = potencia2;

    }
  }
  // a los semaforos y a los velocimetros
  // ahora tambien a la pantalla.
  if (juntitos == true)
  {
    muestro_semaforo(3, potencia);
  }
  else
  {
    muestro_semaforo(1, potencia);
    muestro_semaforo(2, potencia2);
  }

  Velocimetro.write(reloj);
  Velocimetro1.write(reloj1);
}
void pinta(int vamos, int llegamos, int relojero, int hacia, int quien)
{
  mostrar_menu_velocidad();
#ifdef PANTALLA
  Serial.println("vamos  cuenta  velo     sentido     deco");
  Serial.print(vamos);
  Serial.print("   ");
  Serial.print(llegamos);
  Serial.print("   ");
  Serial.print(relojero);
  Serial.print("   ");
  if (hacia == 1)
  {
    Serial.print (" Avanzo ");
  }
  else
  {
    Serial.print (" Retrocedo ");
  }
  if (quien == 1)
  {
    Serial.println (" deco 1 ");
  }
  else
  {
    Serial.println (" deco 2 ");
  }
#endif
}

void muestro_semaforo(int semaforo, int velocidad_actual)
{

  if (velocidad_actual <= iniciomarcha)
  {
    if (semaforo == 1)
    {
      miSemaforo.anula();
    }
    if (semaforo == 2)
    {
      miSemaforo1.anula();
    }
    if (semaforo == 3)
    {
      miSemaforo.anula();
      miSemaforo1.anula();
    }
  }

  if (velocidad_actual > iniciomarcha && velocidad_actual < 80)
  {
    if (semaforo == 1)
    {
      miSemaforo.verde();
    }
    if (semaforo == 2)
    {
      miSemaforo1.verde();
    }
    if (semaforo == 3)
    { // juntitos
      miSemaforo.verde();
      miSemaforo1.verde();
    }
  }
  if (velocidad_actual > 80 && velocidad_actual <  160)
  {
    if (semaforo == 1)
    {
      miSemaforo.ambar();
    }
    if (semaforo == 2)
    {
      miSemaforo1.ambar();
    }
    if (semaforo == 3)
    {
      miSemaforo.ambar();
      miSemaforo1.ambar();
    }
  }

  if (velocidad_actual >=  160)
  {
    if (semaforo == 1)
    {
      miSemaforo.rojo();
    }
    if (semaforo == 2)
    {
      miSemaforo1.rojo();
    }
    if (semaforo == 3)
    {
      miSemaforo.rojo();
      miSemaforo1.rojo();
    }
  }
}
