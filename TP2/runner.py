import subprocess


def write(process, message):
    process.stdin.write(f"{message.strip()}\n".encode("utf-8"))
    process.stdin.flush()


def process(problem: str) -> str:
    data_file_path = f"./{problem}/data.csv"
    # create empty data file
    with open(data_file_path, "w") as data_file:
        data_file.write("")
        data_file.close()

    compile = subprocess.call(
        f"g++ ./{problem}/{problem}.cpp ../lib/MyLib.* -o ./{problem}/{problem} -lpthread",
        shell=True,
    )

    if compile != 0:
        raise RuntimeError(f"Couldnt compile {problem} file")

    posibilities = {
        'producer_consumer': run_producer_consumer,
        'spin_lock': run_spin_lock
    }
    with open(data_file_path, "a") as data_file:
        posibilities[problem](data_file)

    return data_file_path


def run_producer_consumer(data_file) -> None:
    n_values = [1, 2, 4, 8, 16, 32]
    threads_combinations = [(1, 1), (1, 2), (1, 4), (1, 8), (1, 16),
                            (2, 1),(4, 1), (8, 1), (16, 1)]
    for n_value in n_values:
        for thread_combination in threads_combinations:
            for _ in range(10):
                with subprocess.Popen("./producer_consumer/producer_consumer",
                                      stdin=subprocess.PIPE, stdout=data_file) as process:
                    write(process, f"{n_value} {thread_combination[0]} {thread_combination[1]}")
                    ret_code = process.wait()
                    print("producer_consumer ran successfully with parameters N = {}, Np = {}, Nc = {} and return code = {}" \
                        .format(n_value, thread_combination[0], thread_combination[1], ret_code))


def run_spin_lock(data_file) -> None:
    n_values = [10**7, 10**8, 10**9]
    threads_qts = [1, 2, 4, 8, 16, 32, 64, 128, 256]
    for n_value in n_values:
        for thread_qt in threads_qts:
            for _ in range(10):
                with subprocess.Popen("./spin_lock/spin_lock",
                                      stdin=subprocess.PIPE, stdout=data_file) as process:
                    write(process, f"{thread_qt} {n_value}")
                    ret_code = process.wait()
                    print("spin_lock ran successfully with parameters K = {}, N = {} and return code = {}" \
                        .format(thread_qt, n_value, ret_code))


if __name__ == "__main__":
    process(problem="producer_consumer")
    process(problem="spin_lock")