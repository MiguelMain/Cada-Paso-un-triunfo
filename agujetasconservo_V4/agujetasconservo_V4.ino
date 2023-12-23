/*  / version para el agujetas pero solamente servos. 
                                 ,,         
           __                  o-°°|\_____/)
      (___()'`; Agujetas Servos \_/|_)     )
      /,    /`                     \  __  / 
      \\"--\\                      (_/ (_/  



// notas a dia 28 de abril 2023 

Alta nueva placa en el personal.h
1)..... sustituir la x por el nombre/numero que se quiera.
añadimos estas dos lines por cada placa
numero de direccion: 
      const byte direccion_placa_X = 0x60;
creamos el objeto:
      Adafruit_PWMServoDriver placa_X = Adafruit_PWMServoDriver(direccion_placa_x);

modulo agujetasconservo.
2).....
En el void setup()  copiamos las lineas de cualquier de las dos placas que hay ( x es el nombre que hemos dado) 
      placa_x.begin();
      placa_x.setPWMFreq(60);  // Analog NUMERO_SERVO run at ~60 Hz updates
      placa_x.setOscillatorFrequency(25000000);

En el void colocacion()  añadimos esta  linea a los dos for que hay 
      placa_x.writeMicroseconds(todos, vamos);
 
 Y para acabar, 
en el void atacarservo() 
copiamos un case entero con el numero 3 y lo ponemos a continuacion, 
tanto en el lado recto como curvo. No habria que hacer nada mas. 
siempre guardaremos una copia y realizamos los pasos.
------------------------                     ----------------------
-------------------------------------------------------------------
-------------------------------------------------------------------


daremos de alta en la pestaña de personal a los diferentes servos , 
rellenando adecuadamente cada campo, tenemos la facilidad de poder 
poner un servo en el pin que mas nos guste, no tienen porque estar 
en orden. Lo que no podemos hacer es poner dos numeros iguales o 
dos servos en el mismo numero de pin, 
si queremos "hermanar" dos servos en el campo solidario le ponemos
el numero que nos interese, hay que hacer los mismo en el otro servo
para que tanto moviendo uno como el otro, ambos se muevan. 
en caso de ser una aguja individual el campo se debe rellenar con 
el numero 99.

estamos proyectado poner led para el tco de la serie w2812 (5050). 
para solo utilizar un pin, es que de lo contrario serian dos pines
por desvio, y a na que tengamos 30 seria un manojo de cables. 

dia 29/04/2023. 

Se modifica la parte del inicio para poner la tira de led en verde
y se implementa dos nuevos campos en el struct para contemplar 
las dos posibilidades.

Casi que descargo usar la pantalla tft en i2c por el gran 
volumen de memoria que consume.

04-05
He puesto a la funcion atacaservo dos variables para controlar los pasos y el tiempo 
asi se puede personalizar, 

// esta versión es la que nos envio felix el dia 29/05. por lo tanto es la ÚLTIMA 
// hay algunos cambios, sin mucha importancia a fecha 24/12/2023

 __| |________________| |__ 
(__| |________________| |__)
   | |  Pines Usados  | |   
 __| |________________| |__ 
(__|_|________________|_|__)


  
D2 -> encoder gira a un lado.
D3 -> encoder gira a un lado.
D4 -> boton encoder. 
D6 -> boton para ruteando.
D8 -> tira de led. poner una resistencia de 480 a este cable.
D13 -> PERMENCE ENCENDIDO MIENTRAS LOS SERVOS SE MUEVAN. AL INICIO Y LUEGO AL MOVERSE
A4 -> SDA.
A5 -> SCL.

*/
//

//librerias
#include <Wire.h>                     //https://www.arduino.cc/en/reference/wire
#include <Adafruit_PWMServoDriver.h>  //https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
#include <RotaryEncoder.h>            // https://github.com/mathertel/RotaryEncoder
#include <Adafruit_NeoPixel.h>
#include "personal.h"
#include <LiquidCrystal_I2C.h>


String version = "3.20  23-12-23";

//Servos, poner los que usan,

unsigned long controlTiempoRecorro = 0;   // Llevamos el control de tiempo que ha pasado
int intervaloControlTiempoRecorrido = 5;  // Intervalo entre las veces que tiene que escribir
bool terminado_subir = true;

