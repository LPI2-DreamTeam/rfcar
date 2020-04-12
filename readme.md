- [Gestão do Projecto](#orge73444c)
  - [Filesystem: Directórios](#org1f40c06)
  - [Horas do Projecto](#org1708059)
  - [Contactos](#org3bc03a6)
- [Tools](#orga475971)
  - [Team conversation](#orgc6d78b0)
  - [Diagrama de Gantt](#org0ae3751)
  - [Get Things Done](#org5437ba1)
  - [Documentation + Tracking Project](#org3bc523d)
    - [Workflow](#org9f2d0a5)
  - [Git](#orgf9b0f53)
    - [Workflow](#orge14e813)
    - [Commits' policy](#org18db6dc)
- [Project](#org66ccb6b)
  - [RFCAR - Radio Frequency Camera Assisted Rover](#org7c62af3)
    - [Motivation](#org23e6968)
    - [Description](#org74e2480)
    - [Technologies](#org19c85fa)
  - [Workgroups](#orge94468d)
  - [Meetings](#org6f3333d)
    - [2 &#x2014; Planeamento](#orgeb5c9d6)



<a id="orge73444c"></a>

# Gestão do Projecto


<a id="org1f40c06"></a>

## Filesystem: Directórios

**Estrutura do filesystem**: Cada directório deverá conter um ficheiro `rd` de base; todos os outros poderão ter outro nome, mas este será convencionado.

-   [rd-PI](rd-PI.md): contém a informação genérica essencial acerca do projecto.
-   [./sec/img/](sec/img): contém as imagens relativas aos ficheiros de documentação. Cada subdirectório deverá conter uma pasta deste tipo.
-   [./ProjManag/](ProjManag): contém os documentos essenciais à gestão do projecto: diagrama de Gantt, horas do projecto, etc.
-   [./Proj/](Proj): contém a informação relacionada com o projecto dividida pelas várias iterações. Os documentos gerados neste directório servirão de base à escrita do relatório final, sendo, por isso, mais informais e destinando-se a manter todos os grupos de trabalho actualizados.
    -   [./Proj/Iterations/<x>/Research/](Pesquisa): Deve conter todo o *knowledge base* para o projecto. É a primeira etapa do mesmo, mas deverá ser incrementado sempre que necessário. Deve conter o conceito do produto a desenvolver, quais os objectivos, a utilidade, etc. **x** representa o número da iteração.
    -   [./Proj/Iterations/<x>/Analysis/](Analise): Contém os resultados da Análise da pesquisa inicial. Deve resultar na elaboração dos requisitos e restrições ao projecto/produto. Deverá ter uma base quantitativa para iniciar o design, constituindo o conjunto de especificações preliminares do produto:
        -   p. ex.: velocidade deverá ser, no máximo, até 2 m/s.
    -   [./Proj/Iterations/<x>/Design/](Design): contém o desenvolvimento de uma, ou mais, solução(ões) para o produto. Deverá incluir toda a parte de modelação da solução, p. ex.:
        -   Design do sistema de controlo: analiticamente e recorrendo a simulação em scilab
        -   Design da sensorização do sistema: design dos circuitos e respectiva simulação
        -   Design da parte de potência: alimentação, actuação dos motores e respectiva simulação
        -   Design da parte mecânica: design do tapete
    -   [./Proj/Iterations/<x>/Implem/](Implem): contém a implementação do sistema:
        -   desenhos do controlador, sistemas de medição e de potência prontos para testes;
        -   desenhos das peças mecânicas a produzir e/ou adquirir
        -   Lista de materiais
            -   Na fase final do projeto, deverá incluir os PCBs
    -   [./Proj/Iterations/<x>/Tests](Testes): contém todos os testes realizados ao sistema e aos vários protótipos. Consideram-se testes apenas os que são realizados sobre qualquer componente ou protótipo físico.
-   [./Deliverables/](Deliverables): contém todos os elementos a entregar acerca do projecto: relatórios, desenhos finais, apresentações, etc.
    -   [prod-concept](Deliverables/prod-concept.md): conceito de produto a entregar contendo (até <span class="timestamp-wrapper"><span class="timestamp">&lt;2019-11-05 Tue&gt;</span></span>):
        -   constituição do grupo
        -   Orientador
        -   Conceito do produto a desenvolver: **Kit didáctico dum sistema de controlo de carga num tapete rolante**
-   [./Doc/](Doc): contém a info necessária para reproduzir o projecto.
-   [./writing/](readme.md): ficheiros relativos à escrita do relatório final de forma incremental.
-   [./SW/](SW): software desenvolvido para o projecto decomposto pelas várias vertentes.
-   [./HW/](HW): HW desenvolvido para o projecto decomposto pelas várias vertentes.


<a id="org1708059"></a>

## Horas do Projecto

Cada elemento do grupo deverá adicionar o nr. de horas dedicado ao projecto no ficheiro [ProjHours.xlsx](ProjManag/ProjHours.xlsx).Basta duplicar a folha inicial.


<a id="org3bc03a6"></a>

## Contactos

Aqui estão os contactos de todos os elementos do grupo. Sintam-se à vontade para editar.

| Nome           | Nr. Aluno | Email                       | Tlm        |
|-------------- |--------- |--------------------------- |---------- |
| Nuno Rodrigues | 85207     | nunorodrigues0707@gmail.com |            |
| Hugo Carvalho  | 85156     | hugo.mitab@gmail.com        |            |
| Hugo Ferreira  | 80665     | hugunited11@gmail.com       |            |
| João Faria     | 85632     | joaofaria99@gmail.com       |            |
| João Carvalho  | 83564     | jafpcarvalho44@gmail.com    |            |
| José Mendes    | 85951     | josepr.mendes@gmail.com     |            |
| José Pires     | 50178     | a50178@alunos.uminho.pt     | 911 901290 |


<a id="orga475971"></a>

# Tools


<a id="orgc6d78b0"></a>

## Team conversation

A ferramenta [Twist](https://twist.com) foi escolhida por fornecer agrupamento de conversas em tópicos


<a id="org0ae3751"></a>

## Diagrama de Gantt

Será usado o [Gantter](https://www.gantter.com/) para elaboração do diagrama de Gantt requirido pela etapa 0. Este diagrama deverá ser o mais minucioso possível e será utilizado para rastreamento e gestão do projecto.

-   Dado que é gratuito durante 30 dias, será necessário criar contas adicionais de email.
-   Permite exportar e importar ficheiros do tipo `.gantter` (ver [exemplo](sec/examples/PL2.gantter)).

O mapa de Gantt original deverá ser preservado, como constando da etapa 1, mas deverá ser actualizado ao longo do projecto.


<a id="org5437ba1"></a>

## Get Things Done

O diagrama de Gantt deverá ser usado numa macro-escala para gestão do projecto. Contudo, muitas vezes estas são ainda de granularidade elevada. Assim, cada elemento, ou sub-grupo tem liberdade de utilizar uma ferramenta para definição de tarefas mais pequenas, o que poderá ajudar à produtividade.

-   Estas subtarefas devem enquadrar-se de algum modo no diagrama de Gantt.
-   Uma ferramenta deste tipo é o [Trello](https://trello.com/).

Cada elemento do grupo deverá adicionar o nr. de horas dedicado ao projecto no ficheiro [ProjHours.xlsx](ProjManag/ProjHours.xlsx).Basta duplicar a folha inicial.


<a id="org3bc523d"></a>

## Documentation + Tracking Project

Existe um grande fluxo de informação associado ao projeto. Para gerir melhor essa informação cada grupo de trabalho deverá geral a documentação relativa aos seus tópicos, nomeadamente:

-   *código*: usando o doxygen
-   *documentação genérica*: usando o LaTeX
-   *gestão de projecto*: usando o Trello para adicionar tarefas e atribuí-las aos diferentes membros da equipa.


<a id="org9f2d0a5"></a>

### Workflow

Para a gestão da informação relacionada com o projecto será necessário adoptar algumas normas e convenções para utilização adequada e conveniente do repositório.

1.  **Filesystem**: Será criado um *filesystem*, cuja raiz é o directório actual e cujas paths devem ser relativas. Para armazenamento do *filesystem* será usada o github. Para visualizar e navegar pelo conteúdo basta replicar o *filesystem* em qq directório. O *filesystem* será navegável através dos links indicados neste readme file na secção [Directórios](#org1f40c06) deste documento.
2.  **Estrutura**:
    1.  Existirá um ficheiro readme `rd.tex` por cada directório que deve conter a informação essencial a esse tópico e permite a navegação para os restantes subtópicos através de links.
    2.  Cada pasta deve conter um directório `./sec/img/` aonde serão armazenadas as imagens relevantes para o tópico.
    3.  Este ficheiro poderá ser compilado usando o LaTeX, gerando-se o respectivo PDF que estará também disponível no mesmo directório.

**Workflow**:

1.  **Projecto**:
    -   As tarefas deverão ser adicionadas a cartões do Trello e atribuídas aos diferentes elementos da equipa, com a data prevista de entrega.
    -   Cada cartão deverá ter uma data prevista de entrega, não sendo recomendado misturar tarefas para diferentes datas.
    -   Na vista *Calendar* é possível visualizar os cartões do projecto.
2.  **Código**:
    -   O código deverá ser adicionado ao directório específico pelo grupo de trabalho indicado que deverá supervisionar a gestão deste, através dos "pushs" e "merges".
    -   Deverá ser incluído um Doxyfile para geração da documentação quando requisitada. Não será necessário atualizar toda a documentação para o repositório já que o Doxygen poderá fazê-lo mediante um ficheiro de documentação válido e os ficheiros codificados com as tags correctas.
3.  **Doc**:
    -   Criar os ficheiros `.tex` desejados, e.g., `rd.tex`.
    -   Compilar o ficheiro `.tex` e gerar o respectivo PDF.


<a id="orgf9b0f53"></a>

## Git

Git is a version control tool, providing easy management of a distributed document source between the several elements of the team.


<a id="orge14e813"></a>

### Workflow

1.  Basic

    1.  **Clone** the project
    2.  The administrator will add you as a colaborator, giving you direct push permissions.
    3.  **Pull** the last changes from remote repository before starting to work in the project and just before pushing changes to remote repository:
        -   using: `git pull <branchname>`
        -   this avoids most conflicts.
    4.  **Modify** the documents required, keeping it limited to a narrow scope: e.g., fix a bug, add a functionality, etc. Large modifications should be avoided.
    5.  **Stage** the changes: add them to tracking
    6.  **Commit** (or discard) the changes: if commited the changes will override the previous files in the local repository.
    7.  **Push** the changes: send the local repository changes to the remote repository (hosted on github) for update, using `git push <remoteName>`
        -   Conflicts can be detected if someone has modified:
            -   the remote repository (some files, but not the ones you've worked in): for this, one can just pull changes (see 2)
            -   the file(s) you've worked in: in that case, you have to pull the changes (see 2), modify the files with your changes, and commit again.

2.  Advanced

    The advanced workflow increments the basic one, by isolating the core source of the project (current version fully operational) from the addition of new functionalities or from code refinement.
    
    For this purpose, a branch should be created with a descriptive name from master remote (preferably) and the functionality added.
    
    **Workflow (advanced**:
    
    1.  **Branch** from master branch remote: `git branch <branchName> <remoteName>`
    2.  **Modify** the required files
    3.  **merge** the branch again to master remote, effectively adding the functionalities to the core.: `git merge <branchName> <remoteName>`


<a id="org18db6dc"></a>

### Commits' policy

The commits made first to local repository and then to the remote repository should contain relevant information about the changes/deletions being made, namely the reason behind them and what has been changed.

For this purpose, a set of thumb rules and tags are used to enforce this policy as follows:

-   `ADD: <what> to <where> and <why>`: addition of a new file, a new functionality, a new snippet of code
-   `FIX: <what>, <where> and <why>`: fixing a bug, duplicate files, etc. fall in this category
-   `UPDT: <what>, <where> and <why>`: updating the contents of a file, the file structure, etc., follow in this category
-   `RM: <what> to <where> and <why>`: removing of a file

**Example**: `ADD: Git Workflow and Commit Policies to add consistency`


<a id="org66ccb6b"></a>

# Project


<a id="org7c62af3"></a>

## RFCAR - Radio Frequency Camera Assisted Rover


<a id="org23e6968"></a>

### Motivation

This project is being developed in the scope of the integrator project of LPI2. It aims to develop skills in the software engineering area and digital design, wireless communication protocols, odometric vehicle navigation, etc.


<a id="org74e2480"></a>

### Description

The project consists of a remote controlled car used to assist exploration and maintenance domains. For this purpose, the vehicle should contain a remotely operated camera feeding back video to the user. Additionally, the vehicle must contain odometric sensors to assist in driving and prevent crashes when user is not in control, e.g., when connection is lost. The vehicle can be used for exploration of unaccessible areas to human operators like fluid pipelines and other hazardous sites.


<a id="org19c85fa"></a>

### Technologies

-   STM32: vehicle's low level control
-   Raspberry Pi: camera interface
-   Android: Human Machine Interface
-   Others (yet to be defined):
    -   Wi-Fi
    -   Bluetooth
    -   GPS
    -   GPRS


<a id="orge94468d"></a>

## Workgroups

1.  **HW Control**: Nuno (50%), Alex (50%), Hugo F. (50%)
    -   Car chassis
    -   odometric sensors: ultrasonic + InfraRed
    -   motors
    -   buzzer
    -   lights
    -   battery
    -   GPS
2.  **Smartphone**: HMI - Hugo C. (80%), João Faria (100%), Zé Mendes (50%)
    -   Accelerometers
    -   GPS
    -   Wi-Fi/ RF
    -   GPRS
    -   Display
3.  **STM32**: Low-level SW layer control: Zé Mendes (50%), Nuno (50%), Alex (50%), Hugo F. (50%),
    -   Control of car's HW
    -   USART interface with HW controlling a camera
4.  **Camera + control HW**: e.g. Raspberry Pi - Zé Pires(100%), Hugo C. (20%)
    -   Raspberry Pi Zero: runs Linux OS
    -   Camera
5.  **PCBs**: Hugo Carvalho, Hugo Ferreira


<a id="org6f3333d"></a>

## Meetings


<a id="orgeb5c9d6"></a>

### 2 &#x2014; Planeamento

<span class="timestamp-wrapper"><span class="timestamp">&lt;2019-11-12 Tue 14:15&gt;</span></span>

1.  Tópicos

    1.  Ferramentas de gestão do projecto: Typora, Pandoc, Excel (horas)
    2.  Análise da pesquisa preliminar sobre tapetes rolantes: treadmills
    3.  Definição dos componentes básicos para a concepção duma treadmill
        -   **Mecânica**: correia (belt), rolos (eixos)
        -   **Electrónica**:
            -   Alimentação:
                -   Baterias vs Fontes de Alimentação comutadas
        -   **Actuação**: motor DC e circuito de potência
        -   **Controlo**: controlador analógico
        -   **Sensores**: sensor de corrente para inferir carga a que o motor está sujeito
    4.  Planeamento preliminar (dependente da aprovação inicial do tutor): Divisão do projecto em várias etapas:
        1.  **Pesquisa** &#x2014; *State of the art of treadmills*:
            -   Princípio de funcionamento
            -   Identificação dos principais componentes do sistema e as suas características
        2.  **Análise**: Contém os resultados da Análise da pesquisa inicial. Deve resultar na elaboração dos requisitos e restrições ao projecto/produto, Deverá ter uma base quantitativa para iniciar o design, constituindo o conjunto de especificações preliminares do produto: - p. ex.: velocidade deverá ser, no máximo, até 2 m/s.
            1.  Iteração 1: 3 dias <span class="timestamp-wrapper"><span class="timestamp">&lt;2019-11-14 Thu&gt;</span></span>
            2.  Iteração 2: 3 dias
        3.  [./Design/](Design): Pode ser dividido em **design conceptual** e **design da solução**.
            -   No design conceptual, são identificadas as várias soluções possíveis para o problema, sendo quantificada a sua relevância para o projecto através duma escala, inserida numa matriz de avaliação, p.ex., [QFD](https://en.wikipedia.org/wiki/Quality_function_deployment).
            -   Design da solução: contém o desenvolvimento da solução identificada para o produto. Deverá incluir toda a parte de modelação da solução, p. ex.:
                -   Design do sistema de controlo: analiticamente e recorrendo a simulação em scilab
                -   Design da sensorização do sistema: design dos circuitos e respectiva simulação
                -   Design da parte de potência: alimentação, actuação dos motores e respectiva simulação
                -   Design da parte mecânica: design do tapete
            -   Iteração 1: 2 semanas
            -   Iteração 2: 1 semana
        4.  [./Implem/](Implem): será feita em duas partes: <span class="underline">por módulos</span> e <span class="underline">integrada</span>. Contém a implementação do sistema:
            -   desenhos do controlador, sistemas de medição e de potência prontos para testes;
            -   desenhos das peças mecânicas a produzir e/ou adquirir
            -   Lista de materiais
                -   Na fase final do projeto, deverá incluir os PCBs
            -   Iteração 1: 2 semanas
            -   Iteração 2: 1 semana
        5.  [./Testes/](Testes): será feito em duas partes: <span class="underline">por módulos</span> e <span class="underline">total</span>. contém todos os testes realizados ao sistema e aos vários protótipos. Consideram-se testes apenas os que são realizados sobre qualquer componente ou protótipo físico.
            -   Iteração 1: 2 semanas
            -   Iteração 2: 1 semana
        6.  **Verificação/Validação**: (3 dias) &#x2014; as especificações listadas na Análise devem ser verificadas e o protótipo validado por um agente externo (utilizador externo ao grupo).
        7.  **Entrega**: (2 semanas) &#x2014; término do projeto com:
            1.  Protótipo final produzido, verificado e validado
            2.  Documentação de suporte: como replicar, manual de instruções
            3.  Relatório Final: <span class="timestamp-wrapper"><span class="timestamp">&lt;2020-01-30 Thu&gt;</span></span>
            4.  Apresentação Pública: <span class="timestamp-wrapper"><span class="timestamp">&lt;2020-01-31 Fri&gt;</span></span>
    5.  Definição das equipas de projeto pelas áreas identificadas (a **negrito** encontram-se os coordenadores de cada departamento):
        -   <span class="underline">Mecânica</span>: **José Pires**
        -   <span class="underline">Human Machine Interface (HMI) & Sensores</span>: **Hugo Carvalho**, Hugo Ferreira
        -   <span class="underline">Actuação</span>: **João Faria**, Nuno Rodrigues
        -   <span class="underline">Controlo</span>: **João Carvalho**, Nuno Rodrigues
            -   Estas equipas poderão sofrer alterações conforme o projecto assim o exija.
