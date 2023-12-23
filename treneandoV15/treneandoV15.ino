/*



   _                                                         _
  | |_      _ _    ___    _ _      ___    __ _    _ _     __| |    ___
  |  _|    | '_|  / -_)  | ' \    / -_)  / _` |  | ' \   / _` |   / _ \
  _\__|   _|_|_   \___|  |_||_|   \___|  \__,_|  |_||_|  \__,_|   \___/
  _|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|
  "`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'


   programa para gestionar la placa L298N y sus dos motores.
   la idea es mover dos trenes a diferente velocidad usando
   los encoder rotativos.
   los botones que lleva los encoder se usan como freno de emergencia
   al ir girando en sentido horario se aumenta la velocidad.
   al girar antihorario se baja la velocidad.
   como hay motores que arrancaran a unos voltios y en cambio  no se
   conseguiremos que paren a esos mismos de inicio. habra que estudiar una
   variable para conseguirlo, eso con pruebas sobre el terreno.
   Se ha dotado de 2 velocimetros ( servos sg90)
   Se ha dotado de 2 semaforos para ir viendo tambien la velocidad.
   Hay dos botones para el cambio de sentido.
   Se estudia tambien, añadir un boton para que en lugar de usar los dos encoder
   se pueda usar solo uno, y volver a los dos encoder cuando se vuelva a pulsar.
   Version 2.3 de 10-04-2022. Probada sin semaforo. y todo estable y funcionando.
   Version 2.4 de 14-04-2022. El boton del encoder si se pulsa una vez cambia de sentido
   si se pulsa prorrongandamente hace un stop.
   Version 2.5 algunos cambios en el manejo de los trenes cuando van en un solo encoder
   Version 2.6 vamos a poner los pines juntos.
   Version 2.7 se implemtenta que al girar el deco mas de 4 veces seguidas se multipla por 4
   a los tres segundos vuelve a quedarse en su estado original.
   en esta ocasion he corregido los pines, que los tenia cambiados.
   fecha de la ultima modificacion 19/04/2022 16:27
   hay que mirar los colores de los semaforos
   se crea una nueva funcion para los semaforos, asi es llamada por cada instancia, y no solo
   cuando se mira la velocidad. 30/04/2022 a las 17:01
    la ultima locura es que me funcione con movil. casi con las mismas prestaciones. a ver que pasa.
    dia 13 de 06, queremos poner un menu para variar la velocidad de inicio de marcha.
    creo un boton en el pin 48 y con el segundo encoder trato de hacerlo.
    dia 16/06 anulamos lo de dar mas velocidad girando varias veces el encoder.
    dia 18/06 voy a intentar salir del menu si no me interesa.

*/
// #define FELIX
//#define GONZALO
//#define JUANCARLOS
#define MIGUEL
#define PANTALLA
#include <MiLibreria.h>
#include <Servo.h>
#include <Wire.h>  // libreria de comunicacion por I2C
#include <LiquidCrystal_I2C.h>  // libreria para LCD por I2C
#include <SoftwareSerial.h>


// motor A
#define L298_ENA                    9  // MOTOR-DRIVER L298
#define L298_IN1                    7
#define L298_IN2                    8
// motor B
#define L298_ENB                    6  // MOTOR-DRIVER L298
#define L298_IN3                    10
#define L298_IN4                    11
// encoder 1
#define clk                         2
#define dt                          3
#define btn                         4  // boton decoder para stop
// encoder 2
#define clk2                        12
#define dt2                         13
#define btn2                        14  // boton decoder para stop

#define btjuntitos                  23  // boton para unir los dos motores.
#define bt_menu                     48


// led de colores pines
byte pin_verde[] =                  { 18, 25 };
byte pin_ambar[] =                  { 17, 26 };
byte pin_rojo[] =                   { 16, 27 };
byte pinUnion =                     29;  // intentamos saber si las dos salidas estan en el mismo par.

