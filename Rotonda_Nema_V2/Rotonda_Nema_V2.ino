
/*

    
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
░        ░░░░░░░░░     ░░░░░░           ░░░░░     ░░░░░░    ░░░░░   ░      ░░░░░   ░   ░░░░░░░░   ░░░░░░░░░░░░░░  ░░░░░░░
▒   ▒▒▒▒   ▒▒▒▒▒   ▒▒▒▒   ▒▒▒▒▒▒▒   ▒▒▒▒▒▒▒   ▒▒▒▒   ▒▒▒  ▒   ▒▒▒   ▒   ▒▒▒   ▒▒   ▒   ▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒▒▒▒▒▒  ▒  ▒▒▒▒▒
▒   ▒▒▒▒   ▒▒▒   ▒▒▒▒▒▒▒▒   ▒▒▒▒▒   ▒▒▒▒▒   ▒▒▒▒▒▒▒▒   ▒   ▒   ▒▒   ▒   ▒▒▒▒   ▒   ▒   ▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒▒▒▒▒  ▒▒   ▒▒▒▒
▓  ▓   ▓▓▓▓▓▓▓   ▓▓▓▓▓▓▓▓   ▓▓▓▓▓   ▓▓▓▓▓   ▓▓▓▓▓▓▓▓   ▓   ▓▓   ▓   ▓   ▓▓▓▓   ▓   ▓   ▓▓▓▓▓▓▓▓   ▓▓▓▓▓▓▓▓▓▓▓   ▓▓▓   ▓▓▓
▓   ▓▓   ▓▓▓▓▓   ▓▓▓▓▓▓▓▓   ▓▓▓▓▓   ▓▓▓▓▓   ▓▓▓▓▓▓▓▓   ▓   ▓▓▓  ▓   ▓   ▓▓▓▓   ▓   ▓   ▓▓▓▓▓▓▓▓   ▓▓▓▓▓▓▓▓▓▓       ▓   ▓▓
▓   ▓▓▓▓   ▓▓▓▓▓   ▓▓▓▓▓   ▓▓▓▓▓▓   ▓▓▓▓▓▓▓   ▓▓▓▓▓   ▓▓   ▓▓▓▓  ▓  ▓   ▓▓▓   ▓▓   ▓   ▓▓▓▓▓▓▓▓   ▓▓▓▓▓▓▓▓▓   ▓▓▓▓▓▓▓   ▓
█   ██████   █████     ██████████   █████████     ██████   ██████   █      █████   █          █          █   █████████   
█████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████



fecha último parche: 06/11/2023. 
*/

#include <RotaryEncoder.h>  // https://github.com/mathertel/RotaryEncoder
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "personal.h"





// pines arduino usados
// 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
// x x x x x x           xx xx
// ----- - - -           -- --
//   │   │ │ │            │  │
//   │   │ │ │            │  └ LUZ BOTON APRETADO, ES LA INTREGADA
//   │   │ │ │            └ DETECTOR PARA PARAR EN UN PUNTO
//   │   │ │ └ SENTIDO DEL MOTOR
//   │   │ └ PULSADOR PARA CAMBIO DE VUELTAS
//   │   └  PASOS MOTOR, PUNTOS DONDE IRA
//   └ ENCODER, CLK AL 2, DT AL 3 Y BOTON AL 4




// encoder
const byte ENCODER_DT = 2;
const byte ENCODER_CLK = 3;
const byte ENCODER_BOTON = 4;

byte estado_btn_old = 0;
byte rotaciones = 0;  // las vueltas que vamos dando

const byte RANGO_MINIMO_ENCODER = 0;                // Valor mínimo del encoder
const byte RANGO_MAXIMO_ENCODER = NUMERO_VIAS - 1;  // Valor máximo del encoder

// boton para saber si dejamos o recogemos
const byte PULSADOR = 6;
byte estado_boton_old;
const byte LUZ_PULSADOR = 13;

const int VUELTACOMPLETA = 3200;

// sensor

const byte SensorHerradura = 12;  //  sensor para saber el punto 0
const byte statusLED = 13;        //  este led se enciende cuando el sensor es detectado
byte detectState = 0;             //



// los objetos

// pantalla.
LiquidCrystal_I2C lcd(direccion_lcd4li, 16, 2);
//encoder
RotaryEncoder encoder(ENCODER_CLK, ENCODER_DT, RotaryEncoder::LatchMode::FOUR3);



// variables de la placa
int misituacionactual = 0;
bool sentido = true;  // determina el sentido del giro

// funcion encoder.
void checkPosition() {
  encoder.tick();
}

byte pos = 1;
bool giramos = false;

/*!
  @brief   detecta el encoder por la libreria.
  @return  numero de vueltas del encoder.
*/

