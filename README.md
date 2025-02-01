# Virtual Machine

Este repositório apresenta uma máquina virtual de Von neumann baseada na arquitetura MIPS, um microprocessador padrão da indústria utilizado em eletrônicos de consumo, em algumas estações de trabalho Silicon Graphics e em muitos sistemas de comunicação, como televisores, hardware de rede e links sem fio. A arquitetura MIPS é definida por seus registradores e pelo conjunto de instruções em linguagem de montagem.

> [!NOTE]
> Este repositório é um trabalho em progresso (WIP) e, portanto, poderam ser realizadas algumas modificações posteriormente para adequar a máquina virtual aos requisitos do trabalho da disciplina de Sistemas Operacionais.

## Pipeline

O processador MIPS desta máquina virtual apresenta cinco estágios:

* Instruction Fetch (IF): busca a instrução na memória a partir do Program Counter (PC);
* Instruction Decode (ID): decodifica as instruções para branchs, jumps, operações na ULA, etc.;
* Execute (EX): executa as instruções decodificadas;
* Memory Access (MEM): busca ou escreve dados na memória principal;
* Write Back (WB): verifica se o programa terminou.

## Conjunto de instruções

Com isso, foi implementado um subconjunto de instruções da arquitetura MIPS, além de algumas outras instruções para tornar a linguagem de montagem o mais simples possível. Segue abaixo as instruções implementadas:

| Instrução | Descrição                                                                                                            |
|-----------|----------------------------------------------------------------------------------------------------------------------|
| `nop`     | "No operation", representa a ausência de uma instrução na pipeline.                                                  |
| `add`     | Realiza adição: _add rd rs rt_, onde _rd_ é o registrador de destino e _rs_, _rt_ são os operandos.                  |
| `sub`     | Realiza subtração: _sub rd rs rt_, com a mesma lógica do `add`.                                                      |
| `mult`    | Realiza multiplicação: _mult rd rs rt_.                                                                              |
| `div`     | Realiza divisão: _div rd rs rt_.                                                                                     |
| `addi`    | Realiza adição: _addi rd imm1 imm2_, onde _rd_ é o registrador de destino e _imm1_, _imm2_ são os valores imediatos. |
| `subi`    | Realiza subtração: _subi rd imm1 imm2_, com a mesma lógica do `add`.                                                 |
| `multi`   | Realiza multiplicação: _multi rd imm1 imm2_.                                                                         |
| `divi`    | Realiza divisão: _divi rd imm1 imm2_.                                                                                |
| `li`      | "Load immediate", carrega uma constante: _li rd imm_, onde _imm_ é carregado no registrador _rd_.                    |
| `inc`     | Incrementa: _inc rd_, onde _rd_ é o registrador a ser incrementado.                                                  |
| `dec`     | Decrementa: _dec rd_, onde _rd_ é o registrador a ser decrementado.                                                  |
| `jump`    | Salta para uma instrução específica: _jump imm_, onde _imm_ indica a linha da próxima instrução.                     |
| `beq`     | "Branch on equal": _beq rs rt imm_, salta para _imm_ se _rs_ for igual a _rt_.                                       |
| `bnq`     | "Branch on not equal": _bnq rs rt imm_, salta para _imm_ se _rs_ for diferente de _rt_.                              |
| `bgez`    | "Branch on greater or equal to zero": _bgez rs imm_, salta para _imm_ se _rs_ >= 0.                                  |
| `bgtz`    | "Branch on greater than zero": _bgtz rs imm_, salta para _imm_ se _rs_ > 0.                                          |
| `blez`    | "Branch on less or equal to zero": _blez rs imm_, salta para _imm_ se _rs_ <= 0.                                     |
| `bltz`    | "Branch on less than zero": _bltz rs imm_, salta para _imm_ se _rs_ < 0.                                             |
| `lw`      | Lê da memória: _load rd var_, onde _var_ é uma variável e _rd_ é o registrador de destino.                           |
| `sw`      | Escreve na memória: _store var rs_, onde _var_ é a variável e _rs_ é o registrador de origem.                        |
| `halt`    | Termina a execução do programa.                                                                                      |

## Exemplo

O programa a seguir carrega o número 10 no registrador $t3, o decrementa e verifica se ele é maior que zero; se for verdade, ele volta para a linha dois, criando um loop de 10 repetições no programa.

```
1  li   $t3 10
2  dec  $t3
3  bgtz $t3 2
4  halt
```

Após a execução do programa, será gerado um arquivo chamado `pid_1_output.txt`. Nele, é possível ver as instruções percorrendo a pipeline. Recomenda-se lê-lo utilizando espaçamento de 8 para tabulação.

```
CPU ID: 0
CLOCK: 1
Timestamp: 2.27827 ms

GENERAL-PURPOSE REGISTERS
$zero: 0        $at: 101        $v0: 102        $v1: 103
$a0: 104        $a1: 105        $a2: 106        $a3: 107
$t0: 108        $t1: 109        $t2: 110        $t3: 111
$t4: 112        $t5: 113        $t6: 114        $t7: 115
$s0: 116        $s1: 117        $s2: 118        $s3: 119
$s4: 120        $s5: 121        $s6: 122        $s7: 123
$t8: 124        $t9: 125        $k0: 126        $k1: 127
$gp: 128        $sp: 129        $fp: 130        $ra: 131

PIPELINE
IF:     li      $t3     10
ID:     nop
EX:     nop
MEM:    nop
WB:     nop

MEMORY DATA
empty
==============================================================
[...]
```

***Obs.***: O timestamp é inicado no construtor do Minix.

## Compilação e Execução

A máquina virtual disponibilizada possui um arquivo CMake que realiza todo o procedimento de compilação e execução. Para tanto, temos as seguintes diretrizes de execução:

> [!IMPORTANT]
> Os programas estão salvos na pasta `example`, e devem ser utilizadas como argumento do executável da máquina virtual

| Comando                                                                                                                           | Função                                                                                           |
| --------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------ |
| `mkdir build`                                                                                                                     | Cria uma nova pasta chamada `build` para a compilação.                                           |
| `cd build`                                                                                                                        | Navega até a pasta `build`.                                                                      |
| `cmake ..`                                                                                                                        | Configura o projeto utilizando CMake, buscando o `CMakeLists.txt` no diretório anterior.         |
| `make`                                                                                                                            | Executa a compilação do programa utilizando o GCC, com o resultado na pasta `build`.             |
| `./minix ../examples/loop100.txt ../examples/loop100.txt ../examples/loop100.txt ../examples/loop100.txt ../examples/loop100.txt` | Executa os programas `add_and_store.txt` e `if_else` na máquina virtual.                         |
