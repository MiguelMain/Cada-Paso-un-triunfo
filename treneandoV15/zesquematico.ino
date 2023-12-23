/*
                                      +-----+
         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |           GND/RST2  [ ] [ ]         |
         |         MOSI2/SCK2  [ ] [ ]  SCL[ ] |    
         |            5V/MISO2 [ ] [ ]  SDA[ ] |    
         |                             AREF[ ] |
         |                              GND[ ] |
         | [ ]N/C                        13[ ]~|  DT2    
         | [ ]IOREF                      12[ ]~|  CLK2      
         | [ ]RST                        11[ ]~|  EN4 
         | [ ]3V3      +----------+      10[ ]~|  EN3     
         | [ ]5v       | ARDUINO  |       9[ ]~|  ENA   
         | [ ]GND      |   MEGA   |       8[ ]~|  EN2   
         | [ ]GND      +----------+            |
         | [ ]Vin                         7[ ]~|  EN1      
         |                                6[ ]~|  ENB   
         | [ ]A0                          5[ ]~|      
         | [ ]A1                          4[ ]~|  BTN   *
         | [ ]A2                     INT5/3[ ]~|  DT    *
         | [ ]A3                     INT4/2[ ]~|  CLK   *
         | [ ]A4                       TX>1[ ]~|    
         | [ ]A5                       RX<0[ ]~|    
         | [ ]A6                               |   
         | [ ]A7                     TX3/14[ ] |  BTN2   
         |                           RX3/15[ ] |      
         | [ ]A8                     TX2/16[ ] |  led rojo             
         | [ ]A9                     RX2/17[ ] |  led ambar   
         | [ ]A10               TX1/INT3/18[ ] |  led verde             
         | [ ]A11               RX1/INT2/19[ ] |      
         | [ ]A12           I2C-SDA/INT1/20[ ] |  sda           
         | [ ]A13           I2C-SCL/INT0/21[ ] |  scl  
         | [ ]A14                              |            
         | [ ]A15                              |  
         |                RST SCK MISO         |    22 velocimetro      23  juntitos    
         |         ICSP   [ ] [ ] [ ]          |    24 velocimetro1     25  led verde  
         |                [ ] [ ] [ ]          |    26 led ambar        27  led rojo    
         |                GND MOSI 5V          |    28                  29  led polaridad  
         | G                                   |    30                  31    
         | N 5 5 4 4 4 4 4 3 3 3 3 3 2 2 2 2 5 |    32                  33               I2C-SDA/INT1/20
         | D 2 0 8 6 4 2 0 8 6 4 2 0 8 6 4 2 V |    34                  35               I2C-SCL/INT0/21
         |         ~ ~                         |    36                  37     
         | @ # # # # # # # # # # # # # # # # @ |    38                  39     
         | @ # # # # # # # # # # # # # # # # @ |    40                  41    
         |           ~                         |    42                  43    
         | G 5 5 4 4 4 4 4 3 3 3 3 3 2 2 2 2 5 |    44                  45    
         | N 3 1 9 7 5 3 1 9 7 5 3 1 9 7 5 3 V |    46                  47    
         | D                                   |    48 boton menu       49     SPI:
         |                                     |    50 rx               51      50=MISO 51=MOSI
         |     2560                ____________/    52 tx               53      52=SCK  53=SS 
          \_______________________/         
  */
  
