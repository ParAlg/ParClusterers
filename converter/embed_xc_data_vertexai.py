"""embed data from http://manikvarma.org/downloads/XC/XMLRepository.html using Vertex AI"""

import vertexai
from vertexai.language_models import TextEmbeddingModel, TextEmbeddingInput
import pandas as pd
import numpy as np
from tenacity import retry, wait_random_exponential, stop_after_attempt
import gzip
import json
from collections import defaultdict
import tqdm
import os


# need to run gcloud auth application-default login
vertexai.init(project="julian-shun-3fef")

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
    text = []
    fp = read_corpus(fname)
    for line in fp:
        # different dataset might use different fields
        if title_only:
            text.append(line["title"])
        else:
            text.append(line["title"] + line["content"])
        labels.append(line["target_ind"])
    return text, labels


def get_communities(labels):
    """
    `labels[i]` is the label for node i
    Returns a list of list where each inner list is a community. Duplicated communities are removed.
    """
    communities = defaultdict(set)
    for i, clusters in enumerate(labels):
        for c in clusters:
            communities[c].add(i)
    # Convert each inner list into a tuple and create a set of tuples
    unique_set = set(tuple(inner_list) for inner_list in communities.values())

    # Convert the set of tuples back into a list of lists
    unique_list = [list(inner_tuple) for inner_tuple in unique_set]
    return unique_list



@retry(wait=wait_random_exponential(min=1, max=20), stop=stop_after_attempt(10))
def get_embeddings(texts, model):
    """
    Fetches embeddings for a batch of texts using a specified OpenAI embedding model.

    Args:
    - texts (list of str): The texts to encode.
    - model (str): The model to use for encoding.

    Returns:
    - list of list: A list of embedding lists, each representing the embedding for a text.
    """
    embedding_model = TextEmbeddingModel.from_pretrained(model)
    inputs = []
    for text in texts:
        text_embedding_input = TextEmbeddingInput(
            task_type="CLUSTERING", text=text)
        inputs.append(text_embedding_input)
    embeddings = embedding_model.get_embeddings(inputs)
    all_vectors = []
    for embedding in embeddings:
        vector = embedding.values
        all_vectors.append(vector)
        return all_vectors


def embed_texts(text, batch_size=2000):
    """
    `text` is a list of strings, return the embeddings for each string in the list.
    """
    n = len(text)
    embeddings = []

    for start in tqdm.tqdm(range(0, n, batch_size)):
        end = start + batch_size
        batch_texts = text[start:end]
        batch_embeddings = get_embeddings(batch_texts, model="textembedding-gecko@003")
        embeddings.extend(batch_embeddings)
    return embeddings


def get_vectors_and_labels(train_dir, test_dir, title_only):
    all_labels = []
    all_embeddings = []
    for data_dir in [train_dir, test_dir]:
        text, labels = read(data_dir, title_only)
        all_labels.extend(labels)
        
        char_count_sum = sum(len(string) for string in text)
        print("price: ", char_count_sum/1000*0.0002)
#         print("Minute Limit:", num_tokens/ 1e6)

        embeddings = embed_texts(text, batch_size=250)
        all_embeddings.extend(embeddings)
    communities = get_communities(all_labels)
    print("num communities", len(communities))
    return all_embeddings, communities




datasets = [
    "AmazonTitles-670K",
    "WikiSeeAlsoTItles-350K",
    "Amazon-670K.raw",
    "Wikipedia-500K.raw",
]
title_only_dict = {
    "AmazonTitles-670K": True,
    "WikiSeeAlsoTItles-350K": True,
    "Amazon-670K.raw": False,
    "Wikipedia-500K.raw": True,
}
for dataset in datasets:
    print(dataset)
    train_dir = f"{base_dir}/{dataset}/trn.json.gz"
    test_dir = f"{base_dir}/{dataset}/tst.json.gz"
    if ".raw" in dataset:
        train_dir = f"{base_dir}/{dataset}/trn.raw.json.gz"
        test_dir = f"{base_dir}/{dataset}/tst.raw.json.gz"

    all_embeddings, communities = get_vectors_and_labels(
        train_dir, test_dir, title_only_dict[dataset]
    )
    all_embeddings = np.array(all_embeddings)

    if dataset.startswith("Wikipedia") and title_only_dict[dataset]:
        dataset = "WikiTitles"
    dataset = dataset.split("-")[0]
    print("saving to, ", f"{base_dir}/{dataset}_palm.npy")
    with open(f"{base_dir}/{dataset}_palm.npy", "wb") as f:
        np.save(f, all_embeddings)

    lines_to_write = []
    for cluster_list in communities:
        lines_to_write.append("\t".join(str(x) for x in cluster_list))

    print("saving to, ", f"{base_dir}/{dataset}_palm.cmty")
    with open(f"{base_dir}/{dataset}_palm.cmty", "w") as f:
        f.write("\n".join(lines_to_write) + "\n")



