

# ParHAC resolution parameters
a = 0.5
num_terms = 10  # adjust this as needed
sequence = [a**i for i in range(num_terms)]
print('; '.join(map(str, sequence)))

