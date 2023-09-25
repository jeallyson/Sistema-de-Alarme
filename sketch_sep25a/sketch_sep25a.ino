#include <Keypad.h>  // Inclui a biblioteca Keypad para lidar com o teclado matricial.

// Definições para o teclado 4x4
#define linhas 4
#define colunas 4

// Pinos para os LEDs
#define led_vermelho 2
#define led_amarelo 3
#define led_verde 4

// Pino para o buzzer
int buzzer = 5;

// Mapeamento das teclas no teclado matricial
char mapaTeclas[linhas][colunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

String senha = "1234";  // Senha atual que deve ser inserida.
String digitada;        // Senha que está sendo digitada pelo usuário.
int estado = 0;         // Estado do sistema: 0 = Aguardando, 1 = Digitando, 2 = Negado, 3 = Aceito.

// Arrays para os pinos das linhas e colunas do teclado matricial.
byte pinos_linhas[linhas] = {6, 7, 8, 9};
byte pinos_colunas[colunas] = {10, 11, 12, 13};

// Inicialização do objeto Keypad
Keypad teclado = Keypad(makeKeymap(mapaTeclas), pinos_linhas, pinos_colunas, linhas, colunas);

void setup() {
  Serial.begin(9600);  // Inicializa a comunicação serial.
  Serial.println();
  
  // Configuração dos pinos como saídas
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_verde, OUTPUT);
  pinMode(led_amarelo, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  char tecla = teclado.getKey();  // Obtém a tecla pressionada no teclado.

  if (tecla != NO_KEY) {
    estado = 1;  // Define o estado como "Digitando" quando uma tecla é pressionada.

    if (tecla == '#') {
      if (senha == digitada) {
        estado = 3;  // Senha correta, define o estado como "Aceito".
        leds(estado);
        delay(3000);  // Aguarda 3 segundos.
        estado = 0;  // Volta ao estado "Aguardando" após o atraso.
      } else {
        estado = 2;  // Senha incorreta, define o estado como "Negado".
        leds(estado);
        tone(buzzer, 1000, 3000);  // Ativa o buzzer com uma frequência de 1000 Hz por 3 segundos.
        delay(3000);  // Aguarda 3 segundos.
        estado = 0;  // Volta ao estado "Aguardando" após o atraso.
      }
      digitada = "";  // Limpa a senha digitada.
    } else {
      digitada += tecla;  // Adiciona a tecla à senha que está sendo digitada.
    }
    
    leds(estado);  // Atualiza os LEDs de acordo com o estado.
    
    Serial.print("Tecla: ");
    Serial.println(tecla);
  }
}

// Função para verificar se a senha digitada corresponde à senha atual.
bool verificaSenha(String sa, String sd) {
  bool resultado = false;
  if (sa.compareTo(sd) == 0) {
    resultado = true;
  } else {
    resultado = false;
  }
  return resultado;
}

// Função para controlar os LEDs com base no estado.
void leds(int e) {
  if (e == 0) {  // Estado "Aguardando (0)", todos os LEDs desligados.
    digitalWrite(led_vermelho, LOW);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, LOW);
  } else if (e == 1) {  // Estado "Digitando (1)", luz amarela ligada.
    digitalWrite(led_vermelho, LOW);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, HIGH);
  } else if (e == 2) {  // Estado "Negado (2)", luz vermelha ligada.
    digitalWrite(led_vermelho, HIGH);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, LOW);
  } else if (e == 3) {  // Estado "Aceito (3)", luz verde ligada.
    digitalWrite(led_vermelho, LOW);
    digitalWrite(led_verde, HIGH);
    digitalWrite(led_amarelo, LOW);
  }
}
