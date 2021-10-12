import pandas as pd
import glob
import numpy as np
tests_configs = [
    {
        "n": [2],
        "k": 2,
        "r": 10
    },
    {
        "n": [2, 4, 8, 16, 32],
        "k": 2,
        "r": 10
    },
    {
        "n": [2, 4, 8, 16, 32, 64],
        "k": 1,
        "r": 5
    },
    {
        "n": [2, 4, 8, 16, 32, 64, 128],
        "k": 0,
        "r": 3
    },
]


def get_dataframe(path):
    df = None

    if path == './test_0':
        df = pd.read_csv(f"{path}/resultados.txt", sep="|",
                         header=None, index_col=None)
        df['n'] = 2
    else:
        files = glob.glob(f"{path}/resultados/*.txt")
        for file in files:
            df_aux = pd.read_csv(file, sep="|", header=None, index_col=None)
            file_number = int(file.split('resultados/')
                              [-1].replace('.txt', ''))
            df_aux['n'] = file_number
            if df is None:
                df = df_aux
            else:
                df = pd.concat([df, df_aux])

    df.columns = ["time", "pid", "n"]
    return df


def validate_number_of_lines(test_id, df, n):
    expected_number_of_lines = tests_configs[test_id]['r'] * n
    if df.shape[0] == expected_number_of_lines:
        print(
            f"Número de linhas do teste {test_id}, com n = {n} foi correto e igual a {expected_number_of_lines}.")
    else:
        print(
            f"Número de linhas do teste {test_id}, com n = {n} foi errôneo e igual a {df.shape[0]}, não {expected_number_of_lines}.")


def validate_number_of_writes(test_id, df, n):
    expected_number_of_writes = tests_configs[test_id]['r']
    df_writes_count = df.groupby('pid').count()
    as_numpy = df_writes_count.to_numpy()
    number_of_lines = df_writes_count.shape[0]
    number_of_writes = as_numpy[0, 0]
    filtered_df = df_writes_count[df_writes_count['time'] == number_of_writes]
    if filtered_df.shape[0] != number_of_lines:
        print(
            f"No teste {test_id}, com n = {n}, existem processos que escreveram um número diferente de vezes.")
        return

    if number_of_writes == expected_number_of_writes:
        print(
            f"Número de escritas do teste {test_id}, com n = {n} foi correto e igual a {expected_number_of_writes}.")
    else:
        print(
            f"Número de escritas do teste {test_id}, com n = {n} foi errôneo e igual a {number_of_writes}, não {expected_number_of_writes}.")


def validate_increasing(path):
    file = open(path)
    lines = file.readlines()
    last = -1
    for l in lines:
        new = int(l.split('|')[0])
        if new < last:
            return False
        last = new
    return True


def validate_execution(path):
    file = open(path)
    lines = file.readlines()[1:]
    requests = []
    last_granted = 0
    for l in lines:
        splt = l.split(' ')
        if splt[1] == 'GRANT\n':
            last_granted = int(splt[0])
        elif splt[1] == 'REQUEST\n':
            requests.append(int(splt[0]))
        else:
            if int(splt[0]) != requests[0] or int(splt[0]) != last_granted:
                breakpoint()
                return False
            requests = requests[1:]
    return True


def validate_test(test_id):
    path = f"./test_{test_id}"
    df = get_dataframe(path)
    for n in tests_configs[test_id]['n']:
        df_aux = df[df['n'] == n]
        validate_number_of_lines(test_id, df_aux, n)
        validate_number_of_writes(test_id, df_aux, n)
        if test_id == 0:
            print('test:', test_id, validate_increasing('test_0/resultados.txt'))
        else:
            print('test:', test_id, 'n:', n, validate_increasing(
                f"test_{test_id}/resultados/{n}.txt"))


if __name__ == '__main__':
    tests = list(range(0, 4))
    print('coord:', validate_execution('coord.txt'))

    for test in tests:
        validate_test(test)
