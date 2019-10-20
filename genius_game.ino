/*
  DEFINE NOTAS MUSICAIS
*/
#define NOTE_D4  294

#define NOTE_G4  392

#define NOTE_A4  440

#define NOTE_A5  880

//ARRAY PARA ARMAZENAR AS NOTAS
int tons[4] = { NOTE_A5, NOTE_A4, NOTE_G4, NOTE_D4 };

//ARRAY SEQUENCIA DE LUZES
int sequence[100] = {};

//VARIAVEL PARA A RODADA ATUAL
int current_round = 0;

//PINOS LED
int pin_leds[4] = { 8, 9, 10, 11 };

//PINOS BOTOES
int pin_buttons[4] = { 2, 3, 4, 5 };

//PINO BUZZER
int pin_audio[1] = {12};

//VARIAVEL PARA VERIFICAR SE BOTAO FOI PRESSIONADO
int button_pressed = 0;

//INDICA SE JOGADOR PERDEU
bool game_over = false;

//INDICA QUAL O PASSO ATUAL
int current_step = 0;


void setup(){

  //INICIA CONSOLE
  Serial.begin(9600);

  //DEFINE PINOS DOS LEDS COMO SAIDA
  definePins("out", pin_leds);

  //DEFINE PINOS DOS BOTOES COMO ENTRADA
  definePins("inp", pin_buttons);

  //DEFINE O PINO DO BUZER
  definePins("OUT", pin_audio);
  
  //LÊ UM VALOR ALEATORIO ATRAVES DA PORTA ANALOGICA
  randomSeed(analogRead(0));
  
}

void loop(){

  Serial.println(digitalRead(pin_buttons[0]));

  
  //VERIFICA SE USUARIO PERDEU O JOGO E, CASO SIM, RESETA O JOGO
  if( game_over ){
    sequence[100] = {};
    current_round = 0;
    current_step = 0;
    game_over = false;
  }

  if(current_round == 0){
    startGame();
    delay(500);
  }
  
  //INICIA A PRÓXIMA RODADA
  nextRound();

  //MOSTRA SEQUENCIA QUE DEVERA SER MEMORIZADA
  playSequence();

  //AGUARDA JOGAR ESCOLHER OS BOTOES
  waitingPlayer(); 

  //PAUSA DE 1 SEGUNDO ENTRE JOGADAS
  delay(1000);
}


//DEFINE ENTRADA E SAIDA DOS PINOS
void definePins( String type, int pins[] ) {

  if(type == "out"){
    for (int i = 0; i <= 3; i++) {
      
      pinMode(pins[i], OUTPUT);
  
    }   
  }else{
    for (int i = 0; i <= 3; i++) {
  
      pinMode(pins[i], INPUT);
  
    }    
  }
}

//SEQUENCIA DE LUZES PARA INICIAR O JOGO
void startGame() {

  digitalWrite(pin_leds[0], HIGH);

  digitalWrite(pin_leds[1], HIGH);
  digitalWrite(pin_leds[2], HIGH);
  digitalWrite(pin_leds[3], HIGH);

  delay(500);

  digitalWrite(pin_leds[0], LOW);
  digitalWrite(pin_leds[1], LOW);
  digitalWrite(pin_leds[2], LOW);

  digitalWrite(pin_leds[3], LOW);
  delay(500);
}


//INICIA PROXIMA RODADA
void nextRound() {

  int luck_number = random(0, 4);

  sequence[current_round++] = luck_number;
  
}

//INICIA A SEQUENCIA QUE DEVE SER MEMORIZADA
void playSequence() {  
  for (int i = 0; i < current_round; i++) {
    
    tone(pin_audio[0], tons[sequence[i]]);
    
    digitalWrite(pin_leds[sequence[i]], HIGH);
    delay(500);
    
    noTone(pin_audio[0]);

    digitalWrite(pin_leds[sequence[i]], LOW);
    delay(100);
  }
  
    noTone(pin_audio[0]);  
}


//AGUARDANDO RODADA
void waitingRound(){

  bool already_played = false;

  while ( !already_played ) {

    for (int i = 0; i <= 3; i++) {

      if (digitalRead(pin_buttons[i]) == HIGH) {

        //QUAL BOTAO FOI PRESSIONADO
        button_pressed = i;

        tone(pin_audio[0], tons[i]);
        digitalWrite(pin_leds[i], HIGH);

        delay(300);

        digitalWrite(pin_leds[i], LOW);
        noTone(pin_audio[0]);
        
        already_played = true;

      }

    }

    delay(10);
  }  
}


//AGUARDANDO JOGADA
void waitingPlayer(){

  for (int i = 0; i < current_round; i++) {
    //AGUARDA JOGADA
    waitingRound();

    //VALIDA SEQUENCIA DIGITAL PELO USUARIO
    validate();

    //SE PERDEU, PARA O JOGO
    if ( game_over ) {
      break;
    }
    current_step++;
  }
  current_step = 0;  
}

void validate() {
  if (sequence[current_step] != button_pressed) {
    // GAME OVER.

    for (int i = 0; i <= 3; i++) {
      tone(pin_audio[0], tons[i]);
      digitalWrite(pin_leds[i], HIGH);
      delay(200);
      digitalWrite(pin_leds[i], LOW);
      noTone(pin_audio[0]);
    }

    for (int i = 0; i <= 3; i++) {
      tone(pin_audio[0], tons[0]);
      digitalWrite(pin_leds[0], HIGH);

      digitalWrite(pin_leds[1], HIGH);

      digitalWrite(pin_leds[2], HIGH);

      digitalWrite(pin_leds[3], HIGH);

      delay(100);

      digitalWrite(pin_leds[0], LOW);

      digitalWrite(pin_leds[1], LOW);

      digitalWrite(pin_leds[2], LOW);

      digitalWrite(pin_leds[3], LOW);

      delay(100);
      noTone(pin_audio[0]);
    }
    
    game_over = true;
  }  
}