unsigned long controlTiempoRecorroBaja = 0;   // Llevamos el control de tiempo que ha pasado
int intervaloControlTiempoRecorridoBaja = 5;  // Intervalo entre las veces que tiene que escribir
bool terminado_bajar = true;

// encoder
const int Encoder_clk = 2;
const int Encoder_dt = 3;
const int pinBotonEncoder = 4;

int rotaciones = 0;
int estado_btn_old = 0;
byte pos = 1;
byte RANGO_MINIMO_ENCODER = 0;
byte RANGO_MAXIMO_ENCODER = NUMERO_SERVOS - 1;


// luces
const byte LED_PIN = 8;
const byte TOTAL_LED = 16;
const byte BRIGHTNESS = 20;
int cuenta;

bool rutero = false;  // si es true, es para las rutas
byte pinBotonRutero = 6;
byte estado_botonruta_old = 0;

byte longitud_ruta = 0;
byte agujafectada;
bool estado_requerido;


// led rojo ahora sera negro
int r = 0;  // 255 para rojo
int g = 0;
int b = 0;

// led verde
int r1 = 0;
int g1 = 255;  // 255 para verde
int b1 = 0;

// otros objetos de la aplicacion.
Adafruit_NeoPixel MisLuces(TOTAL_LED, LED_PIN, NEO_GRB + NEO_KHZ800);
RotaryEncoder encoder(Encoder_clk, Encoder_dt, RotaryEncoder::LatchMode::FOUR3);

// pantalla
LiquidCrystal_I2C lcd(direccion_lcd4li, 20, 4);

int rojo = MisLuces.Color(255, 0, 0);
int verde = MisLuces.Color(0, 255, 0);
// caracteres españoles
byte u2[8] = {
  B00010,
  B00100,
  B00000,
  B10001,
  B10001,
  B10001,
  B10011,
  B01101
};
byte o2[8] = {
  B00010,
  B00100,
  B01110,
  B10001,
  B10001,
  B10001,
  B01110,
  B00000
};
byte i2[8] = {
  B00001,
  B00010,
  B01100,
  B00100,
  B00100,
  B00100,
  B01110,
  B00000
};



void checkPosition() {
  encoder.tick();
}


/*!
  @brief   Cada vez que giramos el encoder, salta la interrupcion.
  @info:   Aumenta o disminuye el valor de una variable.
  @return  No devuelve nada
*/
int ENCODER() {
  encoder.tick();
  rotaciones = encoder.getPosition();

  if (rutero == false) {  // estamos con los servos

    if (rotaciones < RANGO_MINIMO_ENCODER) {
      encoder.setPosition(RANGO_MINIMO_ENCODER);
      rotaciones = RANGO_MINIMO_ENCODER;
    } else if (rotaciones > RANGO_MAXIMO_ENCODER) {  // (rotaciones >= RANGO_MAXIMO_ENCODER)
      encoder.setPosition(RANGO_MAXIMO_ENCODER);
      rotaciones = RANGO_MAXIMO_ENCODER;
    }

    if (pos != rotaciones) {
      pos = rotaciones;
      Serial.print("Encodere -> ");
      Serial.println(rotaciones);
      mostrar_posicion_encoder(rotaciones);
    }

  }  // servos


  if (rutero == true) {  // rutas

    if (rotaciones < RANGO_MINIMO_ENCODER) {
      encoder.setPosition(RANGO_MINIMO_ENCODER);
      rotaciones = RANGO_MINIMO_ENCODER;
    } else if (rotaciones > RANGO_MAXIMO_ENCODER) {  // (rotaciones >= RANGO_MAXIMO_ENCODER)
      encoder.setPosition(RANGO_MAXIMO_ENCODER);
      rotaciones = RANGO_MAXIMO_ENCODER;
    }

    if (pos != rotaciones) {
      pos = rotaciones;
      mostrar_menuRutas(rotaciones);
      Serial.print("Ruta -> ");
      Serial.println(rotaciones);
      //mostrar_posicion_encoder(rotaciones);
    }

  }  // rutas
}


/*!
  @brief   Se lee continuamente en el loop.
  @info:   Con el cambiamos del menu servo a menu ruta.
  @return  No devuelve nada
*/