int iniciomarcha =                  30;  // para ver cuando el motor se mueve, por quitar vueltas al encoder
int estado_clk_old[] =              { 0, 0 };
int estado_btn_old[] =              { 1, 1 };
int state_clk_old =                 0;
int state_dt_old =                  0;
int estado_clk_aux =                0;
int estado_btjuntitos_old =         1;
int estado_bt_menu_old =            1;
int count[] =                       {0, 0}; //{ iniciomarcha, iniciomarcha };
int aprieto =                       0;  // para botones del encoder
int reloj =                         0;
int reloj1 =                        0;
int velocidad_old  =                0;//iniciomarcha;
int velocidad_old1 =                0;//iniciomarcha;
int vueltas_para_cambio =           0;
int inercia =                       10;  // variable para los saltos que va dando el decoder, esto estaba en 10 en las pruebas. poner segun necesitad,
int estadoLed;
int tiempo_retardo_velocidad =      150;
// se estudiara poner algo para variar estos saltos.
int contadorqueveo =                0;
String valores_posibles[] =         {"10", "20", "30 ", "40", "50", "60", "MEDIA-1", "MEDIA-2", "atras" };
int opciones =                      sizeof(valores_posibles) / sizeof(valores_posibles[0]);
int rotaciones =                    0;
int state_clk  =                    0;
int state_dt  =                     0;

// blue
char val;
String palabra;
String dato1;
String dato2;

int k1;
int k2;
int dato1n =                        0;
int dato2n =                        0;
bool DatosRecibidos =               false;
bool dato1_cargado =                false;
bool dato2_cargado =                false;

// blue fin
unsigned long demorarulo;
unsigned long demora;
unsigned long demora_enciendo  ;
unsigned long tiempo_espera;
long oldPosition  =                  -999;
bool sentido =                      true;   // motor a giro
bool sentido2 =                     true;  // motor b giro
bool emergencia =                   false;
bool emergencia1 =                  false;
bool juntitos =                     false;
bool sentidojuntitos =              false;  // ambos motores en el mismo sentido
bool grabando =                     true;
bool dato1_velocidad =              false;
bool dato2_velocidad =              false;
// creamos los "objetos"
miLedes miSemaforo(pin_rojo[0], pin_ambar[0], pin_verde[0]);   //18,17,16
miLedes miSemaforo1(pin_rojo[1], pin_ambar[1], pin_verde[1]);  //25,26,27

char TextoLinea1[16] =              "  ";
char TextoLinea2[16] =              "  ";
String presentacion =               "  ";
byte body[8] = {
  0b00001110,
  0b00001110,
  0b00000100,
  0b00011111,
  0b00000100,
  0b00001010,
  0b00010001,
  0b00000000,
};
SoftwareSerial miBt(50, 52);
#ifdef FELIX
LiquidCrystal_I2C lcd(0x27, 16, 2);
#endif

#ifdef JUANCARLOS
LiquidCrystal_I2C lcd(0x27, 16, 2);
#endif

#ifdef GONZALO
LiquidCrystal_I2C lcd(0x2F, 16, 2); //
#endif

#ifdef MIGUEL
LiquidCrystal_I2C lcd(0x3F, 16, 2);
#endif

Servo Velocimetro;
Servo Velocimetro1;

void setup() {
  voidsetup();
  voidvelocimetros();
  miBt.begin(9600);
  Serial.begin(9600);
#ifdef FELIX
  lcd.init();
  lcd.backlight();
  presentacion = "FELIX";
#else
  lcd.begin();
#endif
#ifdef MIGUEL
  //  lcd.init();
  // lcd.backlight();
  lcd.begin();
  presentacion = "MIGUEL";
#endif
  lcd.clear();
  state_clk_old = digitalRead(clk);
  state_dt_old = digitalRead(dt);
  mostrar_menu_inicio();
  mostrar_menu_velocidad();
  lcd.createChar (0, body);
}


void loop() {
  serialbluetoh();
  leer_botones();
  leer_encoder();
  saber_polaridad();
  if (grabando == false)
  {
    encoder_configurar();  // pestaña cambios
  }
}
