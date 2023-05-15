l1_cache=("512" "1024" "2048" "4096" "8192")
l2_cache=("16384" "32768" "65536" "131072" "262144")
assoc=("1" "2" "4" "8" "16")
blocksize=("8" "16" "32" "64" "128")

printf "clearing old outputs\n"
for ((j = 1; j <= 8; j++)); do
    echo -n "" > "outputs/trace${j}.txt"
done
printf "completed\n\n"

printf "script started\n\n"

for ((j = 1; j <= 8; j++)); do
    printf "trace $j started\n\n"
    printf "blocksize started\n"
    for ((i = 0; i < 5; i++)); do
        out=$(./cache_simulate ${blocksize[i]} 1024 2 65536 8 "memory_trace_files/trace${j}.txt")
        echo "(${blocksize[i]}, $out)" >> "outputs/trace${j}.txt"
    done
    printf "blocksize completed\n\n"

    echo "" >> "outputs/trace${j}.txt"

    printf "l1 cache started\n"
    for ((i = 0; i < 5; i++)); do
        out=$(./cache_simulate 64 ${l1_cache[i]} 2 65536 8 "memory_trace_files/trace${j}.txt")
        echo "(${l1_cache[i]}, $out)" >> "outputs/trace${j}.txt"
    done
    printf "l1 cache completed\n\n"

    echo "" >> "outputs/trace${j}.txt"

    printf "l1 assoc started\n"
    for ((i = 0; i < 5; i++)); do
        out=$(./cache_simulate 64 1024 ${assoc[i]} 65536 8 "memory_trace_files/trace${j}.txt")
        echo "(${assoc[i]}, $out)" >> "outputs/trace${j}.txt"
    done
    printf "l1 assoc completed\n\n"

    echo "" >> "outputs/trace${j}.txt"

    printf "l2 cache started\n"
    for ((i = 0; i < 5; i++)); do
        out=$(./cache_simulate 64 1024 2 ${l2_cache[i]} 8 "memory_trace_files/trace${j}.txt")
        echo "(${l2_cache[i]}, $out)" >> "outputs/trace${j}.txt"
    done
    printf "l2 cache completed\n\n"

    echo "" >> "outputs/trace${j}.txt"

    printf "l2 assoc started\n"
    for ((i = 0; i < 5; i++)); do
        out=$(./cache_simulate 64 1024 2 65536 ${assoc[i]} "memory_trace_files/trace${j}.txt")
        echo "(${assoc[i]}, $out)" >> "outputs/trace${j}.txt"
    done
    printf "l2 assoc completed\n\n"
    printf "trace $j completed\n\n"
done

printf "script completed\n"
