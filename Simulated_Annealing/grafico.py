# -*- encoding: utf-8 -*-

from matplotlib import pyplot as plt
import sys
import numpy as np

def main(argv):
    file = open(argv[1], "r")
    dados = []
    for line in file:
        dados.append(int(line))
    dados = np.asarray(dados)
    indices = np.zeros(len(dados), int)
    for i in range(0, len(dados)):
        indices[i] = i
    # for i in xrange(0, len(dados)):
    # plt.xlim(right = len(indices))
    # plt.yticks(np.arange(0, 1000, step=20))
    # plt.yticks(np.arange(1000, 1065, step=5))

    # plt.ylim(top = 1065)
    plt.plot(indices[::1000], dados[::1000], linestyle='-', marker='o')
    plt.show()

if __name__ == "__main__":
    main(sys.argv)
