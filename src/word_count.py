#! -*- encoding: utf-8 -*-

import os
import json
import pickle
import traceback
import random
from multiprocessing import Pool, freeze_support
from collections import Counter


with open('chi_char.txt') as fin:
    all_chars = set(fin.read())


def count_corpus_dict(corpus_dict):
    single_char_counter = Counter()
    bin_char_counter = Counter()
    try:
        corpus_string1 = '#' + corpus_dict['html'].strip('原标题') + '#' + corpus_dict['title']
        corpus_string2 = corpus_dict['html'].strip('原标题') + '#' + corpus_dict['title'] + '#'
        for c1, c2 in zip(corpus_string1, corpus_string2):
            if c1 in all_chars and c2 in all_chars:
                single_char_counter.update([c1])
                bin_char_counter.update([(c1, c2)])
    except Exception as e:
        print('Error %s happens, while dealing %s' % (e, corpus_dict))
        traceback.print_exc()
    return single_char_counter, bin_char_counter


if __name__ == '__main__':
    freeze_support()

    overall_single_char_counter = Counter()
    overall_bin_char_counter = Counter()

    corpus_dir = './sina_news/'
    corpus_fnames = os.listdir(corpus_dir)

    for fname in corpus_fnames:
        print('%s begin...' % fname)
        with open(corpus_dir + fname, 'rb') as fin:
            text = fin.read().decode('utf-8')

        json_texts = text.split('\n')

        corpus_dicts = [json.loads(json_text) for json_text in json_texts if json_text]
        # for debug
        #corpus_dicts = random.sample([json.loads(json_text) for json_text in json_texts if json_text], 1000)
        p = Pool(4)
        for i, (single_char_counter, bin_char_counter) in enumerate(p.imap_unordered(count_corpus_dict, corpus_dicts)):
            overall_single_char_counter.update(single_char_counter)
            overall_bin_char_counter.update(bin_char_counter)
            print('\r%s - %d/%d done' % (fname, i + 1, len(corpus_dicts)), end='')
        with open('single_char_counter2.pic', 'wb') as fout1, open('bin_char_counter2.pic', 'wb') as fout2:
            pickle.dump(overall_single_char_counter, fout1)
            pickle.dump(overall_bin_char_counter, fout2)
        print('\r%s done...' % fname)

    print(overall_single_char_counter.most_common(30))
    print(overall_bin_char_counter.most_common(30))