void botonRutero() {
  bool estado_botonruta = digitalRead(pinBotonRutero);


  if ((estado_botonruta_old == HIGH) && (estado_botonruta == LOW)) {

    rutero = !rutero;
    if (rutero == true) {                                /// ESTAMOS CON LAS RUTAS.
      RANGO_MAXIMO_ENCODER = RANGO_MAXIMO_EN_RUTAS - 1;  //  maximo valor array rutas
      encoder.setPosition(RANGO_MINIMO_ENCODER);
      encoder.tick();
      Serial.print("Ruta -> ");
      Serial.println(rutero);
      delay(1000);
      lcd.clear();
      mostrar_menuRutas(0);
    } else {
      RANGO_MAXIMO_ENCODER = NUMERO_SERVOS - 1;
      encoder.setPosition(RANGO_MINIMO_ENCODER);
      encoder.tick();
      Serial.print("Aguja -> ");
      Serial.println(rutero);
      delay(1000);
      lcd.clear();
      mostrar_posicion_encoder(rotaciones);
      // mostrar_valor_posicion(Guarda_aguja[rotaciones].struct_estado);
    }
  }
  estado_botonruta_old = estado_botonruta;
}



/*!
  @brief   Se lee continuamente en el loop.
  @info:   Dependiendo del menu o pone los servo a 1 o va la ruta establecida.
  @return  No devuelve nada
*/

void botonEncoder() {
  bool estado_btn = digitalRead(pinBotonEncoder);
  if (estado_btn_old == HIGH && estado_btn == LOW) {
    if (rutero == false) {                 // son los servos.  Se pone a uno para identificar
      MiniServo[rotaciones].contador = 1;  //que hay que moverlo.
      if (MiniServo[rotaciones].solidario != 99) {
        byte amigo = MiniServo[rotaciones].solidario;
        MiniServo[amigo].contador = 1;
      }
    } else {
      run_option(rotaciones);
    }
  }

  estado_btn_old = estado_btn;
}


/*!
  @brief   Se ejecuta cada inicio.
  @info:   Asi nos aseguramos de saber que esta cada aguja en su sitio
  @return  No devuelve nada
*/

void colocacion() {
  int situoIni;
  int situoFin;
  byte placa;
  byte pin_placa;

  for (int cuentaservo = 0; cuentaservo < NUMERO_SERVOS; cuentaservo++) {
    Serial.print(F("Numero de servo -> "));
    Serial.println(cuentaservo);
    situoIni = MiniServo[cuentaservo].angulo_inicio;
    situoFin = MiniServo[cuentaservo].angulo_final;
    int calculo = situoFin - situoIni / 2;
    placa = MiniServo[cuentaservo].placa;
    pin_placa = MiniServo[cuentaservo].pin_placa_servo;
    switch (placa) {
      case 1:
        for (int vamos = situoIni + calculo; vamos < situoFin; vamos = vamos + 3) {
          placa_uno.writeMicroseconds(pin_placa, vamos);
          delay(10);
        }
        for (int vamos = situoFin; vamos > situoIni; vamos = vamos - 3) {
          placa_uno.writeMicroseconds(pin_placa, vamos);
          lcd.setCursor(3, 2);
          lcd.print("vamos por el ");
          lcd.print(cuentaservo);
          delay(10);
        }
        break;
      case 2:

        for (int vamos = situoIni + calculo; vamos < situoFin; vamos = vamos + 3) {
          placa_dos.writeMicroseconds(pin_placa, vamos);
          delay(10);
        }
        for (int vamos = situoFin; vamos > situoIni; vamos = vamos - 3) {
          placa_dos.writeMicroseconds(pin_placa, vamos);
          lcd.setCursor(3, 2);
          lcd.print("vamos por el ");
          lcd.print(cuentaservo);
          delay(10);
        }
        break;
    }
  }
}

void _informeRuta(byte quien) {
  int valores;

  valores = digitalRead(13);
  if (valores == true) {
    if (rutero == true) {
      lcd.setCursor(4, 3);
      lcd.print("MOVIENDO..");
      lcd.print(quien);
    }
  } else {
    if (rutero == true) {
      lcd.setCursor(4, 3);
      lcd.print("             ");
    }
  }
}

