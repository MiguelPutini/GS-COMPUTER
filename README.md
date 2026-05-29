# 🚀 Sistema IoT para Monitoramento de Cápsula Espacial

Sistema embarcado desenvolvido com Arduino Uno para monitoramento em tempo real das condições internas de uma cápsula espacial simulada. O projeto utiliza sensores de temperatura, luminosidade e vibração, com exibição dos dados em display LCD e alertas automáticos.

---

## 📷 Circuito

![Circuito Completo](imagens/circuito_completo.png)

---

## 🎯 Objetivo

Desenvolver um sistema IoT capaz de monitorar variáveis físicas essenciais para a operação segura de um módulo espacial, coletando, processando e exibindo informações em tempo real sobre as condições internas da cápsula.

---

## 🔧 Componentes Utilizados

| Componente | Quantidade | Função |
|---|---|---|
| Arduino Uno | 1 | Microcontrolador principal |
| LCD 16x2 I2C (PCF8574) | 1 | Exibição dos dados em tempo real |
| Sensor TMP36 | 1 | Medição de temperatura |
| LDR (Fotoresistor) | 1 | Medição de luminosidade |
| SW-200D (Tilt Sensor) | 1 | Detecção de vibração e impactos |
| LED Vermelho | 1 | Alerta visual |
| Resistor 10kΩ | 2 | Pull-down para LDR e SW-200D |
| Resistor 220Ω | 1 | Proteção do LED |
| Protoboard | 1 | Montagem do circuito |

---

## 🔌 Mapa de Pinos

| Componente | Pino Arduino | Tipo |
|---|---|---|
| LCD SDA | A4 | Digital (I2C) |
| LCD SCL | A5 | Digital (I2C) |
| LCD VCC | 5V | Alimentação |
| LCD GND | GND | Alimentação |
| TMP36 — Sinal | A0 | Analógico |
| TMP36 — VCC | 5V | Alimentação |
| TMP36 — GND | GND | Alimentação |
| LDR — Sinal | A1 | Analógico |
| SW-200D — Sinal | D2 | Digital |
| LED — Anodo (+) | D13 | Digital |
| LED — Catodo (-) | GND | Alimentação |

---

## ⚙️ Funcionalidades

- 🌡️ **Temperatura** — leitura em °C com indicador de status [OK] ou [ALTO]
- 💡 **Luminosidade** — detecção de luz [BAIXA], [OK] ou [ALTA]
- 📳 **Vibração** — detecção de impactos com status Normal [OK] ou DETECTADA! [!]
- 🚨 **Alertas automáticos** — LED acende e LCD exibe "!! ATENCAO !! CHECAR SISTEMAS" quando algum sensor ultrapassa os limites
- 🔄 **Telas alternadas** — display alterna entre os 3 sensores a cada 2 segundos
- 📡 **Monitor serial** — todos os dados transmitidos em tempo real pelo serial a 9600 baud

---

## 📊 Limites de Alerta

| Sensor | Condição | Limite | Ação do Sistema |
|---|---|---|---|
| Temperatura | Acima de | 35°C | LED acende + aviso no LCD |
| Luminosidade | Abaixo de | 200 | LED acende + aviso no LCD |
| Luminosidade | Acima de | 800 | LED acende + aviso no LCD |
| Vibração | Qualquer detecção | — | LED acende + aviso no LCD |

---


## 📷 Simulação em Funcionamento

| Temperatura | Luminosidade |
|---|---|
| ![LCD Temperatura](imagens/lcd_temperatura.png) | ![LCD Luminosidade](imagens/lcd_luminosidade.png) |

| Vibração | Alerta Ativo |
|---|---|
| ![LCD Vibração](imagens/lcd_vibracao.png) | ![LCD Alerta](imagens/lcd_alerta.png) |

### Monitor Serial

![Monitor Serial](imagens/monitor_serial.png)

---

## 🛠️ Como Simular no Tinkercad

1. Acesse [tinkercad.com](https://www.tinkercad.com) e crie uma conta
2. Clique em **Criar novo projeto de circuito**
3. Adicione os componentes: Arduino Uno, LCD 16x2 I2C, TMP36, LDR, SW-200D, LED, resistores e protoboard
4. Monte o circuito seguindo o mapa de pinos acima
5. Clique em **Código** e troque para modo **Texto**
6. Apague o conteúdo e cole o código do arquivo `capsula_espacial.ino`
7. Clique em **Iniciar Simulação**

---

## 💻 Trecho Principal do Código

```cpp
// Converte leitura do TMP36 para graus Celsius
float lerTemperatura() {
  int leitura = analogRead(PINO_TEMPERATURA);
  float tensao = leitura * (5.0 / 1023.0);
  float temperatura = (tensao - 0.5) * 100.0;
  return temperatura;
}

// Verifica se algum sensor está fora do normal
bool verificarAlertas(float temp, int luz, bool vibracao) {
  if (temp > TEMP_MAXIMA) return true;
  if (luz < LUZ_MINIMA)   return true;
  if (luz > LUZ_MAXIMA)   return true;
  if (vibracao)            return true;
  return false;
}
```

---

## 📚 Bibliotecas Necessárias

| Biblioteca | Finalidade |
|---|---|
| `Wire.h` | Comunicação I2C nativa do Arduino |
| `LiquidCrystal_I2C.h` | Controle do display LCD via I2C |

---

## 🔭 Tecnologias Utilizadas

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Tinkercad](https://img.shields.io/badge/Tinkercad-FF6B00?style=for-the-badge&logo=autodesk&logoColor=white)

---

## 🌍 Conexão com ODS da ONU

Este projeto se conecta com os seguintes Objetivos de Desenvolvimento Sustentável:

| ODS | Descrição | Relação com o Projeto |
|---|---|---|
| ODS 9 | Inovação e Infraestrutura | Uso de tecnologia IoT e sistemas embarcados |
| ODS 11 | Cidades Inteligentes | Monitoramento automatizado de ambientes críticos |
| ODS 13 | Ação Climática | Sensoriamento ambiental em tempo real |

---

## 👨‍🚀 Melhorias Futuras

- [ ] Adicionar sensor de pressão BMP280
- [ ] Integrar módulo Wi-Fi ESP8266 para envio remoto de dados
- [ ] Implementar cartão SD para registro histórico das leituras
- [ ] Adicionar buzzer para alertas sonoros
- [ ] Sensor de umidade DHT22
- [ ] Dashboard web para visualização dos dados em tempo real
