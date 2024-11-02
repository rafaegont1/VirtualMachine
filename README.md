# Virtual Machine

Este repositório apresenta uma máquina virtual de Von neumann baseada na arquitetura MIPS, um microprocessador padrão da indústria utilizado em eletrônicos de consumo, em algumas estações de trabalho Silicon Graphics e em muitos sistemas de comunicação, como televisores, hardware de rede e links sem fio. A arquitetura MIPS é definida por seus registradores e pelo conjunto de instruções em linguagem de montagem.

> [!CAUTION]
> Este repositório é um trabalho em progresso (WIP) e, portanto, poderam ser realizadas algumas modificações posteriormente para adequar a máquina virtual aos requisitos do trabalho da disciplina de Sistemas Operacionais.

## Pipeline

O processador MIPS desta máquina virtual apresenta cinco estágios:

* Instruction Fetch (IF): busca a instrução na memória a partir do Program Counter (PC);
* Instruction Decode (ID): decodifica as instruções para branchs, jumps, operações na ULA, etc.;
* Execute (EX): executa as instruções decodificadas;
* Memory Access (MEM): busca dados da memória principal;
* Write Back (WB): escreve dados na memória principal;

> [!NOTE]
> Pode ser que, como dito anteriormente, o funcionamento da máquina virtual mude, podendo alterar a pipeline de modo que, por exemplo, a leitura e escrita de dados na memória seja executada em MEM e a escrita do banco de registradores seja realizada no WB, para ficar mais próximo à arquitetura MIPS.

## Conjunto de instruções

Com isso, foi implementado um subconjunto de instruções da arquitetura MIPS, além de algumas outras instruções para tornar a linguagem de montagem o mais simples possível. Segue abaixo as instruções implementadas:

* `nop`: "no operation", representa a ausência de uma instrução na pipeline;
* `add`, `sub`, `mult`, `div`: instruções realizadas na ULA da forma (_add/sub/mult/div rd rs rt_) onde _rd_ é o registrador de destino do resultado da operação e _rs_ e _rt_ são os registradores dos operandos;
* `li`: "load immediate", instrução da forma (_li rd imm_) carrega a constante _imm_ no registrador _rd_;
* `inc`, `dec`: "increment" e "decrement", instruções da forma (_inc/dec rd_), onde _rd_ é o registrador de destino;
* `jump`: instrução da forma (_jump imm_), que faz com que o PC vá para a instrução da linha _imm_;
* `beq`, `bnq`: "branch on equal" e "branch on not equal", instrução da forma (_beq/bnq rs rt imm_) que compara _rs_ e _rt_ e, se a condição for verdadeira, pula para a linha _imm_;
* `bgez`, `bgtz`, `blez`, `bltz`: "branch on greater or equal to zero", etc., instruções da forma (_bgez/bgtz/blez/bltz rs imm_) que compara _rs_ a zero e pulam para a linha _imm_ se a condição for verdadeira;
* `load`, `store`: instruções das formas (_load rd var_) e (_store var rs_), onde _var_ é a variável na memória, _rd_ o registrador de destino e _rs_ o registrador de origem, que leem e escrevem na memória principal a partir dos registradores.

## Exemplo

O programa a seguir carrega o número 10 no registrador $t3, o decrementa e verifica se ele é maior que zero; se for verdade, ele volta para a linha dois, criando um loop de 10 repetições no programa.

```
1  li   $t3 10
2  dec  $t3
3  bgtz $t3 2
```

Após a execução do programa, será gerado um arquivo chamado log.txt. Nele, é possível ver as instruções percorrendo a pipeline. Recomenda-se lê-lo utilizando espaçamento de 8 para tabulação.

```
CLOCK: 1

GENERAL-PURPOSE REGISTERS
$zero: 0	$at: 101	$v0: 102	$v1: 103
$a0: 104	$a1: 105	$a2: 106	$a3: 107
$t0: 108	$t1: 109	$t2: 110	$t3: 111
$t4: 112	$t5: 113	$t6: 114	$t7: 115
$s0: 116	$s1: 117	$s2: 118	$s3: 119
$s4: 120	$s5: 121	$s6: 122	$s7: 123
$t8: 124	$t9: 125	$k0: 126	$k1: 127
$gp: 128	$sp: 129	$fp: 130	$ra: 131

PIPELINE
IF:  add  $t3  $t1  $t2
ID:  nop
EX:  nop
MEM: nop
WB:  nop

MEMORY DATA
empty
==============================================================
[...]
```

## Compilação e Execução

A máquina virtual disponibilizada possui um arquivo CMake que realiza todo o procedimento de compilação e execução. Para tanto, temos as seguintes diretrizes de execução:

> [!IMPORTANT]
> Os programas estão salvos na pasta proc, e devem ser utilizadas como argumento do executável da máquina virtual

| Comando                    | Função                                                                                           |
| -------------------------- | ------------------------------------------------------------------------------------------------ |
| `mkdir build`              | Cria uma nova pasta chamada `build` para a compilação.                                           |
| `cd build`                 | Navega até a pasta `build`.                                                                      |
| `cmake ..`                 | Configura o projeto utilizando CMake, buscando o `CMakeLists.txt` no diretório anterior.         |
| `make`                     | Executa a compilação do programa utilizando o GCC, com o resultado na pasta `build`.             |
| `./vm ../proc/loop.txt`    | Executa o programa `loop.txt` na máquina virtual.                                                |
