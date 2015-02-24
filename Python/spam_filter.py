# coding=utf-8
'''
Python 2.7.1
NLTK 2.0.4
'''
from __future__ import print_function
from random import sample
from nltk import NaiveBayesClassifier, FreqDist, regexp_tokenize, pos_tag
from nltk.classify import accuracy, apply_features
from nltk.corpus import wordnet as wn, stopwords


def get_marked_set(path):
    with open(path) as f:
        return [list(reversed(sms.split('\t', 1))) for sms in f]


def get_tokens(text):
    # return [w.lower() for w in regexp_tokenize(text, "[\w']+") if w.lower() not in stopwords.words('english')]
    return [w.lower() for w, tag in pos_tag(regexp_tokenize(text, "[\w']+"))
                        if tag not in {'POS', 'DT', 'PRP', 'IN', 'VBZ', 'RB', 'MD', 'CC', 'TO', 'PRP$'}]


def get_all_words(content_set):
    all_words = []
    for sms in content_set:
        words_in_content = get_tokens(sms[0])
        for word in words_in_content:
            word = wn.morphy(word) or word
            all_words.append(word)
    return FreqDist(all_words)


class Doc_extract:
    def __init__(self, all_words, amount_of_words):
        self.words = all_words
        self.amount_of_words = amount_of_words

    def document_extraction(self, documnet):
        features = {}
        words_in_doc = get_tokens(documnet)
        for word in self.words.keys()[:min(len(self.words), self.amount_of_words)]:
            word = wn.morphy(word) or word
            features['contains(%s)' % word] = (word in words_in_doc)
        return features

    def __call__(self, document):
        return self.document_extraction(document)


def get_f_measure(classifier, items_set):
    results = classifier.batch_classify([fs for (fs, l) in items_set])
    tp = sum([l==r for ((fs, l), r) in zip(items_set, results) if l == 'ham'])
    fp = sum([l!=r for ((fs, l), r) in zip(items_set, results) if l == 'spam'])
    fn = sum([l==r for ((fs, l), r) in zip(items_set, results) if l == 'spam'])
    precision = float(tp)/(tp + fp) if tp or fp else 0.00001
    recall = float(tp)/(tp + fn) if tp or fn else 0.00001
    f_measure = float(2 * precision * recall)/(precision + recall)
    return precision, recall, f_measure

def get_trained_classifier(train_set):
    return NaiveBayesClassifier.train(train_set)


def cross_validate(content_set, times, words, amount_of_words):
    incr = len(content_set) // times
    document_extraction = Doc_extract(words, amount_of_words)
    for i in xrange(times):
        train_set = apply_features(document_extraction,
                                   content_set[:i * incr] + content_set[(i + 1) * incr:])
        test_set = apply_features(document_extraction,
                                  content_set[i * incr: min((i + 1) * incr, len(content_set))])
        classifier = get_trained_classifier(train_set)
        acc = accuracy(classifier, test_set)
        print('\n{0} classifier\n\tAccuracy:  {1:.6}'.format(i + 1, acc))
        print('\tPrecision: {0:.6}\n\tRecall:    {1:.6}\n\tF_measure: {2:.6}'.format(*get_f_measure(classifier, test_set)))


if __name__ == '__main__':
    sms_set = sample(get_marked_set('SMSSpamCollection'), 100)
    words = get_all_words(sms_set)
    cross_validate(sms_set, 3, words, 200) # cross-validation on 3 folds
