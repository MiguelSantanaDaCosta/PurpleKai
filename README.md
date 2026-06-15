# PurpleKai - Linguagem de Programação em Francês (v0.1.0)

**PurpleKai** é uma linguagem de programação interpretada, com tipagem dinâmica e sintaxe em francês.  
Na versão **0.1.0**, a linguagem já é **Turing complete** e suporta variáveis, operadores aritméticos e lógicos, condicionais, loops, `break`, `continue`, comentários e execução de scripts. [web:59][web:62]

> ⚠️ **Nota**: Esta versão é um **MVP funcional**. Recursos como funções, classes, tratamento de erros e módulos ainda não estão implementados.

---

## Funcionalidades implementadas

- Variáveis mutáveis (`laisser`) e constantes (`constant`).
- Operadores aritméticos: `+`, `-`, `*`, `/`, `%`.
- Operadores de comparação: `==`, `!=`, `<`, `<=`, `>`, `>=`.
- Operadores lógicos: `et`, `ou`, `non`.
- Condicionais: `si` / `sinon`.
- Loops: `tantque` e `pour`.
- Controle de fluxo com `break` e `continue`.
- Comentários de linha com `--`.
- Concatenação mista entre texto e número.
- REPL interativo e execução de arquivos `.pk`.
- Tipos básicos: inteiro, decimal, string, booleano e nulo. [web:59][web:62]

---

## Exemplo funcional

```ruby
laisser a = 10;
laisser b = 20;
laisser soma = a + b;

"Resultado: " + soma;

si (soma > 25) {
    "Maior que 25";
} sinon {
    "Menor ou igual a 25";
}

pour (laisser i = 0; i < 3; i = i + 1) {
    "Valor de i: " + i;
}
```

---

## Como executar

```bash
make clean
make
./purplekai
./purplekai script.pk
```

---

## Especificação da linguagem

### Palavras-chave

| Francês | Equivalente | Função |
|---|---|---|
| `laisser` | variável | declara variável mutável |
| `constant` | constante | declara valor imutável |
| `si` | `if` | condição principal |
| `sinon` | `else` | bloco alternativo |
| `tantque` | `while` | loop condicional |
| `pour` | `for` | loop com inicialização, condição e incremento |
| `break` | `break` | interrompe o loop |
| `continue` | `continue` | pula para a próxima iteração |
| `vrai` | `true` | valor booleano verdadeiro |
| `faux` | `false` | valor booleano falso |
| `nul` | `null` | valor nulo |

### Operadores

- Aritméticos: `+ - * / %`
- Comparação: `== != < <= > >=`
- Lógicos: `et`, `ou`, `non`

### Tipos de dados

- Inteiro.
- Decimal.
- String.
- Booleano.
- Nulo.

### Comentários

Comentários de linha usam `--` e vão até o fim da linha.

```ruby
laisser x = 10;  -- comentário
```

---

## Limitações da versão 0.1.0

- Não há funções definidas pelo usuário.
- Não há orientação a objetos.
- Não há classes, objetos, herança ou polimorfismo.
- Não há tratamento de exceções.
- Não há módulos nem biblioteca padrão.
- Comentários podem ter limitações dependendo da posição no código.
- `break` e `continue` funcionam apenas em contextos suportados pelo interpretador atual.

---

## Próximas versões

- **v0.2.0**: funções e escopo léxico.
- **v0.3.0**: classes e objetos.
- **v0.4.0**: tratamento de erros e módulos.
- **v0.5.0**: melhorias de performance e novos recursos de linguagem.

---

## Documento de especificação

Este repositório também inclui uma especificação realista da versão atual da linguagem, descrevendo apenas o que já funciona hoje.  
O documento pode ser convertido para PDF com:

```bash
pandoc PurpleKai-spec-v0.1.0.md -o PurpleKai.pdf
```

---

## Licença

Distribuído sob licença MIT.

---

## Agradecimentos

Total inspiração a **Kailane Damasceno da Silva**, minha melhor amiga e minha chama.