int ENCODER()  //
{
  encoder.tick();
  rotaciones = encoder.getPosition();

  if (rotaciones < RANGO_MINIMO_ENCODER)  //
  {
    encoder.setPosition(RANGO_MINIMO_ENCODER);
    rotaciones = RANGO_MINIMO_ENCODER;
  } else if (rotaciones > RANGO_MAXIMO_ENCODER) {  // (rotaciones >= RANGO_MAXIMO_ENCODER)
    encoder.setPosition(RANGO_MAXIMO_ENCODER);
    rotaciones = RANGO_MAXIMO_ENCODER;
  }

  if (pos != rotaciones)  //
  {
    pos = rotaciones;
#if (DEBUG == 2)
    Serial.print(rotaciones);
    Serial.print(F(" ..... "));
    Serial.print(mirotonda[rotaciones].titulo);
    Serial.print("  ");
    Serial.println(mirotonda[rotaciones].desplazamiento);
#endif
    aviso_situacion();

#if (DEBUG == 8)
    int me_situo = mirotonda[pos].desplazamiento - misituacionactual;
    int me_situo_reves = ((misituacionactual + VUELTACOMPLETA) + (-mirotonda[pos].desplazamiento));
    bool hacia_donde_vamos = (me_situo) < (me_situo_reves);
    Serial.print("diferencia -> ");
    Serial.print(me_situo);
    Serial.print("\t");
    Serial.print(me_situo_reves);
    Serial.print("\t");
    if (hacia_donde_vamos == true) {
      Serial.println("sentido normal");
    } else {
      Serial.println("sentido inverso");
    }

#endif
  }
}


/*!
  @brief   Lee continuamente el boton. Al puslar desencadena las acciones asignadas.
  @return  Sin devolucion de nada, informa por pantalla y monitor serie.
*/

void botonEncoder()  //
{
  bool estado_btn = digitalRead(ENCODER_BOTON);

  if (estado_btn_old == HIGH && estado_btn == LOW)  //
  {
    run_option(rotaciones);
  }
  estado_btn_old = estado_btn;
}



/*!
  @brief   Enviamos a otra funcion segun el numero de rotacion.
  @param   el numero de la rotacion
  @return  Sin datos de vuelta .
*/

void run_option(int rotacion)  //
{
  int a_donde_vamos;
  int numero_de_pasos;

  // si tenemos activo el pulsador, giramos media vuelta más
  if (giramos == false)  //
  {
    a_donde_vamos = mirotonda[rotacion].desplazamiento;
  } else {
    a_donde_vamos = mirotonda[rotacion].desplazamiento + VUELTACOMPLETA / 2;
  }

  if (misituacionactual < a_donde_vamos)  //
  {
    numero_de_pasos = a_donde_vamos - misituacionactual;
#if (DEBUG == 2)
    Serial.println("");
    Serial.print(F("numero de pasos que vamos a dar para llegar + "));
    Serial.println(numero_de_pasos);
#endif
    sentido = true;
    aviso_movimiento(numero_de_pasos);
    muevo_placa(numero_de_pasos);

  } else  //  vamos a ir hacia atras,
  {

    numero_de_pasos = a_donde_vamos - misituacionactual;
#if (DEBUG == 2)
    Serial.println("");
    Serial.print(F("numero de pasos que vamos a restar para llegar - "));
    Serial.println(numero_de_pasos);
#endif
    sentido = false;
    aviso_movimiento(numero_de_pasos);  // lo dejo en negativo para ver que vamos a la inversa
    muevo_placa(abs(numero_de_pasos));  // el calculo sale negativo y por observacion de felix se puso el abs
  }
  delay(200);
  misituacionactual = mirotonda[rotacion].desplazamiento;  //donden estamos;
#if (DEBUG == 2)
  Serial.print(F("mi situacion "));
  Serial.println(misituacionactual);
#endif
}

/*!
  @brief   movemos el motor. Hay una variable (sentido)que determina el sentido.
  @param   el numero puntos que se mueve.
  @return  Sin datos de vuelta .
*/
void muevo_placa(int numero_de_pasos)  //
{
  bool estado_led = digitalRead(statusLED);
  digitalWrite(11, LOW);
  digitalWrite(SENTIDO_MOTOR_GRANDE, sentido);
  for (int i = 0; i <= numero_de_pasos; i++) {
    digitalWrite(statusLED, HIGH);
    digitalWrite(PASOS_MOTOR_GRANDE, HIGH);
    VELOCIDAD(REVOLUCIONES);
    digitalWrite(PASOS_MOTOR_GRANDE, LOW);
    digitalWrite(statusLED, LOW);
    VELOCIDAD(REVOLUCIONES);
  }
  delay(2000);
  digitalWrite(statusLED, estado_led);
  aviso_situacion();
  digitalWrite(11, HIGH);
}

