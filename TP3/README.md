## Para rodar os estudos de caso

Em uma janela do terminal:

```shell
make coordinator # para compilar o coordenador
make client # para compilar o cliente

sh ./coordinator # rodando o coordenador, os testes necessitam que ele esteja rodando previamente
```

Em outra janela:

```shell
sh test_0.sh
sh test_1.sh
sh test_2.sh
sh test_3.sh
```

## Para rodar as validações dos estudos de caso

```shell
python validator.py
```
