# -*- encoding: utf-8 -*-

from matplotlib import pyplot as plt
import sys
import numpy as np

def main(argv):
    execucoes = []
    medias = []
    indices = 0
    for e in xrange(0, 10):

        with open(argv[1] + "_" + str(e) + ".txt", "r") as f:
            lines = f.read().splitlines()
            execucoes.append([])
            for line in lines:
                execucoes[e].append(int(line))


        indices = np.zeros(len(execucoes[0]), int)
        for i in xrange(0, len(execucoes[0])):
            indices[i] = i
        # for i in xrange(0, len(dados)):
        # plt.xlim(right = len(indices))
        # plt.yticks(np.arange(0, 1000, step=20))
        # plt.yticks(np.arange(1000, 1065, step=5))

    for i in xrange(0, len(execucoes[0])):
        media = 0
        for e in xrange(0, 10):
            media += execucoes[e][i]
        media /= 10
        medias.append(media)

    plt.plot(indices[::100], medias[::100], linestyle='-', marker= None)
    plt.show()

if __name__ == "__main__":
    main(sys.argv)
