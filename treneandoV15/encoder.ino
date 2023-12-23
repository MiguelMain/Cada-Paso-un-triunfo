/*
            ######## ##    ##  ######   #######  ########  ######## ########
            ##       ###   ## ##    ## ##     ## ##     ## ##       ##     ##
            ##       ####  ## ##       ##     ## ##     ## ##       ##     ##
            ######   ## ## ## ##       ##     ## ##     ## ######   ########
            ##       ##  #### ##       ##     ## ##     ## ##       ##   ##
            ##       ##   ### ##    ## ##     ## ##     ## ##       ##    ##
            ######## ##    ##  ######   #######  ########  ######## ##     ##
*/

void leer_encoder()
{
  // primer encoder
  int estado_clk = digitalRead(clk);
  int estado_dt = digitalRead(dt);
  // segudo encoder
  int estado_clk2 = digitalRead(clk2);
  int estado_dt2 = digitalRead(dt2);
  // intento igualar las variables, a ver que pasa.


  //count[0] = mando1;
  //count[1] = mando2;
  if (grabando == false)
  {
    return;
  }

  /*
    if (vueltas_para_cambio > 8) {
      inercia = 4 ;
    }
    if (millis() - demorarulo >= 3000) {
      inercia = 10;
      vueltas_para_cambio = 0;
      demorarulo = millis();
    }
  */

  if ((estado_clk_old[0] == HIGH) && (estado_clk == LOW))
  {
    if (estado_dt == LOW)
    {
      // vueltas_para_cambio ++;
      count[0] = count[0] - inercia;
    }
    else
    {
      //vueltas_para_cambio ++;
      count[0] = count[0] + inercia;
    }
    count[0] = min(254, max(0, count[0]));
  }


  if ((estado_clk_old[1] == HIGH) && (estado_clk2 == LOW))
  {
    if (estado_dt2 == LOW)
    {
      // vueltas_para_cambio ++;
      count[1] = count[1] - inercia;
    }
    else
    {
      // vueltas_para_cambio ++;
      count[1] = count[1] + inercia;
    }
    count[1] = min(254, max(0, count[1]));
  }

  delay(10);
  estado_clk_old[0] = estado_clk;
  estado_clk_old[1] = estado_clk2;


  /*
    if (juntitos == true) {
    count[1] = velocidad_old1;
    veloz(count[0], count[0]);
    } else {
    veloz(count[0], count[1]);
    }
  */

  veloz(count[0], count[1]);

}
