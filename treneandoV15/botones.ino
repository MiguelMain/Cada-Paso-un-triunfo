/*

                        ########   #######  ########  #######  ##    ##
                        ##     ## ##     ##    ##    ##     ## ###   ##
                        ##     ## ##     ##    ##    ##     ## ####  ##
                        ########  ##     ##    ##    ##     ## ## ## ##
                        ##     ## ##     ##    ##    ##     ## ##  ####
                        ##     ## ##     ##    ##    ##     ## ##   ###
                        ########   #######     ##     #######  ##    ##
*/
void leer_botones()
{
  int estado_btn  = digitalRead(btn);
  int estado_btn2 = digitalRead(btn2);
  int estado_btjuntitos = digitalRead(btjuntitos);
  int estado_bt_menu = digitalRead(bt_menu);


  // este es el boton para enviar a la configuracion de poner inicio marcha a otro valor
  if ((estado_bt_menu_old == 0) && (estado_bt_menu == 1))
  {
#ifdef PANTALLA
    Serial.println("grabando on");
#endif
    grabando = false;
    mostrar_menu();
  }
  // final del boton

  if ((estado_btjuntitos_old == 0) && (estado_btjuntitos == 1))
  {
    juntitos = !juntitos;
#ifdef PANTALLA
    Serial.println("paso juntitos ");
#endif
  }

  if ((estado_btn == 0) || (estado_btn2 == 0 ))
  {
    aprieto++;
    delay(15);
  }

  if ((estado_btn_old[0]) == 0 && (estado_btn == 1))
  {
    if (aprieto > 25)
    {
      parada(1);
    }
    else
    {
      sentidos();
    }
    aprieto = 0;
  }


  if ((estado_btn_old[1]) == 0 && (estado_btn2 == 1))
  {
    if (aprieto > 25)
    {
      if (juntitos == false)
      {
        parada(2);
      }
    }
    else
    {
      if (juntitos == false)
      {
        sentidos2();
      }
    }
    aprieto = 0;
  }

  estado_btn_old[0]  = estado_btn ;
  estado_btn_old[1]  = estado_btn2;
  estado_btjuntitos_old  = estado_btjuntitos;
  estado_bt_menu_old = estado_bt_menu;
}