/*!
  @brief   Es llamada por situoservos().
  @param numero_placa:  Nos indica la placa pca9685 que debemos atacar.
  @param indice_servo:  Nos indica el servo a mover.
  @return  No devuelve nada
*/
void atacaservo(byte numero_placa, byte indice_servo) {

  //////////////////////////////////////////////////////
  //            posicion de recto
  //////////////////////////////////////////////////////

  byte pasos_movemos = 4;    // no son angulos,
  byte tiempo_retardo = 18;  // delay para cada paso del for 85
  byte rojos;
  byte verdes;
  int situoIni;
  int situoFin;
  byte pinto = 0;
  if (MiniServo[indice_servo].dondeestoy == posicion_recto) {  // estamos en recto
    byte quien = MiniServo[indice_servo].pin_placa_servo;
    rojos = MiniServo[indice_servo].orden_tira_rojo;
    verdes = MiniServo[indice_servo].orden_tira_verde;
    switch (numero_placa) {
      case 1:
        situoIni = MiniServo[indice_servo].angulo_inicio;
        situoFin = MiniServo[indice_servo].angulo_final;
        for (int vamos = situoIni; vamos <= situoFin; vamos = vamos + pasos_movemos) {
          //Serial.println(pinto);
          //pinto++;
          placa_uno.writeMicroseconds(quien, vamos);
          digitalWrite(13, HIGH);
          _informeRuta(quien);
          delay(tiempo_retardo);
        }
        digitalWrite(13, LOW);
        _informeRuta(88);
        MiniServo[indice_servo].dondeestoy = posicion_curva;
        MiniServo[indice_servo].contador = 0;
        break;
      case 2:
        situoIni = MiniServo[indice_servo].angulo_inicio;
        situoFin = MiniServo[indice_servo].angulo_final;
        for (int vamos = situoIni; vamos <= situoFin; vamos = vamos + pasos_movemos) {
          // Serial.println(pinto);
          // pinto++;
          placa_dos.writeMicroseconds(quien, vamos);
          digitalWrite(13, HIGH);
          _informeRuta(quien);
          delay(tiempo_retardo);
        }
        digitalWrite(13, LOW);
        _informeRuta(88);
        MiniServo[indice_servo].dondeestoy = posicion_curva;
        MiniServo[indice_servo].contador = 0;
        break;
    }
    MisLuces.setPixelColor(verdes, MisLuces.Color(r, g, b));
    MisLuces.setPixelColor(rojos, MisLuces.Color(r1, g1, b1));
    MisLuces.show();
    if (rutero == false) {
      lcd.setCursor(13, 2);
      if (MiniServo[rotaciones].dondeestoy == false) {
        lcd.print("Recto");
      } else {
        lcd.print("Curvo");
      }
    }
    return;
  }

  //////////////////////////////////////////////////////
  //            posicion de curvo
  //////////////////////////////////////////////////////

  if (MiniServo[indice_servo].dondeestoy == posicion_curva) {  // estamos en curvo
    int quien = MiniServo[indice_servo].pin_placa_servo;
    pinto = 0;
    rojos = MiniServo[indice_servo].orden_tira_rojo;
    verdes = MiniServo[indice_servo].orden_tira_verde;
    switch (numero_placa) {
      case 1:
        situoIni = MiniServo[indice_servo].angulo_inicio;
        situoFin = MiniServo[indice_servo].angulo_final;
        for (int vamos = situoFin; vamos >= situoIni; vamos = vamos - pasos_movemos) {
          placa_uno.writeMicroseconds(quien, vamos);
          digitalWrite(13, HIGH);
          _informeRuta(quien);
          delay(tiempo_retardo);
        }
        digitalWrite(13, LOW);
        _informeRuta(88);
        MiniServo[indice_servo].dondeestoy = posicion_recto;
        MiniServo[indice_servo].contador = 0;
        break;
      case 2:
        situoIni = MiniServo[indice_servo].angulo_inicio;
        situoFin = MiniServo[indice_servo].angulo_final;
        for (int vamos = situoFin; vamos >= situoIni; vamos = vamos - pasos_movemos) {
          placa_dos.writeMicroseconds(quien, vamos);
          digitalWrite(13, HIGH);
          _informeRuta(quien);
          delay(tiempo_retardo);
        }
        digitalWrite(13, LOW);
        _informeRuta(88);
        MiniServo[indice_servo].dondeestoy = posicion_recto;
        MiniServo[indice_servo].contador = 0;
        break;
    }
    MisLuces.setPixelColor(rojos, MisLuces.Color(r, g, b));
    MisLuces.setPixelColor(verdes, MisLuces.Color(r1, g1, b1));
    MisLuces.show();
    if (rutero == false) {
      lcd.setCursor(13, 2);
      if (MiniServo[rotaciones].dondeestoy == false) {
        lcd.print("Recto");
      } else {
        lcd.print("Curvo");
      }
    }
  }
}




