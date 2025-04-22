# 🌐 Projeto ESP32 WebServer com Sensor DHT11 e Controle de LEDs

Este projeto implementa um WebServer embarcado no ESP32 que permite:

- 🔌 Conectar-se via Wi-Fi com configuração automática usando WiFiManager  
- 🌡️ Ler dados de temperatura e umidade do sensor DHT11  
- 🔴🟢🔵 Controlar três LEDs RGB via navegador web  
- 🌍 Acessar a página via hostname local usando mDNS (`esp32-luis.local`)  
- 🎨 Interface moderna com Bootstrap  

---

## 📷 Imagens do Projeto

### Visão Geral
<!-- Imagem do projeto geral aqui -->
![Imagem Geral do Projeto](CAMINHO_PARA_IMAGEM_1)

### Sensor DHT11, ESP32 e LEDs
<!-- Imagem detalhada de sensores/conexões -->
![Componentes Conectados](CAMINHO_PARA_IMAGEM_2)

---

## ⚙️ Funcionalidades

- Interface Web responsiva com Bootstrap 5
- Leitura dos dados do sensor DHT11 (temperatura e umidade)
- Controle dos LEDs via requisições GET:
  - `GET /L1`: Alterna LED Vermelho
  - `GET /L2`: Alterna LED Verde
  - `GET /L3`: Alterna LED Azul
- Acesso direto ao ESP32 por `http://esp32-luis.local` usando mDNS
- Conexão Wi-Fi simplificada usando o portal de configuração do WiFiManager

---

## 📡 Componentes Utilizados

| Componente   | Descrição                              |
|--------------|----------------------------------------|
| ESP32        | Microcontrolador com Wi-Fi integrado   |
| DHT11        | Sensor de temperatura e umidade        |
| LEDs (R/G/B) | Indicadores visuais controlados via web|
| Breadboard   | Protoboard para montagem dos componentes|
| Jumpers      | Cabos de conexão                       |

---

## 📦 Instalação e Execução

1. Conecte os componentes conforme o esquema:
   - **DHT11:** Pino de dados no GPIO 4
   - **LEDs:**
     - Vermelho: GPIO 19  
     - Verde: GPIO 18  
     - Azul: GPIO 15

2. Carregue o código na placa ESP32 via Arduino IDE ou PlatformIO.

3. Na primeira execução, o ESP32 cria uma rede Wi-Fi chamada **Luis ESP32** com senha **12341234**.

4. Conecte-se a essa rede, acesse o IP `192.168.4.1` e configure a rede Wi-Fi doméstica.

5. Acesse a interface web via IP local ou via `http://esp32-luis.local`.

---

## 💡 Demonstração da Interface Web

<!-- Captura de tela da página web aqui -->
![Interface Web](CAMINHO_PARA_IMAGEM_3)

---

## 🧠 Sobre o Código

- O WebServer escuta na porta 80.
- Os dados são atualizados a cada leitura do sensor (a cada 3 segundos).
- A interface HTML+CSS+JS usa Bootstrap para responsividade e estilo.
- O cabeçalho HTTP é analisado para detectar cliques nos botões.

---

## ✍️ Autor

**Luis Carlos**  
Estudante de Engenharia de Computação  
[LinkedIn](https://www.linkedin.com/) | [GitHub](https://github.com/)

---

## 📜 Licença

Este projeto é de código aberto sob a licença MIT. Veja o arquivo `LICENSE` para mais detalhes.
