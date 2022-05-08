## Simple Twitter Notification Server

Implementação de um simples servidor de notificações com dinâmica semelhante à do Twitter. Aplica os conceitos de programação concorrente com threads, comunicação com protocolo UDP e replicação passiva que foram estudados na disciplina de Sistemas Operacionais II N - *INF01151*.

O projeto foi programado para sistemas de tipo Unix e particularmente testado em sistemas Linux e macOS.

### Como usar

Para compilar, execute o comando `make`:

    make all

Em seguida, execute o arquivo `main` para rodar o servidor:

    ./main

Por padrão, o servidor local atende requisições de clientes na porta 8888. Além disso, ele utiliza o endereço *multicast* `228.8.8.8` e a porta 8889 para fins de comunicação de grupo.

Para fazer uso da funcionalidade de replicação passiva, basta iniciar diversas instâncias do arquivo `main`. Antes de iniciar uma nova instância, certifique-se que as instâncias em execução foram inicializadas com sucesso, já tendo retornado os seus identificadores únicos.