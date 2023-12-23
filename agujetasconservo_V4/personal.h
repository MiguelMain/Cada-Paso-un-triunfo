// iremos poniendo los datos de cada uno.

String PRESENTACION = "FELIXTREN";


const byte direccion_placa_servo1 = 0x40;  // nombre en el programa     placa primera de sevo;
const byte direccion_placa_servo2 = 0x60;  // nombre en el programa     placa segunda de sevo;0x60 0x70


const byte direccion_lcd4li = 0x27;  // pantalla lcd 0x3f 4x20  0x27 la 2x16

const byte NUMERO_SERVOS = 8;        // agujas con servos.
const byte NUMERO_PLACAS_SERVO = 2;  // plcas pca (adafruid).

const byte RANGO_MAXIMO_EN_RUTAS = 4;  // este numero habra que ponerlo con las rutas que tengamos
const byte RANGO_MAXIMO_AGUJAS_MOVIBLES = 10;

// servos
Adafruit_PWMServoDriver placa_uno = Adafruit_PWMServoDriver(direccion_placa_servo1);
Adafruit_PWMServoDriver placa_dos = Adafruit_PWMServoDriver(direccion_placa_servo2);


enum { posicion_recto,
       posicion_curva };



struct servoStru  //
{
  byte codigo;  // sera el codigo que veamos en pantalla.
  int angulo_inicio;
  int angulo_final;
  bool dondeestoy;  // sabremos si estamos en curvo o recto, de inicio se pone en recto
  byte contador;    // bandera para saber si hay que mover el desvio
  byte placa;
  byte solidario;         // si tiene alguno relacionado.
  byte pin_placa_servo;   // donde esta pinchado dentro de la placa
  byte orden_tira_verde;  // de la tira de led, el numero que corresponde, el verde es por donde va
  byte orden_tira_rojo;   // de la tira de led, el numero que corresponde, el rojo es apagado
};

servoStru MiniServo[NUMERO_SERVOS];



void relleno_base()  //
{

  MiniServo[0].codigo = 0;
  MiniServo[0].angulo_inicio = 500;  // 800
  MiniServo[0].angulo_final = 1300;  // 1000
  MiniServo[0].dondeestoy = posicion_recto;
  MiniServo[0].placa = 1;
  MiniServo[0].pin_placa_servo = 0;
  MiniServo[0].solidario = 99;
  MiniServo[0].contador = 0;
  MiniServo[0].orden_tira_verde = 2;
  MiniServo[0].orden_tira_rojo = 3;
  //
  MiniServo[1].codigo = 1;
  MiniServo[1].angulo_inicio = 500;
  MiniServo[1].angulo_final = 1300;
  MiniServo[1].dondeestoy = posicion_recto;
  MiniServo[1].placa = 2;
  MiniServo[1].pin_placa_servo = 0;
  MiniServo[1].solidario = 99;
  MiniServo[1].contador = 0;
  MiniServo[1].orden_tira_verde = 2;
  MiniServo[1].orden_tira_rojo = 3;
  //
  MiniServo[2].codigo = 2;
  MiniServo[2].angulo_inicio = 500;
  MiniServo[2].angulo_final = 1300;
  MiniServo[2].dondeestoy = posicion_recto;
  MiniServo[2].placa = 2;
  MiniServo[2].pin_placa_servo = 1;
  MiniServo[2].solidario = 99;
  MiniServo[2].contador = 0;
  MiniServo[2].orden_tira_verde = 0;
  MiniServo[2].orden_tira_rojo = 1;
  //
  MiniServo[3].codigo = 3;
  MiniServo[3].angulo_inicio = 500;
  MiniServo[3].angulo_final = 1300;
  MiniServo[3].dondeestoy = posicion_recto;
  MiniServo[3].placa = 2;
  MiniServo[3].pin_placa_servo = 3;
  MiniServo[3].solidario = 99;
  MiniServo[3].contador = 0;
  MiniServo[3].orden_tira_verde = 6;
  MiniServo[3].orden_tira_rojo = 7;
  //
  MiniServo[4].codigo = 4;
  MiniServo[4].angulo_inicio = 500;
  MiniServo[4].angulo_final = 1300;
  MiniServo[4].dondeestoy = posicion_recto;
  MiniServo[4].placa = 1;
  MiniServo[4].pin_placa_servo = 7;
  MiniServo[4].solidario = 99;
  MiniServo[4].contador = 0;
  MiniServo[4].orden_tira_verde = 8;
  MiniServo[4].orden_tira_rojo = 9;
  //
  //
  MiniServo[5].codigo = 5;
  MiniServo[5].angulo_inicio = 500;
  MiniServo[5].angulo_final = 1300;
  MiniServo[5].dondeestoy = posicion_recto;
  MiniServo[5].placa = 1;
  MiniServo[5].pin_placa_servo = 9;
  MiniServo[5].solidario = 99;
  MiniServo[5].contador = 0;
  MiniServo[5].orden_tira_verde = 10;
  MiniServo[5].orden_tira_rojo = 11;
  //
  MiniServo[6].codigo = 6;
  MiniServo[6].angulo_inicio = 500;
  MiniServo[6].angulo_final = 1300;
  MiniServo[6].dondeestoy = posicion_recto;
  MiniServo[6].placa = 1;
  MiniServo[6].pin_placa_servo = 11;
  MiniServo[6].solidario = 99;
  MiniServo[6].contador = 0;
  MiniServo[6].orden_tira_verde = 12;
  MiniServo[6].orden_tira_rojo = 13;
  //
  MiniServo[7].codigo = 7;
  MiniServo[7].angulo_inicio = 500;
  MiniServo[7].angulo_final = 1300;
  MiniServo[7].dondeestoy = posicion_recto;
  MiniServo[7].placa = 1;
  MiniServo[7].pin_placa_servo = 12;
  MiniServo[7].solidario = 99;
  MiniServo[7].contador = 0;
  MiniServo[7].orden_tira_verde = 14;
  MiniServo[7].orden_tira_rojo = 15;
}