void ponerlucesinicio() {
  byte rojos;
  byte verdes;
  for (int cuentaservo = 0; cuentaservo < NUMERO_SERVOS; cuentaservo++)  //
  {
    rojos = MiniServo[cuentaservo].orden_tira_rojo;
    verdes = MiniServo[cuentaservo].orden_tira_verde;
    MisLuces.setPixelColor(rojos, MisLuces.Color(r, g, b));
    MisLuces.setPixelColor(verdes, MisLuces.Color(r1, g1, b1));
    MisLuces.show();
  }
}

void mostrar_posicion_encoder(int _posicion) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU SERVOS ");
  lcd.setCursor(0, 1);
  lcd.print("Desv");
  lcd.write(2);
  lcd.print("o    ->");
  lcd.setCursor(12, 1);
  int mostrarPosicion = _posicion;             // +1
  if (mostrarPosicion < 10) lcd.print("   ");  // ponemos un espacio delante si es de un dígito
  lcd.print(mostrarPosicion);
  lcd.setCursor(0, 2);
  lcd.print("Estado:   -> ");

  if (MiniServo[rotaciones].dondeestoy == false) {
    lcd.print("Recto");
  } else {
    lcd.print("Curvo");
  }

  lcd.setCursor(0, 3);
  if (MiniServo[rotaciones].solidario != 99) {
    lcd.print("Solidario ->   ");
    lcd.print((MiniServo[rotaciones].solidario));
  } else {
    lcd.print("                   ");
  }
}


void cartapresentacion() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AGUJETAS CON SERVOS");
  lcd.setCursor(0, 1);
  lcd.print("Ver. ");
  lcd.print(version);
  lcd.setCursor(5, 8);
  lcd.print("EMPEZAMOS");
  lcd.setCursor(5, 3);
  lcd.print(PRESENTACION);
  delay(4000);
  lcd.clear();
}

void situoservos() {
  for (int cuentaservo = 0; cuentaservo < NUMERO_SERVOS; cuentaservo++)  //
  {
    if (MiniServo[cuentaservo].contador == 1)  //
    {
      byte cuentaplaca = MiniServo[cuentaservo].placa;
      atacaservo(cuentaplaca, cuentaservo);
    }
  }
}


void setup() {
  Serial.begin(115200);
  placa_uno.begin();
  placa_uno.setPWMFreq(60);  // Analog NUMERO_SERVO run at ~60 Hz updates
  placa_uno.setOscillatorFrequency(25000000);
  placa_dos.begin();
  placa_dos.setPWMFreq(60);  // Analog NUMERO_SERVO run at ~60 Hz updates
  placa_dos.setOscillatorFrequency(25000000);

  relleno_base();
  MaestroRutas();
  pinMode(pinBotonEncoder, INPUT_PULLUP);
  pinMode(pinBotonRutero, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  MisLuces.begin();
  MisLuces.setBrightness(10);
  MisLuces.clear();
  encoder.setPosition(RANGO_MINIMO_ENCODER);
  encoder.tick();
  Serial.println(__FILE__);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.createChar(0, u2);
  lcd.createChar(1, o2);
  lcd.createChar(2, i2);
  cartapresentacion();
  _cofiguracionServos();
  //colocacion();
  ponerlucesinicio();
  digitalWrite(13, LOW);
}


void loop() {
  int valores;
  ENCODER();
  valores = digitalRead(13);
  if (valores == false) {
    botonEncoder();
    botonRutero();
  } else {
    Serial.println("paso por aqui");
    if (rutero == true) {
    }
  }
  situoservos();
}