/*
  @brief   Movemos el motor hasta que lo para el sensor.
  @return  no devuelve nada.
*/

void SituarPlacaInicio()  //
{
  digitalWrite(SENTIDO_MOTOR_GRANDE, HIGH);  // giro en un sentido
  for (int i = 0; i < 840; i++) {            // 200 pasos para motor de 0.9 grados de angulo de paso
    detectState = digitalRead(SensorHerradura);
    if (detectState == LOW) {         //If encoder output is high
      digitalWrite(statusLED, HIGH);  //Turn on the status LED
      Serial.println("detectado  ");
      delay(500);
      digitalWrite(statusLED, LOW);
      break;
    }
    digitalWrite(PASOS_MOTOR_GRANDE, HIGH);  // nivel alto
    VELOCIDAD(REVOLUCIONES);                 // por 10 mseg
    digitalWrite(PASOS_MOTOR_GRANDE, LOW);   // nivel bajo
    VELOCIDAD(REVOLUCIONES);                 // por 10 mseg
  }
  delay(2000);
}


/*
  @brief   Sacamos por el monitor serie los datos de los puntos.
  @return  no devuelve nada.
*/
void imprime()  //
{
#if (DEBUG == 2)
  Serial.println("orden      titulo        pasos   ");
  Serial.println("");

  for (int cuenta = 0; cuenta < NUMERO_VIAS; cuenta++) {
    Serial.print(cuenta);
    Serial.print('\t');
    Serial.print(mirotonda[cuenta].titulo);
    Serial.print('\t');
    Serial.println(mirotonda[cuenta].desplazamiento);
  }
  Serial.println(F(".............................."));
  Serial.println("");
#endif
}

/*
  @brief   Sacamos por lcd algunos datos.
  @return  no devuelve nada.
*/

void aviso_situacion() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mirotonda[rotaciones].titulo);
  lcd.setCursor(0, 1);
  lcd.print("Giro: ");
  lcd.print(mirotonda[rotaciones].desplazamiento);
  if (giramos == true) {
    lcd.setCursor(11, 1);
    lcd.print("---");
    //lcd.setCursor(13, 1);
    lcd.write(0);
  } else {
    lcd.setCursor(11, 1);
    lcd.write(1);
    lcd.print("---");
  }
}
/*
  @brief   Sacamos por el lcd lo que esta pasando.
  @return  no devuelve nada.
*/
void aviso_movimiento(int numero_de_pasos)  //
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Movemos:  ");
  lcd.print(numero_de_pasos);
  lcd.setCursor(0, 1);
  lcd.print("colocando....");
}


void setup()  //
{
  pinMode(ENCODER_CLK, INPUT_PULLUP);    // pin del enoder
  pinMode(ENCODER_DT, INPUT_PULLUP);     // pin del encoder
  pinMode(ENCODER_BOTON, INPUT_PULLUP);  // boton del encoder
  pinMode(PULSADOR, INPUT_PULLUP);       // poner input, solo
  pinMode(LUZ_PULSADOR, OUTPUT);
  pinMode(SensorHerradura, INPUT);  //Set pin 12 as input
  pinMode(statusLED, OUTPUT);       //Set pin 13 as output
  digitalWrite(statusLED, LOW);
  digitalWrite(LUZ_PULSADOR, LOW);
  pinMode(PASOS_MOTOR_GRANDE, OUTPUT);    // pin 4 como salida
  pinMode(SENTIDO_MOTOR_GRANDE, OUTPUT);  // pin 5 como salida
  agrego();                               // funcion para meter los puntos
#if (DEBUG == 2)
  Serial.begin(115200);
#endif
  Serial.println(__FILE__);
  estado_boton_old = digitalRead(PULSADOR);
  SituarPlacaInicio();
  imprime();
  lcd.init();
  lcd.createChar(0, body);
  lcd.createChar(1, cabezas);
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("ROTONDILLA");
  lcd.setCursor(3, 1);
  lcd.print("  FELIX  ");
  delay(2000);
}




void loop()  //
{

  // DE AQUI PARA ABAJO ES EL LOOP QUE DEBE FUNCIONAR.
  byte state_btn = digitalRead(PULSADOR);

  if (estado_boton_old == HIGH && state_btn == LOW) {
    Serial.print(F("cambiamos "));
    giramos = !giramos;
    delay(100);
    Serial.println("");
    if (giramos == true) {
      Serial.println(F("Pies "));
      digitalWrite(LUZ_PULSADOR, HIGH);
    } else {
      Serial.println(F("Cabeza "));
      digitalWrite(LUZ_PULSADOR, LOW);
    }
    aviso_situacion();
  }
  estado_boton_old = state_btn;

  ENCODER();
  botonEncoder();
}
