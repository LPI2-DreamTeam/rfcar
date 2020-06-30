
# Table of Contents

1.  [Bluetooth Chat (BT Chat)](#org49f1c6b)
2.  [Intro: BT](#org9c098e9)
    1.  [Conexões de saída](#org9bd60b3)
    2.  [Conexões de entrada](#orgf320987)
    3.  [Sockets: Cliente e Servidor](#org06d6505)
    4.  [RFCOMM](#org524de0c)
    5.  [Service Discovery Protocol (SDP)](#org3fc4e33)
        1.  [Service Record](#orgf4456c5)
3.  [Análise *v1*](#orgf82aa6d)
    1.  [Funcionalidades <code>[5/5]</code>](#org87f4cdb)
    2.  [User Mockups](#org2015214)
    3.  [Decisões: Stack e Plataforma de desenvolvimento](#org07d62f3)
4.  [Design *v1*](#org50ffbac)
    1.  [Diagrama de Classes](#org2a1a1ce)
    2.  [Finite State Machine](#orgdd37361)
    3.  [Deployment](#orgc96a367)
5.  [Implementação *v1*](#org69702a0)
    1.  [Main: App Server](#orgf9185a3)
    2.  [Main: App Client](#org20a287a)
6.  [Testes *v1*](#org3bae28e)
7.  [Resultados *v1* <code>[6/6]</code>](#orgf1f1539)
8.  [Trabalho futuro *v1* <code>[0/6]</code>](#orgbc307a4)
9.  [Análise *v2*](#org7b960a0)
10. [Design *v2*: <span class="underline">Threads</span>](#org2187118)
    1.  [Design *v2*: <span class="underline">Threads</span>](#org6c09292)
    2.  [Design *v2*: <span class="underline">Threads</span>](#org0cdc3a7)
    3.  [Design *v2*: <span class="underline">Threads</span>](#org7c9768b)
    4.  [Design *v2*: <span class="underline">Threads</span>](#org1c686ad)
    5.  [Design *v2*: <span class="underline">Threads</span>](#org1539949)
    6.  [Design *v2*: <span class="underline">Threads</span>](#org8369327)
    7.  [Design *v2*: <span class="underline">Threads</span>](#org7f6a87c)
11. [Implementação *v2*](#org144a07c)
    1.  [Implementação *v2*: <span class="underline">Threads</span>](#orgca57cd3)
    2.  [Implementação *v2*: <span class="underline">Threads</span>](#orgbd6f99d)
    3.  [Implementação *v2*: <span class="underline">User Interface</span>](#orgbc52243)
12. [Trabalho futuro *v2* <code>[0/1]</code>](#org112c551)
13. [Conclusões <code>[6/6]</code>](#orga26ea7b)
14. [Ferramentas](#org985bbe1)
    1.  [Controlo de versões: Git](#org883c40b)
    2.  [Documentação](#org4576432)
15. [Referências <code>[3/3]</code>](#org2039d0f)



<a id="org49f1c6b"></a>

# Bluetooth Chat (BT Chat)

O **BT Chat** é uma aplicação para troca de mensagens entre 2 dispositivos (PCs ou
telemóveis).

-   O protocolo de troca de dados deve ser simples, p. ex., uma aplicação de curto
    alcance.
-   Pode incluir interface gráfica


<a id="org9c098e9"></a>

# Intro: BT

O Bluetooth (BT) é uma tecnologia de comunicação:

-   sem fios (ondas rádio) e
-   a curtas distâncias, geralmente inferiores a 10 metros.

**Protocolos de transporte**:

![img](doc/img/screenshot-20190520-014928.png "Protocolos de transporte BT")


<a id="org9bd60b3"></a>

## Conexões de saída

![img](doc/img/screenshot-20190519-211851.png "Procedimento para Conexões de saída")


<a id="orgf320987"></a>

## Conexões de entrada

![img](doc/img/screenshot-20190519-212511.png "Procedimento para Conexões de entrada")


<a id="org06d6505"></a>

## Sockets: Cliente e Servidor

![img](doc/img/screenshot-20190520-152535.png "Sockets Cliente e Servidor")


<a id="org524de0c"></a>

## RFCOMM

O **Radio Frequency Communications** (RFCOMM) é um protocolo

-   **fiável** (*garantia* de entrega: deliver-or-die)
-   baseado em **streams** (semântica). Envia os dados sem se preocupar onde um
    pacote começa e outro acaba.
-   Emula bem as portas séries.
-   **Portos**: 30 portos abertos por máquina.
-   **Única opção**: dependendendo da app e da plataforma-alvo, por vezes é o
    protocolo disponível.


<a id="org3fc4e33"></a>

## Service Discovery Protocol (SDP)

![img](doc/img/screenshot-20190520-023315.png "Service Discovery Protocol")


<a id="orgf4456c5"></a>

### Service Record

![img](doc/img/screenshot-20190520-023829.png "Service Record")


<a id="orgf82aa6d"></a>

# Análise *v1*


<a id="org87f4cdb"></a>

## Funcionalidades <code>[5/5]</code>

-   [X] Verificar estado da comunicação BT
-   [X] Pesquisar dispositivos na proximidade
-   [X] Emparelhar com um dispositivo
-   [X] Enviar msg, lida do utilizador, para o dispositivo emparelhado
-   [X] Receber msg do dispositivo emparelhado e apresentar ao utilizador


<a id="org2015214"></a>

## User Mockups

![img](doc/img/bt-chat-UI-mockups.png "User mockups")


<a id="org07d62f3"></a>

## Decisões: Stack e Plataforma de desenvolvimento

O BT é uma tecnologia de comunicação complexa e cuja implementação varia de SO
em SO.

-   Foi escolhida a stack `BlueZ`, pois fornece suporte a todos os protocolos BT e
    é open-source.
-   O SO escolhido foi, portanto, o `GNU/Linux`.

![img](doc/img/screenshot-20190520-204139.png "Suporte aos protocolos de comunicação BT na stack `BlueZ`")


<a id="org50ffbac"></a>

# Design *v1*


<a id="org2a1a1ce"></a>

## Diagrama de Classes

![img](doc/html/class_app_server__coll__graph.png "Diagrama de classes")


<a id="orgdd37361"></a>

## Finite State Machine

![img](doc/img/bt-chat-fsm.png "Finite State Machine")


<a id="orgc96a367"></a>

## Deployment

![img](doc/img/bt-chat-Deployment.png "Diagrama de instalação (deployment)")


<a id="org69702a0"></a>

# Implementação *v1*


<a id="orgf9185a3"></a>

## Main: App Server

    int main(int argc, char **argv)
    {
        AppServer *app;
    
    /* Construct the app */
        try{
    	app = new AppServer();
        }catch(runtime_error &e)
        {
    	cout << e.what() << endl;
        }
    
    /* Execute the app */
        app->exec();
        return 0;
    }


<a id="org20a287a"></a>

## Main: App Client

    int main(int argc, char **argv)
    {
        AppClient *app;
    
    /* Construct the app */
        try{
    	app = new AppClient();
        }catch(runtime_error &e)
        {
    	cout << e.what() << endl;
        }
    
    /* Execute the app */
        app->exec();
        return 0;
    }


<a id="org3bae28e"></a>

# Testes *v1*

![img](doc/img/rfcomm-client-server-bt-succ.png "Conexão com sucesso entre 2 Raspberry Pi: server (esq.); cliente (dir.)")


<a id="orgf1f1539"></a>

# Resultados *v1* <code>[6/6]</code>

-   [X] Abstracção da stack `Bluez` para Comunicação BT em `C++`
-   [X] Criação dum cliente e servidor que comunicam entre si
-   [X] Transmissão e recepção de mensagens (síncrona)
-   [X] Registo de serviços BT e pesquisa dos mesmos
-   [X] Histórico de mensagens através duma base de dados (ficheiros)
-   [X] Controlo da aplicação por FSM


<a id="orgbc307a4"></a>

# Trabalho futuro *v1* <code>[0/6]</code>

-   [ ] Fusão das 2 aplicações numa
-   [ ] Elevar o nível da API da classe `RFCommSocketServer`
-   [ ] Uso de multithreading para tornar a aplicação mais responsiva
-   [ ] Criação duma interface gráfica
-   [ ] Melhoria da descoberta de dispositivos
-   [ ] Melhoria da base de dados


<a id="org7b960a0"></a>

# Análise *v2*

-   [ ] <span class="underline">Fusão das 2 aplicações numa</span>: os papéis serão determinados pelo dispositivo
    que inicia a conexão (cliente)
-   [ ] <span class="underline">Elevar o nível da API da classe `RFCommSocketServer`</span>: as funções `bind` e
    `listen` devem constar do construtor.
-   [ ] <span class="underline">Uso de multithreading</span>: é necessário definir os sockets como
    não-blocantes, para que todas as funções-membro não bloqueiem, permitindo o
    multithreading.
-   [ ] <span class="underline">Interface gráfica</span>: utilização da plataforma `Qt` por ser
    `cross-platform`.


<a id="org2187118"></a>

# Design *v2*: <span class="underline">Threads</span>

-   **Modelação de actividades concorrentes**: que ocorrem virtualmente de forma
    independente umas das outras
    -   Concorrência com múltiplos processos
    -   Concorrência com múltiplas threads num único processo: menor overhead
-   **Threads**: entidade computacional mais básica capaz de ser executada de forma
    independente por um CPU, com contexto próprio e espaço de endereços
    partilhado. Executa uma função associada.

![img](doc/img/screenshot-20190626-010325.png "Comunicação entre 2 threads que executam concorrentemente num único processo")


<a id="org6c09292"></a>

## Design *v2*: <span class="underline">Threads</span>

-   **Suporte**: a partir do `C++11`, a STL suporta threads, mutexes, variáveis
    conditionais, etc.
-   **Gestão**: 
    -   as threads podem criadas com uma função de thread 
        
            this->accept_th = new thread(accept_thread_fcn);
    -   têm de ser esperadas ou desacopladas (correm em BG)
        
            accept_th->join();
            accept_th->detach();
    -   Podem ser passados parâmetros para a thread por valor ou por refª
        
            thread t(&App::send_socket_thread_fcn, this, s);
            thread t(&App::send_socket_thread_fcn, this, std::ref(s));
    -   Executam até terminar a função de thread


<a id="org0cdc3a7"></a>

## Design *v2*: <span class="underline">Threads</span>

**Partilha de dados**: o espaço de memória das threads é partilhado e
 frequentemente têm de aceder a dados partilhados com a thread principal (main)
 ou outras.

-   O acesso aos dados deve ser controlado, utilizando, p.ex., `mutexes`
    (MUTual EXclusion)
-   Apenas um objecto deve ter acesso aos dados, usando `lock` para aceder e
    `unlock` quando termina.
-   O `mutex` nunca deve ser usado directamente, a fim de evitar deadlocks, mas
    sim através dum tipo de `std::lock`


<a id="org7c9768b"></a>

## Design *v2*: <span class="underline">Threads</span>

**Partilha de dados**: o espaço de memória das threads é partilhado frequentemente
 têm de aceder a dados partilhados com a thread principal (main) ou outras.

-   Idioma `RAII`: quando o recurso é adquirido é inicializado e é destruído
    quando sai de `scope` 
    
        void App::send_CB(const string &s)
        {
            lock_guard<mutex> u_lk(omut); // protect shared resource: output buffer
            send_buff.push( s ); /* Store string in output buffer */
        }


<a id="org1c686ad"></a>

## Design *v2*: <span class="underline">Threads</span>

**Sincronização**: É necessário frequentemente sincronizar acções entre threads,
 p.ex. uma thread produz dados e a outra consome.

-   `std::condition_variable`: Esperar por eventos cíclicos
-   `std::future`: Esperar por um evento único


<a id="org1539949"></a>

## Design *v2*: <span class="underline">Threads</span>

**Sincronização**: <span class="underline">Exemplo</span>

    void App::send_CB(const string &s){
        if(!connected) return;
        std::lock_guard<mutex> u_lk(omut); // protect shared resource: output buffer
        send_buff.push( s ); /* Store string in output buffer */
        send_data.notify_one(); /* Notify Send thread that it can send data */
    }
    
    void App::send_thread_fcn(){ 
        string s;
        while(1) { // thread runs forever
    	unique_lock<mutex> u_lk(omut); // protect shared resource: output buffer
        /* wait for send condition to be activated, with some data to process */
    	send_data.wait(u_lk, [this]{ return !send_buff.empty(); } ); 
    	s = send_buff.front( ); /* Retrieve string from output buffer */
    	send_buff.pop(); /* Remove element from queue */
    	u_lk.unlock(); /* Unlock buffer */
    	/* Further processing */
        }
    }


<a id="org8369327"></a>

## Design *v2*: <span class="underline">Threads</span>

-   os sockets são não-blocantes e são adicionados ao conjunto de file descriptors
    para serem monitorizados: `readfds` e `writefds`.
-   **3 threads que correm em background**:
    -   `accept_th`: verifica se foi aceite uma conexão pelo servidor, verificando
        se este socket `isReadable()`. Termina quando a ligação é estabelecida pelo
        cliente através do `connect` ou do `accept` através do servidor.


<a id="org7f6a87c"></a>

## Design *v2*: <span class="underline">Threads</span>

-   **3 threads que correm em background**:
    -   `send_th`: quando o utilizador tenta enviar dados, o `event callback` trata
        de fazer `push` para uma FIFO (`queue`) de mensagens a enviar. A thread
        `send_th` é notificada que há dados para enviar. Extrai os dados da FIFO e
        lança outra thread para correr em background que deve enviar os dados.
    -   `read_th`: verifica se o socket conectado `isReadable()` e faz tenta
        extrair os dados todos. Faz `push` dos dados para uma FIFO de leitura e
        lança outra thread para correr em background que deve actualizar a UI.


<a id="org144a07c"></a>

# Implementação *v2*

-   Threads
-   UI


<a id="orgca57cd3"></a>

## Implementação *v2*: <span class="underline">Threads</span>

-   Criação e desacoplamento:

    /* Create threads */
    this->accept_th = new thread(&App::accept_thread_fcn, this);
    this->send_th = new thread(&App::send_thread_fcn, this);
    this->recv_th = new thread(&App::recv_thread_fcn, this);
    
    /* Run threads in background */
    accept_th->detach();
    send_th->detach();
    recv_th->detach();


<a id="orgbd6f99d"></a>

## Implementação *v2*: <span class="underline">Threads</span>

-   Execução: Exemplo - `accept_th`:

     void App::accept_thread_fcn()
     {  int status;
        /* Connected is a shared variable; it can be set by connect call */
        while( !connected ){
    	try{
    	    status = serv->Accept();
    	}catch(runtime_error &e){
    	    DisplayWarnMsgbox("[Accept thread fcn]: ", e.what());
    	}
    	/* Server accepted a connection; signal connection and update socket */
    	if(status)
    	{
    	    unique_lock<mutex> u_lk(accept_mut); // protect shared resource
    	    connected = true;
    	    ui->connectButton->setEnabled(false);
    	    initiateDatabase("00:00:00:00:00:00"); // TODO: obtain target addr
    	    sock = serv;
    	    u_lk.unlock();
    	    break;
    	}
    	std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }


<a id="orgbc52243"></a>

## Implementação *v2*: <span class="underline">User Interface</span>

![img](doc/img/bt-chat-UI-Qt-blank.png "User Interface (QT): Device discovery")


<a id="org112c551"></a>

# Trabalho futuro *v2* <code>[0/1]</code>

-   [ ] Testar conexão enviando periodicamente mensagem de ping `KEEP_ALIVE`


<a id="orga26ea7b"></a>

# Conclusões <code>[6/6]</code>

-   [X] Funcionalidades implementadas
-   [X] Domínio da comunicação BT através do protocolo RFCOMM
-   [X] Arquitectura Cli/Serv dinâmica c/ sockets não-blocantes
-   [X] Aplicação responsiva com multithreading
-   [X] CLI e UI
-   [X] Desenvolvimento do projecto de SW
    -   [X] Uso de metodologia `Waterfall`
    -   [X] Documentação
    -   [X] Controlo de versões


<a id="org985bbe1"></a>

# Ferramentas


<a id="org883c40b"></a>

## Controlo de versões: Git

-   [Github](https://github.com/ElectroQuanta/BTChat)


<a id="org4576432"></a>

## Documentação

[Documentação do projecto](doc/html/index.html)


<a id="org2039d0f"></a>

# Referências <code>[3/3]</code>

-   [X] HUANG, Albert S.; RUDOLPH, Larry. **Bluetooth essentials for
    programmers**. Cambridge University Press, 2007.
-   [X] WILLIAMS, Anthony. **C++ Concurrency in Action-Practical
    Multithreading**, 2012.
-   [X] <span class="underline">Linux Man pages</span>: `socket`, `accept`, `bind`, `connect`, `send`, `recv`,
    etc.

