// Biblioteca comunicacao WIFI, WIFI MANAGER, SENSOR DHT11 E DNS PARA ESP32
#include <FS.h>          
#include <WiFiManager.h> 
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <locale.h>
#include <DHT.h> 
#include <ESPmDNS.h>
 
#define DHTPIN 4         // Pino de dados 
#define DHTTYPE DHT11    // Tipo do sensor (DHT11)
DHT dht(DHTPIN, DHTTYPE); 


#define LED_R  19        // pinagem dos leds
#define LED_G  18
#define LED_B  15


// Configuracoes de Endereco e de Internet WIFI
WiFiServer server(80); // porta
// variavel para armazenar a solicitacao HTTP
String header;

// variaveis para controle de tempo
unsigned long HoraAtual = millis(); 
unsigned long HoraAnterior = 0; 
const long tempoLimite = 2000;


// Wifi Manager
WiFiManager wifiManager;
// Status para controle dos Led
int Estado_do_led_r = LOW; 
int Estado_do_led_g = LOW; 
int Estado_do_led_b = LOW; 


   
void setup() {
  Serial.begin(115200);

  pinMode(DHTPIN, INPUT);   
  dht.begin(); 
  delay(8000);  // Atraso para garantir inicializacao do sensor 

  Serial.println("Configurar Rede Wifi");
  //  SETUP -- wifi -- ////////////////////////////
  wifiManager.autoConnect("Luis ESP32", "12341234");

  //wifiManager.resetSettings ();   // Apaga todas redes Salvas do Wifi

  delay(150);
  WiFi.begin();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("------------------------------------------------------------");
    Serial.println("Conectado IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("------------------------------------------------------------");

    delay(150);
    server.begin();
  }

  if (!MDNS.begin("esp32-luis")) {
    Serial.println("Erro no MDNS!");
    while (1) {
      delay(1000);
    }
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////

  //*** SETUP -- Definicao dos Pinos -- //////////
  delay(50);
  MDNS.addService("http", "tcp", 80);

  pinMode(LED_R,   OUTPUT);
  pinMode(LED_G,   OUTPUT);
  pinMode(LED_B,   OUTPUT);

  // Iniciam em LOW -> Desligados
  digitalWrite(LED_R,   LOW);
  digitalWrite(LED_G,   LOW);
  digitalWrite(LED_B,   LOW);
  Serial.println("Inicio de programa Web **************************");

}


void loop () {
  // leitura dht
  float temp = dht.readTemperature(); 
  float umi = dht.readHumidity(); 
  delay(3000);  // Atraso entre leituras
  if (isnan(temp) || isnan(umi)) {
    Serial.println("Falha na leitura do sensor DHT11!");
    return; 
  }
 
  setlocale(LC_ALL,"Portuguese_Brazil");
  WiFiClient client = server.available();    // Escutar os clientes que chegam
   if (client) {                             // Se um novo cliente se conectar
    HoraAtual = millis();
    HoraAnterior = HoraAtual;
    Serial.println("Novo Cliente."); // imprimir uma mensagem na porta serial
    String currentLine = "";         // faz uma string para conter os dados de entrada do cliente


    // faz um loop enquanto o cliente esta conectado e dentro do e dentro do tempo limite
    while (client.connected() && HoraAtual - HoraAnterior <= tempoLimite) {  

      HoraAtual = millis();
      if (client.available()) { // se houver bytes para ler do cliente
        char c = client.read(); // leia um byte, entao
        Serial.write(c);        // imprima no monitor serial
        header += c;
        if (c == '\n') {        // Quando o codigo detecta um caractere de nova linha ('\n'), ele verifica se a linha anterior esta' vazia.
          // uma linha vazia indica o fim da solicitação HTTP do cliente (ou seja, o cliente terminou de enviar cabecalhos).
          if (currentLine.length() == 0) {
            
            // Apos o fim da solicitacao, envia a seguinte resposta HTTP simples ao cliente
            client.println("HTTP/1.1 200 OK");       // 200 OK indica sucesso
            client.println("Content-type:text/html");// indica que a resposta sera em html
            client.println("Connection: close");     // indica que a conexão será fechada após a resposta
            client.println();
            // Verifica o caracter enviado pelo html com metodo GET

            if (header.indexOf("GET /L1") >= 0) {
              Estado_do_led_r =! Estado_do_led_r;
              digitalWrite(LED_R, Estado_do_led_r);
            }
            if (header.indexOf("GET /L2") >= 0) {
              Estado_do_led_g =! Estado_do_led_g;
              digitalWrite(LED_G, Estado_do_led_g);
            }
            if (header.indexOf("GET /L3") >= 0) {
              Estado_do_led_b =! Estado_do_led_b;
              digitalWrite(LED_B, Estado_do_led_b);
            }

            // Definicoes do corpo da pagina
            // Exibir a pagina web em HTML, CSS e JS
            const char* html_head = R"rawliteral(
            <!DOCTYPE html><html>
            <head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
            <link rel=\"icon\" href=\"data:,\">
            <title> Projeto ESP32 </title>                 
            )rawliteral";

            // CSS para estilizar os botões e Pagina
            const char* css = R"rawliteral(
            <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-YvpcrYf0tY3lHB60NNkmXc5s9fDVZLESaAA55NDzOxhy9GkcIdslK1eN7N6jIeHz" crossorigin="anonymous"></script>
            <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-QWTKZyjpPEjISv5WaRU9OFeRpok6YctnYmDr5pNlyT2bRjXh0JMhjY6hW+ALEwIH" crossorigin="anonymous">
            <style>
              html {
                font-family: Helvetica;
                display: inline-block;
                margin: 0px auto;
                text-align:
                center; background-color:
                #F0FFF0;}
              body {
                padding-top: 0px: 
              }
              header{
                background-image: url('https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQLrLEzKVETKcJgfNRqPnHYsGkcAnsN1elX_bDtasHVqQPyxd_qgOqerOXP3hK9XcFZ8lo&usqp=CAU');
                color: white;
                padding: 20px;
              }
              .img-size {
                margin 10px;
                width: 500px;
                height: 300px;
                object-fit: cover; 
              }
              img-link {
                width: 600px;
                height: auto;
              }
              @media (max-width: 576px) {
                .img-size {
                  width: 100%; 
                  height: auto; 
                }
                img-link {
                  width: 300px;
                  height: auto;
                }
              }
              article{
                display: block
              }


            </style>
            </head>
            )rawliteral";
            client.print(html_head);
            client.print(css);


            //**************************************************
            // Título da página da web, temperatura e umidade
            client.print(R"rawliteral(
              <body class="h-100 bg-info-subtle d-flex justify-content-center flex-column">
              <header>
              <h1>Projeto ESP32 WebServer</h1>
                <p><em>por Luis Carlos</em></p>
              </header>
              <main class="gap-5">
                <div class=" d-flex gap-5 justify-content-evenly">
                  <div class="card" style="width: 18rem;">
                  <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTeDIVWn1sy9rYNEdJ7KApPdw952JkP25zl2A&s" class="card-img-top" alt="esp32">
                  <div class="card-body">
                    <h5 class="card-title">ESP32</h5>
                    <p>Microcontrolador de 32 bits com Wi-Fi</p>
                    
                  </div>
                  </div>
                  <div class="card" style="width: 18rem;">
                  <img src="https://down-br.img.susercontent.com/file/e35f413f32af61f4b93c871674677e23" class="card-img-top" alt="sensor dht11">
                  <div class="card-body">
                    <h5 class="card-title">DHT11</h5>
                    <p>Sensor de Temperatura e Umidade</p>
                    
                  </div>
                  </div>
                  <div class="card" style="width: 18rem;">
                  <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcS3XyhFhROWowb2I-6Kb07B1ZZEMpCZaNxh6A&s" class="card-img-top" alt="led">
                  <div class="card-body">
                    <h5 class="card-title">LED</h5>
                    <p>Diodo emissor de luz</p>
                    
                  </div>
                  </div>
                  
                </div>
               
                <section>
                <div class="fs-3 ">
                  <p class="text-danger fw-semibold"> Temperatura: )rawliteral");
                  client.print(String(temp, 1));
                  client.print(R"rawliteral( C</p>
                  <p class="text-info-emphasis fw-semibold"> Umidade: )rawliteral");
                  client.print(String(umi, 1));
                  client.print(R"rawliteral( %</p>
                </div>
                </section>
                )rawliteral");


                // Atualizacao do HTML com GET e estilizacao com Bootstrap/CSS
                client.print(R"rawliteral(
                <section>
                  <div class="d-grid gap-2 col-6 mx-auto">)rawliteral");
                    if (Estado_do_led_r) {
                      client.print("<a href=\"/L1\"><button class=\" btn btn-danger btn-lg\">LED-RED-ON</button></a>");
                    } else {
                      client.print("<a href=\"/L1\"><button class=\" btn btn-outline-danger btn-lg\">LED-RED-OFF</button></a>");
                    }
                
                    if (Estado_do_led_g) {
                      client.print("<a href=\"/L2\"><button class=\" btn btn-success btn-lg\">LED-GREEN-ON</button></a>");
                    } else {
                      client.print("<a href=\"/L2\"><button class=\" btn btn-outline-success btn-lg\">LED-GREEN-OFF</button></a>");
                    }
            
                    if (Estado_do_led_b) {
                      client.print("<a href=\"/L3\"><button class=\" btn btn-primary btn-lg\">LED-BLUE-ON</button></a>");
                    } else {
                      client.print("<a href=\"/L3\"><button class=\" btn btn-outline-primary btn-lg\">LED-BLUE-OFF</button></a>");
                    }
                client.print(R"rawliteral(
                </div>
                </section>
                <article>
                <div class="d-flex justify-content-center">
                  <div id="carouselExampleInterval" class="carousel slide" data-bs-ride="carousel">
                    <div class="carousel-inner img-size">
                      <div class="carousel-item img-link active" data-bs-interval="10000">
                        <!-- Imagem com link -->
                        <a href="https://getbootstrap.com/">
                          <img src="https://getbootstrap.com/docs/4.5/assets/img/bootstrap-icons.png" class="d-block w-80 img-size" alt="bootstrap">
                        </a>
                      </div>
                      <div class="carousel-item img-link img-size" data-bs-interval="2000">
                        <!-- Imagem com link -->
                        <a href="https://ebaconline.com.br/blog/o-que-e-linguagem-c">
                          <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTX62ONGoUTZ15q4ef7KHpKy0XDTYmvpz-OcQ&s" class="d-block w-80 img-size" alt="linguagem de programacao C">
                        </a>
                      </div>
                      <div class="carousel-item img-link img-size">
                        <!-- Imagem com link -->
                        <a href="https://www.arduino.cc/en/software">
                          <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSc10l1ts2JCDc7ywPIHEgArE9jfGNCpfl7Lg&s" class="d-block w-80 img-size" alt="arduino ide">
                        </a>
                      </div>
                    </div>
                    <button class="carousel-control-prev" type="button" data-bs-target="#carouselExampleInterval" data-bs-slide="prev">
                      <span class="carousel-control-prev-icon" aria-hidden="true"></span>
                      <span class="visually-hidden">Previous</span>
                    </button>
                    <button class="carousel-control-next" type="button" data-bs-target="#carouselExampleInterval" data-bs-slide="next">
                      <span class="carousel-control-next-icon" aria-hidden="true"></span>
                      <span class="visually-hidden">Next</span>
                    </button>
                  </div>
                </div>
                </article>

                </main>
                <footer>
                  <p class="fs-6 p-4 m-3">Todos os direitos reservados</p>
                </footer>
                </body>
                <script>
                  // Salva a posicao do scroll antes de recarregar ou sair da pagina
                  window.onbeforeunload = function () {
                    localStorage.setItem("scrollPosition", window.scrollY);
                  };

                  // Restaura a posicao do scroll ao carregar a pagina
                  window.onload = function () {
                    const scrollPosition = localStorage.getItem("scrollPosition");
                    if (scrollPosition) {
                      window.scrollTo(0, parseInt(scrollPosition, 10));
                    }
                  };
                </script>

              </html>
              )rawliteral");
            client.println();               // A resposta HTTP termina com outra linha em branco:
            break;                          // interromper o loop while:
          } else {
            currentLine = "";               // se você recebeu uma nova linha, limpe currentLine:
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    // Limpar o header
    header = "";
    // fecha a conexão:
    client.stop();
    Serial.println("Cliente Disconectado.");
    Serial.println("---------------------");
  }
}
