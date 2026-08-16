# ArduinoEV3_lib
Biblioteca para comunicação entre a plataforma Lego EV3 (Master) e Arduino (Slave) via protocolo I2C.

- ArduinoEV3: biblioteca para Arduino

- EV3Arduino: biblioteca para EV3

## Funcionamento
A biblioteca se baseia em um vetor que armazena as informações necessárias para envio, sendo cada posição do vetor um "canal", que armazena um dado numérico. Assim, cada canal pode armazenar uma informação diferente, por exemplo [canal 0 = IR , canal 1 = Ultrassônico , canal 2 = potênciometro].

Como o vetor fica armazenado na memória do Arduino, o Arduino pode ler/escrever nesse vetor livremente. Já o EV3, precisa requisitar a leitura/escrita para o Arduino, para que só assim, o Arduino opere com o vetor.

 A função do Arduino `.write(i, x)` armazena o dado x no canal i do vetor, enquanto a função `.read(i)` retorna o dado escrito no canal i do vetor. 

 Do lado do EV3, a função `.readArduino(port, i)` requisita ao Arduino a leitura do canal i, de maneira que, quando receber a resposta(o dado), retorna ela. A função `.writeArduino(port, i, x)` requisita a escrita do dado x no canal i do vetor do Arduino que, ao receber, o altera.

 A função `.begin()` do Arduino inicializa os parâmetros necessários para comunicação I2C. Se for necessário mudar o endereço I2C do Arduino, utilizar `.begin(novoEnderecoI2C)`. Por default, esse endereço é 0x04. Se você alterá-lo, é necessário alterar o endereço correspondente no EV3, com `.set_address(novoEnderecoI2C)`. 

ATENÇÃO: Note que a biblioteca pode dar conflito quando é necesário ler sensores I2C no Arduino, pois ele teria que agir como Slave e como Master ao mesmo tempo. Assim, recomenda-se o uso de um arduino DUE, ou ESP32, que possuem dois ou mais bus I2C. Para tal, utilizar a função `.begin(enderecoI2C, &Wire)` especificando qual bus da Wire utilizar para comunicar com o brick(para a Wire1, escrever &Wire1, para Wire2, &Wire2, etc).

LIMITAÇÕES: 16 canais(arbitrário); 4 bytes Arduino->EV3, 3 bytes EV3->Arduino (limitação da comunicação).
## Baixar
No repositório, baixar a biblioteca inteira(<>code -> dowload .zip).

Para o Arduino, copiar a pasta 'ArduinoEV3' e colar na pasta 'libraries' do Arduino(C:\Program Files (x86)\Arduino\libraries).

Para o EV3, copiar o arquivo 'EV3Arduino.h' e colar na pasta 'includes' do ROBOTC(C:\Program Files (x86)\Robomatter Inc\ROBOTC Development Environment 4.X\Includes).
