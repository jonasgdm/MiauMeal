#include <Servo.h>

Servo myservo;

const int servoPin = 9;  // Pino do Servo (PB1)
const int buttonPin = 12;  // Pino do Botão (PB4)
const int ledPin = 13;  // Pino do LED (PB5)

void setup() {
  // Configuração inicial do servo
  myservo.attach(servoPin);
  
  // Configuração dos pinos usando Assembly
  asm volatile (
    "cbi %[portb], %[buttonPin] \n" // Configura botão como entrada
    "sbi %[ddrb], %[ledPin] \n"     // Configura LED como saída
    "cbi %[portb], %[ledPin] \n"    // Desliga o LED
    :                               // Sem saídas
    : [ddrb] "I" (_SFR_IO_ADDR(DDRB)),
      [portb] "I" (_SFR_IO_ADDR(PORTB)),
      [buttonPin] "I" (4),          // Pino 12 como entrada
      [ledPin] "I" (5)              // Pino 13 como saída
  );
  
  // Move o servo para 180 graus
  myservo.write(180);
  delay(1000);
  myservo.detach();
}

void loop() {
  int buttonVal;

  // Leitura do valor do botão em Assembly
  asm volatile (
    "sbic %[pinb], %[buttonPin] \n"  // Testa se o botão está pressionado
    "ldi %[result], 0 \n"           // Define buttonVal como 0 se pressionado
    "sbis %[pinb], %[buttonPin] \n"  // Testa se o botão não está pressionado
    "ldi %[result], 1 \n"           // Define buttonVal como 1 se não pressionado
    : [result] "=r" (buttonVal)      // Saída
    : [pinb] "I" (_SFR_IO_ADDR(PINB)),
      [buttonPin] "I" (4)            // Pino 12
  );

  // Controle do servo com base no valor do botão
  if (buttonVal == 0) {
    myservo.attach(servoPin);
    myservo.write(30);   // Move o servo para 30 graus
    delay(575);          // Delay de 575ms
    myservo.write(180);  // Move o servo para 180 graus
    delay(1500);         // Delay de 1.5s
    myservo.detach();    // Desanexa o servo
    delay(5000);         // Delay de 5s
  }

  // Delay de 13ms em Assembly
  asm volatile (
    "ldi r18, 209 \n"    // Aproximadamente 13ms de delay
    "1: dec r18 \n"
    "brne 1b \n"
  );
}