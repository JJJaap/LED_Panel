import soundcard as sc

inputs = sc.all_microphones(include_loopback=True)

for input in inputs:
    print(input)