void MenuRutero() {
  while (rutero == true) {

    ENCODER();
    botonEncoder();
    situoservos();
  }
}


/*!
  @brief   Es la ruta que vamos a hacer, solo pone a 1 las agujas, si es que estan mal situadas.
  @param:  ruta_a_hacer, número que nos indica la ruta.
  @return  No devuelve nada
*/

void run_option(byte ruta_a_hacer) {

  byte desvio_afecto = 0;
  byte vamos_moviendo = 0;
  byte desvio_posicion = 0;
  bool posicion_correcta = false;
  byte atacamos_lado = 0;
  byte atacamos_placa = 0;
  byte relaciones;
  byte pin_afectado_solidario;
  byte _segunda_placa;

  for (vamos_moviendo = 0; vamos_moviendo < 10; vamos_moviendo++) {
    desvio_afecto = StructArrayRutas[ruta_a_hacer].Agujas_Afectadas_Ruta[vamos_moviendo];
    if (desvio_afecto == 99) {
      lcd.clear();
      lcd.setCursor(0, 2);
      lcd.print("TERMINADO");
      delay(2000);
      mostrar_menuRutas(ruta_a_hacer);
      break;
    } else {
      desvio_posicion = StructArrayRutas[ruta_a_hacer].estado_agujas_afectada[vamos_moviendo];
      posicion_correcta = false;
      if (MiniServo[desvio_afecto].dondeestoy == desvio_posicion) {
        posicion_correcta = true;
      } else {
        MiniServo[desvio_afecto].contador = 1;
        if (MiniServo[desvio_afecto].solidario != 99) {
          byte amigo = MiniServo[desvio_afecto].solidario;
          MiniServo[amigo].contador = 1;
        }
        mostrar_avances(ruta_a_hacer, desvio_afecto, desvio_posicion);
        delay(1500);   
      }
    }
  }
  //_pantalla();
}



void _pantalla() {
  for (byte i = 0; i < NUMERO_SERVOS; i++) {
    Serial.print(F(" orden "));
    Serial.print(i);
    Serial.print(F("  aguja ->  "));
    Serial.print(MiniServo[i].angulo_inicio);
    Serial.print(F("  placa -> "));
    Serial.print(MiniServo[i].placa);
    Serial.print(F("  estado -> "));
    Serial.print(MiniServo[i].dondeestoy);
    Serial.print(F("  pin conta "));
    Serial.print(MiniServo[i].contador);
    Serial.print(F(" relacionado "));
    Serial.println(MiniServo[i].solidario);
  }
}





void mostrar_avances(byte numero_ruta, byte desvio, byte posi) {
  String pinta;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ruta: ");
  lcd.print(numero_ruta);
  lcd.setCursor(0, 1);
  lcd.print("desvio-> ");
  lcd.print(desvio);
  lcd.setCursor(0, 2);
  lcd.print("Estado: ");
  if (posi == 0) {
    pinta = " Recto";
  } else {
    pinta = " Curvo";
  }
  lcd.print(pinta);
}

void mostrar_menuRutas(byte hojaruta) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu: RUTEANDO");
  lcd.setCursor(0, 1);
  lcd.print("destino: ");
  lcd.print(StructArrayRutas[hojaruta].nombre_de_ruta);
}