// ------------------------
// ------------------------

struct StrayectosRutas {
  String nombre_de_ruta;
  byte Agujas_Afectadas_Ruta[RANGO_MAXIMO_AGUJAS_MOVIBLES];
  bool estado_agujas_afectada[RANGO_MAXIMO_AGUJAS_MOVIBLES];
};

StrayectosRutas StructArrayRutas[RANGO_MAXIMO_EN_RUTAS];


void MaestroRutas() {
  // Al añadir una ruta nueva, recordar actualizar la variable NUMEROMAXIMORUTAS
  // ruta 0
  StructArrayRutas[0].nombre_de_ruta = "ruta CURVO";
  // repetir tantas agujas lleve la ruta
    
  StructArrayRutas[0].Agujas_Afectadas_Ruta[0] = 1;
  StructArrayRutas[0].Agujas_Afectadas_Ruta[1] = 2;
  StructArrayRutas[0].Agujas_Afectadas_Ruta[2] = 3;
  StructArrayRutas[0].Agujas_Afectadas_Ruta[3] = 4;
  StructArrayRutas[0].Agujas_Afectadas_Ruta[4] = 5;
  StructArrayRutas[0].Agujas_Afectadas_Ruta[5] = 6;
  StructArrayRutas[0].Agujas_Afectadas_Ruta[6] = 99;

  // seran las mismas - 1 lineas que agujas lleve la ruta
  StructArrayRutas[0].estado_agujas_afectada[0] = posicion_curva;
  StructArrayRutas[0].estado_agujas_afectada[1] = posicion_curva;
  StructArrayRutas[0].estado_agujas_afectada[2] = posicion_curva;
  StructArrayRutas[0].estado_agujas_afectada[3] = posicion_curva;
  StructArrayRutas[0].estado_agujas_afectada[4] = posicion_curva;
  StructArrayRutas[0].estado_agujas_afectada[5] = posicion_curva;
  //estado_desvio_curvo;


  // ruta 1  nombre
  StructArrayRutas[1].nombre_de_ruta = "ruta calida";
  // ruta 1 agujas afectadas---------------------
  StructArrayRutas[1].Agujas_Afectadas_Ruta[0] = 0;
  StructArrayRutas[1].Agujas_Afectadas_Ruta[1] = 2;
  StructArrayRutas[1].Agujas_Afectadas_Ruta[2] = 99;
  // ruta 1 como se deben colocar las agujas
  StructArrayRutas[1].estado_agujas_afectada[0] = 0;
  StructArrayRutas[1].estado_agujas_afectada[1] = 0;


  // ruta 2


  StructArrayRutas[2].nombre_de_ruta = "Molinos";

  StructArrayRutas[2].Agujas_Afectadas_Ruta[0] = 2;
  StructArrayRutas[2].Agujas_Afectadas_Ruta[1] = 1;
  StructArrayRutas[2].Agujas_Afectadas_Ruta[2] = 4;
  StructArrayRutas[2].Agujas_Afectadas_Ruta[3] = 5;
  StructArrayRutas[2].Agujas_Afectadas_Ruta[4] = 99;

  StructArrayRutas[2].estado_agujas_afectada[0] = 1;  //estado_desvio_curvo;
  StructArrayRutas[2].estado_agujas_afectada[1] = 1;  //estado_desvio_recto;
  StructArrayRutas[2].estado_agujas_afectada[2] = 1;  //estado_desvio_recto;
  StructArrayRutas[2].estado_agujas_afectada[3] = 1;  //estado_desvio_recto;

  StructArrayRutas[3].nombre_de_ruta = "ruta RECTOS";
  // repetir tantas agujas lleve la ruta
  StructArrayRutas[3].Agujas_Afectadas_Ruta[0] = 1;
  StructArrayRutas[3].Agujas_Afectadas_Ruta[1] = 2;
  StructArrayRutas[3].Agujas_Afectadas_Ruta[2] = 3;
  StructArrayRutas[3].Agujas_Afectadas_Ruta[3] = 4;
  StructArrayRutas[3].Agujas_Afectadas_Ruta[4] = 5;
  StructArrayRutas[3].Agujas_Afectadas_Ruta[5] = 6;
  StructArrayRutas[3].Agujas_Afectadas_Ruta[6] = 99;

  // seran las mismas - 1 lineas que agujas lleve la ruta
  StructArrayRutas[3].estado_agujas_afectada[0] = posicion_recto;
  StructArrayRutas[3].estado_agujas_afectada[1] = posicion_recto;
  StructArrayRutas[3].estado_agujas_afectada[2] = posicion_recto;
  StructArrayRutas[3].estado_agujas_afectada[3] = posicion_recto;
  StructArrayRutas[3].estado_agujas_afectada[4] = posicion_recto;
  StructArrayRutas[3].estado_agujas_afectada[5] = posicion_recto;
  //estado_desvio_curvo;
}



