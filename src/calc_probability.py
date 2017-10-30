import pickle
import math
import numpy as np


def calc_lncond_prob(bin_count, single_count, single_prob, alpha):
    return - math.log(alpha * (bin_count / single_count) + (1 - alpha) * single_prob)


def generate_txt(single_count, bin_count, alpha=0.95, beta=1.5):
    single_sum = sum(single_count.values())
    single_prob = {k: v / single_sum for k,v in single_count.items()}

    dissipation_map = {k: calc_lncond_prob(v, single_count[k[0]], single_prob[k[0]], alpha) for k, v in bin_count.items()}
    for k, v in single_prob.items():
        dissipation_map[('##', k)] = - math.log((1 - alpha) * v)

    dissipation_map = {k: v ** beta for k, v in dissipation_map.items()}

    with open('dissipation_map_%.2f_%.2f.txt' % (alpha, beta), 'w') as fout:
        fout.writelines(['%s %s %f\n' % (k[0], k[1], v) for k, v in dissipation_map.items()])

#with open('single_char_prob.pic', 'wb') as fout1, open('lncond_bin_char_prob.pic', 'wb') as fout2:
#    pickle.dump(single_prob, fout1)
#    pickle.dump(lncond_bin_char_prob, fout2)

if __name__ == '__main__':
    with open('single_char_counter2.pic', 'rb') as fin1, open('bin_char_counter2.pic', 'rb') as fin2:
        single_count = pickle.load(fin1)
        bin_count = pickle.load(fin2)
    #generate_txt(single_count, bin_count)
    #exit()
    for alpha in np.linspace(0.80, 0.95, 4):
        for beta in np.linspace(0.8, 2.6, 4):
            print(alpha, beta)
            generate_txt(single_count, bin_count, alpha, beta)
