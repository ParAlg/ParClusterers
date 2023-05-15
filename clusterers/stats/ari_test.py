from sklearn.metrics.cluster import adjusted_rand_score


def test1():
  a = [0, 0, 1]
  b = [1, 1, 0]
  return adjusted_rand_score(a,b)



def test2():
  a = [0, 0, 1]
  b = [1, 0, 0]
  return adjusted_rand_score(a,b)

def test3():
  a = [1, 1, 1]
  b = [1, 0, 0]
  return adjusted_rand_score(a,b)

def test4():
  a = [1, 1, 1, 0, 0, 0, 0, 0, 0, 0]
  b = [1, 0, 0, 0, 0, 0, 1, 1, 1, 1]
  return adjusted_rand_score(a,b)



def main():
  print(test1())
  print(test2())
  print(test3())
  print(test4())
  


if __name__ == "__main__":
  main()