void _cofiguracionServos() {
  Serial.print(F(""));
  Serial.print(F("codigo"));
  Serial.print("\t");
  Serial.print(F("inicio"));
  Serial.print("\t");
  Serial.print(F("final"));
  Serial.print("\t");
  Serial.print(F("placa"));
  Serial.print("\t");
  Serial.print(F(" pin placa"));
  Serial.print("\t");
  Serial.print(F(" Solidario"));
  Serial.print("\t");
  Serial.print(F(" posi "));
  Serial.print("\t");
  Serial.print(F(" luz verde"));
  Serial.print("\t");
  Serial.println(F(" luz roja"));



  for (int recorrido = 0; recorrido < NUMERO_SERVOS; recorrido++) {
    Serial.print(MiniServo[recorrido].codigo);
    Serial.print("\t");
    Serial.print(MiniServo[recorrido].angulo_inicio);
    Serial.print("\t");
    Serial.print(MiniServo[recorrido].angulo_final);
    Serial.print("\t");
    Serial.print(MiniServo[recorrido].placa);
    Serial.print("\t");
    Serial.print("\t");
    Serial.print(MiniServo[recorrido].pin_placa_servo);
    Serial.print("\t");
    Serial.print("\t");
    Serial.print(MiniServo[recorrido].solidario);
    Serial.print("\t");

    Serial.print(MiniServo[recorrido].dondeestoy);
    Serial.print("\t");
    Serial.print("\t");
    Serial.print(MiniServo[recorrido].orden_tira_verde);
    Serial.print("\t");
    Serial.print("\t");
    Serial.println(MiniServo[recorrido].orden_tira_rojo);
  }
  Serial.println(F("---------------------------------------------"));
  Serial.println("");
}
