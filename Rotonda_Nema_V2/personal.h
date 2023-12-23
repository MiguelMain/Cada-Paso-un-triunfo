
// para controlar la velocidad del giro.  a mas numero menor velocidad.
const long REVOLUCIONES = 5500;


#define VELOCIDAD(REVOLUCIONES) delayMicroseconds(REVOLUCIONES)

#define DEBUG 2                 // para algunos print poner el 2, otro numero no sale nada.
#define PASOS_MOTOR_GRANDE 9    // pin STEP de A4988 a pin 5
#define SENTIDO_MOTOR_GRANDE 10  // pin DIR de A4988 a pin 7

// puntos donde podemos ir.
const byte direccion_lcd4li = 0x27;  // pantalla lcd 16x2

const int NUMERO_VIAS = 6;  // total puntos donde ir


struct maqueta {
  String titulo;
  int desplazamiento;
};


maqueta mirotonda[NUMERO_VIAS];


/*!
  @brief   Damos de alta los puntos. debe coincidir el numero con NUMERO_VIAS
  @return  sin retorno de datos.
*/
void agrego() {
  mirotonda[0].titulo = "via 0   ";
  mirotonda[0].desplazamiento = 0;

  // via 1
  mirotonda[1].titulo = "via 1   ";
  mirotonda[1].desplazamiento = 500;

  // via 2
  mirotonda[2].titulo = "via 2   ";
  mirotonda[2].desplazamiento = 750;

  // via 3
  mirotonda[3].titulo = "via 8   ";
  mirotonda[3].desplazamiento = 1500;

  // via 4
  mirotonda[4].titulo = "via 4    ";
  mirotonda[4].desplazamiento = 2150;

  // via 5
  mirotonda[5].titulo = "via muerta";
  mirotonda[5].desplazamiento = 3000;
}

byte body[8] = {
  B11111,
  B10101,
  B11111,
  B11111,
  B01110,
  B01010,
  B11011,
  B00000
};
byte cabezas[8] = {
  B00001,
  B00011,
  B01111,
  B01111,
  B01111,
  B00011,
  B00001,
  B00000
};
