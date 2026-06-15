#!/usr/bin/env python3
conteudo = """laisser a = 10;
laisser b = 20;
laisser soma = a + b;
"Resultado: " + soma;

si (soma > 25) {
    "Maior que 25";
} sinon {
    "Menor ou igual a 25";
}

"--- Loop pour ---";
pour (laisser i = 0; i < 3; i = i + 1) {
    "Valor de i: " + i;
}

"--- Loop tantque ---";
laisser x = 3;
tantque (x > 0) {
    "x = " + x;
    x = x - 1;
}

"--- Break ---";
pour (laisser i = 0; i < 5; i = i + 1) {
    si (i == 3) { break; }
    i;
}

"--- Continue ---";
pour (laisser i = 0; i < 5; i = i + 1) {
    si (i == 2) { continue; }
    i;
}

"--- Concatenação mista ---";
laisser prefixo = "Número: ";
prefixo + 42;

"--- Escopo ---";
laisser global = 10;
{
    laisser local = 20;
    "dentro: " + local;
}
"fora: " + global;

constant PI = 3.14;
PI;

"--- Expressões ---";
(10 + 5) * 2 - 8 / 4;
(5 > 3) et (10 < 20) ou non faux;

"--- Fatorial de 5 ---";
laisser n = 5;
laisser fat = 1;
tantque (n > 0) {
    fat = fat * n;
    n = n - 1;
}
"Fatorial: " + fat;

"--- Todos os testes concluídos com sucesso! ---"
"""

with open("teste_completo.pk", "w") as f:
    f.write(conteudo)
print("Arquivo 'teste_completo.pk' recriado (sem comentários e com ; garantidos).")
