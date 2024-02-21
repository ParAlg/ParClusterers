"""Count average number of cluster a data point is in. """

import numpy as np
import gzip
import json

base_dir = "../xc_data"

def read_corpus(fname):
    """
    Read gzip file with one json string in each line

    Returns
    -------
    A generator with dictionary as values
    """
    with gzip.open(fname, "r") as fp:
        for line in fp:
            yield json.loads(line)


def read(fname, title_only):
    """
    if title_only is True, we only return the titles. Otherwise concat title and content.
    """
    labels = []
    fp = read_corpus(fname)
    for line in fp:
        labels.append(line["target_ind"])
    return labels


def get_labels(train_dir, test_dir):
    all_labels = []
    for data_dir in [train_dir, test_dir]:
        labels = read(data_dir)
        all_labels.extend(labels)

    return all_labels

datasets = ["AmazonTitles-670K","WikiSeeAlsoTItles-350K", "Amazon-670K.raw", "Wikipedia-500K.raw"]
for dataset in datasets:
    print(dataset)
    train_dir = f"{base_dir}/{dataset}/trn.json.gz"
    test_dir = f"{base_dir}/{dataset}/tst.json.gz"
    if ".raw" in dataset:
        train_dir = f"{base_dir}/{dataset}/trn.raw.json.gz"
        test_dir = f"{base_dir}/{dataset}/tst.raw.json.gz"
        
    all_labels = get_labels(train_dir, test_dir)
    print("average:", np.mean(np.array([len(c) for c in all_labels])))
