# 🧮 Simulador de Arquitetura Acadêmica — POXIM

Este projeto implementa um **simulador de arquitetura de computadores** desenvolvido para fins acadêmicos.
O simulador lê um arquivo de entrada contendo instruções em formato **hexadecimal** (representando o código de máquina da arquitetura POXIM) e gera um arquivo de saída descrevendo a execução de cada instrução.

---

## 🎯 Objetivo

Compreender o funcionamento de uma arquitetura de processador simplificada, abordando:

* Decodificação e execução de instruções
* Operações aritméticas e lógicas
* Manipulação de registradores e memória
* Controle de fluxo (branching)
* Atualização do registrador de status (SR) com flags

---

## 🗂️ Estrutura do Projeto

```
├── arquitetura_poxim.c   # Código-fonte principal em C
├── poxim_input.txt                       # Arquivo de entrada com instruções em hexadecimal
└── (gerado após execução) poxim_output.txt  # Arquivo de saída com o resultado da simulação
```

---

## ⚙️ Como Executar

### 1️⃣ Compilar o programa

```bash
gcc arquitetura_poxim.c -o poxim1
```

### 2️⃣ Executar o simulador

Por padrão, o código busca os arquivos `poxim1_input.txt` e cria `poxim1_output.txt` automaticamente.

---

## 📥 Exemplo de Arquivo de Entrada

O simulador utiliza um arquivo `.txt` contendo instruções em **formato hexadecimal**, representando o código de máquina da arquitetura POXIM.

O arquivo de exemplo incluso no repositório é:

```
poxim1_input.txt
```

Trecho ilustrativo do conteúdo:

```text
0x1015AB0A
0x4E350001
0x0EF11000
0x7C000000
0x01BFFFFF
...
```

Cada linha representa uma **instrução de 32 bits** que será decodificada e executada pelo simulador.
Durante a execução, o programa lê esse arquivo, processa cada instrução e gera o arquivo `poxim1_output.txt` com o resultado detalhado da simulação.

---

## 🧾 Exemplo de Saída

O simulador gera o arquivo `poxim1.output.txt` contendo a execução detalhada, linha a linha, com o valor dos registradores e o estado dos flags.

```
[START OF SIMULATION]
0x00000000:  mov R1,10         R1=0x0000000A
0x00000004:  add R2,R1,R3      R2=R1+R3=0x00000014, SR=0x00000000
...
```

---

## 💻 Funcionalidades Implementadas

* **32 registradores (R0–R31)**
* **Memória de 32 KiB**
* Instruções:

  * **Aritméticas:** `add`, `sub`, `muli`, `divi`, `modi`
  * **Lógicas:** `and`, `or`, `xor`, `not`
  * **Comparação:** `cmp`, `cmpi`
  * **Transferência:** `mov`, `movs`
  * **Acesso à memória:** `l8`, `l16`, `l32`, `s8`, `s16`, `s32`
  * **Controle de fluxo:** `beq`, `bge`, `bgt`, `bbe`, `bbt`, `bae`, `bat`
* Atualização do registrador **SR (Status Register)** com flags:

  * `CY` → Carry
  * `OV` → Overflow
  * `SN` → Sinal
  * `ZD` / `ZN` → Zero detect / Zero not
  * Verificação de **divisão por zero**

---

## 🧠 Conceitos Envolvidos

* Estrutura e simulação de uma **arquitetura de processador**
* Modelagem do **ciclo de busca, decodificação e execução**
* Manipulação de **instruções em hexadecimal**
* Implementação de **flags e status register**
* Simulação de memória e registradores em **baixo nível**

---

## 👨‍💻 Autor

**Henrick Cardoso**

Projeto desenvolvido na disciplina de **Arquitetura de Computadores** — 2024.

---
