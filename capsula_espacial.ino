#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pinos dos sensores
#define PINO_TEMPERATURA A0   // TMP36 no pino analógico A0
#define PINO_LDR         A1   // Fotoresistor no pino analógico A1
#define PINO_VIBRACAO    2    // SW-200D no pino digital 2
#define PINO_LED         13   // LED de alerta no pino digital 13

// Configura o LCD com endereço 0x27, 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Valores limite para disparar alertas
const float TEMP_MAXIMA = 35.0;  // Temperatura máxima segura em °C
const int   LUZ_MINIMA  = 200;   // Abaixo disso, escuridão anormal
const int   LUZ_MAXIMA  = 800;   // Acima disso, luz intensa demais

// Variáveis para controlar qual tela está sendo exibida no LCD
int telaAtual = 0;
unsigned long ultimaTroca = 0;
const unsigned long INTERVALO_TELA = 2000; // Troca de tela a cada 2 segundos

void setup() {
  // Inicia a comunicação serial para acompanhar os dados pelo computador
  Serial.begin(9600);

  // Define o SW-200D como entrada com resistor interno
  // Com PULLUP, o pino fica em HIGH normalmente e vai LOW quando detecta vibração
  pinMode(PINO_VIBRACAO, INPUT_PULLUP);

  // Define o LED como saída para acender os alertas
  pinMode(PINO_LED, OUTPUT);

  // Inicializa o LCD e liga a luz de fundo
  Wire.begin();
  lcd.init();
  lcd.backlight();

  // Exibe mensagem de boas vindas por 2 segundos antes de começar
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CAPSULA ESPACIAL");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando......");
  delay(2000);
  lcd.clear();

  Serial.println("Sistema de monitoramento iniciado!");
}

void loop() {
  // Faz a leitura dos três sensores a cada ciclo
  float temperatura  = lerTemperatura();
  int   luminosidade = analogRead(PINO_LDR);
  bool  vibracao     = lerVibracao();

  // Verifica se algum sensor está fora do normal
  // Se sim, acende o LED de alerta
  bool alerta = verificarAlertas(temperatura, luminosidade, vibracao);
  digitalWrite(PINO_LED, alerta ? HIGH : LOW);

  // Manda os dados para o monitor serial
  Serial.print("Temp: ");
  Serial.print(temperatura);
  Serial.print("C | Luz: ");
  Serial.print(luminosidade);
  Serial.print(" | Vibracao: ");
  Serial.println(vibracao ? "SIM" : "NAO");

  // Verifica se já passou o tempo de trocar a tela
  unsigned long agora = millis();
  if (agora - ultimaTroca >= INTERVALO_TELA) {
    ultimaTroca = agora;
    telaAtual = (telaAtual + 1) % 3; // Alterna entre 0, 1 e 2
    lcd.clear();
  }

  // Exibe a tela do sensor correspondente
  switch (telaAtual) {
    case 0: exibirTemperatura(temperatura); break;
    case 1: exibirLuminosidade(luminosidade); break;
    case 2: exibirVibracao(vibracao); break;
  }

  // Se houver alerta, pisca uma mensagem de aviso no LCD
  if (alerta) {
    delay(800);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!! ATENCAO !!");
    lcd.setCursor(0, 1);
    lcd.print("CHECAR SISTEMAS");
    delay(800);
    lcd.clear();
  }

  delay(300); // Pequena pausa antes do próximo ciclo
}

// Converte a leitura analógica do TMP36 para graus Celsius
// O TMP36 gera 10mV por grau com offset de 500mV — fórmula do fabricante
float lerTemperatura() {
  int leitura = analogRead(PINO_TEMPERATURA);
  float tensao = leitura * (5.0 / 1023.0);       // Converte para volts
  float temperatura = (tensao - 0.5) * 100.0;     // Converte para Celsius
  return temperatura;
}

// Lê o sensor de vibração SW-200D
// Retorna true quando detecta vibração ou inclinação
bool lerVibracao() {
  return digitalRead(PINO_VIBRACAO) == LOW;
}

// Compara os valores lidos com os limites definidos
// Retorna true se qualquer sensor estiver fora do normal
bool verificarAlertas(float temp, int luz, bool vibracao) {
  if (temp > TEMP_MAXIMA) return true; // Temperatura alta demais
  if (luz < LUZ_MINIMA)   return true; // Muito escuro
  if (luz > LUZ_MAXIMA)   return true; // Luz intensa demais
  if (vibracao)            return true; // Impacto ou turbulência detectada
  return false;
}

// Exibe a temperatura no LCD com indicador de status
void exibirTemperatura(float temp) {
  lcd.setCursor(0, 0);
  lcd.print("TEMPERATURA:    ");
  lcd.setCursor(0, 1);
  lcd.print(temp, 1);   // Uma casa decimal
  lcd.print("C ");
  lcd.print(temp > TEMP_MAXIMA ? "[ALTO]" : "[OK]  ");
}

// Exibe a luminosidade e seu status no LCD
void exibirLuminosidade(int luz) {
  lcd.setCursor(0, 0);
  lcd.print("LUMINOSIDADE:   ");
  lcd.setCursor(0, 1);
  lcd.print(luz);
  lcd.print(" ");
  if (luz < LUZ_MINIMA)       lcd.print("[BAIXA]");
  else if (luz > LUZ_MAXIMA)  lcd.print("[ALTA] ");
  else                         lcd.print("[OK]   ");
}

// Exibe o status do sensor de vibração no LCD
void exibirVibracao(bool vibracao) {
  lcd.setCursor(0, 0);
  lcd.print("VIBRACAO:       ");
  lcd.setCursor(0, 1);
  lcd.print(vibracao ? "DETECTADA!  [!]" : "Normal      [OK]");
}
