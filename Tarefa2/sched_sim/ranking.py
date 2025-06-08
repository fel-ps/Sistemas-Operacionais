import pandas as pd

arquivos_e_escalonadores = [
    ("resultados_fifo.txt", "FIFO"),
    ("resultados_sjf.txt", "SJF"),
    ("resultados_ljf.txt", "LJF"),
    ("resultados_prio_static.txt", "PRIO_STATIC"),
    ("resultados_prio_dynamic.txt", "PRIO_DYNAMIC"),
    ("resultados_prio_dynamic_quantum.txt", "PRIO_DYNAMIC_QUANTUM"),
]

resultados = []

for nome_arquivo, nome_escalonador in arquivos_e_escalonadores:
    dados = []
    with open(nome_arquivo, "r") as f:
        for linha in f:
            if "TME:" in linha:
                partes = linha.strip().split()
                tme = float(partes[1])
                n_procs = int(partes[2])
                dados.append((n_procs, tme))

    df = pd.DataFrame(dados, columns=["n_procs", "tme"])
    media_geral = df["tme"].mean()
    resultados.append((nome_escalonador, media_geral))

# Ordena do menor para o maior TME
ranking = sorted(resultados, key=lambda x: x[1])

# Exibe o ranking
print("Ranking dos algoritmos por menor TME médio geral:\n")
for pos, (nome, media) in enumerate(ranking, 1):
    print(f"{pos}. {nome}: {media:.2f} ms")
