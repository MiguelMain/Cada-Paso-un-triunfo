/*
                        ########  ##       ##     ## ########
                        ##     ## ##       ##     ## ##
                        ##     ## ##       ##     ## ##
                        ########  ##       ##     ## ######
                        ##     ## ##       ##     ## ##
                        ##     ## ##       ##     ## ##
                        ########  ########  #######  ########
*/

void serialbluetoh()
{
  if (miBt.available())
  {
    val  = miBt.read();
    palabra = palabra + val ;
    if (val  == '*')
    {
      k1 = palabra.indexOf('│');
      dato1 = palabra.substring(0, k1);
      //      Serial.print(" ");
      dato1n = dato1.toInt();
      //   Serial.println(dato1n);
      k2 = palabra.indexOf('│', k1 + 1);
      dato2 = palabra.substring(k1 + 1, k2);
      dato2n = dato2.toInt();
      //    Serial.println(dato2n);
      palabra = " ";
      delay(3);
      DatosRecibidos = true;
    }
  }
  if (DatosRecibidos == true)
  {
    DatosRecibidos = false;
    if (dato1n == 315)
    {
      juntitos = !juntitos;
    }
    if (dato1n == 400)
    {
      parada(1); // paro
    }
    if (dato1n == 410)
    {
      parada(2); // paro
    }
    if (dato1n == 320)
    {
      sentidos(); // sentido 1
    }
    if (dato1n == 420)
    {
      sentidos2(); //sentido 2
    }

    if (dato1n < 254)
    {

      if  (dato1_cargado == false)
      {
        count[0] = dato1n;
        count[1] = dato2n;
        dato2_cargado = true;
      }

      if  (dato2_cargado == false)
      {
        count[0] = dato1n;
        count[1] = dato2n;
        dato1_cargado = true;
      }
    }

  }
  //dato1n = 0;
  //dato2n = 0;
}
