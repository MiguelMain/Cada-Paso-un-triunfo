/*
                        ##           ######     ########
                        ##          ##    ##    ##     ##
                        ##          ##          ##     ##
                        ##          ##          ##     ##
                        ##          ##          ##     ##
                        ##          ##    ##    ##     ##
                        ########     ######     ########
*/
void mostrar_menu()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu: Treneando");
  lcd.setCursor(0, 1);
  lcd.print(valores_posibles[rotaciones]);
}

void mostrar_menu_cambio()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu: Treneando");
  lcd.setCursor(0, 1);
  lcd.print("Cambiado a :");
  lcd.print(iniciomarcha);
  delay(1550);
  lcd.clear();
}

void mostrar_menu_velocidad()
{
  int relojes = 0;
  int relojes2 = 0;
  relojes = map(velocidad_old, 0, 254, 0, 100);
  relojes2 = map(velocidad_old1, 0, 254, 0, 100);
  //lcd.noDisplay();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mando 1: ");
  lcd.setCursor(9, 0);
  lcd.print(relojes);
  lcd.setCursor(12, 0);
  lcd.print("%");
  if (digitalRead(L298_IN1) == HIGH)
  {
    lcd.setCursor(14, 0);
    lcd.print("->");
  }
  if (digitalRead(L298_IN2) == HIGH)
  {
    lcd.setCursor(14, 0);
    lcd.print("<-");
  }
  lcd.setCursor(0, 1);
  lcd.print("Mando 2: ");
  lcd.setCursor(9, 1);
  lcd.print(relojes2);
  lcd.setCursor(12, 1);
  lcd.print("%");

  if (digitalRead(L298_IN3) == HIGH)
  {
    lcd.setCursor(14, 1);
    lcd.print("->");
  }

  if (digitalRead(L298_IN4) == HIGH)
  {
    lcd.setCursor(14, 1);
    lcd.print("<-");
  }
  delay(20);
  lcd.noCursor();
}

void mostrar_menu_inicio()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu: Treneando");
  lcd.setCursor(0, 1);
  lcd.print("(c) " + presentacion + " ");
  lcd.write((byte)0);
  delay(2000);
  lcd.clear();
}
