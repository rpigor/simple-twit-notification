## Simple Twitter Notification Client

Implementação de um simples cliente de linha de comando para um sistema de notificações semelhante ao Twitter aplicando conceitos de comunicação com protocolo UDP e programação concorrente com threads para a disciplina de Sistemas Operacionais II N - *INF01151*.

O projeto foi programado para sistemas de tipo Unix e particularmente testado em sistemas Linux e macOS.

### Como usar

Para compilar, execute o comando `make`:

    make all

Em seguida, execute o arquivo `main` com os seguintes parâmetros para logar com o cliente em um servidor:

    ./main <profile> <ip> <port>

Onde `profile` é o nome de usuário do perfil a iniciar sessão, `ip` é o endereço IPv4 do servidor e `port` a sua porta.

Por exemplo, para se logar com o perfil `@igor` em um servidor local rodando na porta 8888:

    ./main @igor 127.0.0.1 8888