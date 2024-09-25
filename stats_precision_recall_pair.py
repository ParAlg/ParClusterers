
import logging
import sys
import json

def _config_str_to_dict(input_str):
  # e.g. 
  # input_str = "precision_recall_pair_thresholds: 0.86;0.88;0.90;0.92;0.94,f_score_param: 0.5"
  # result_dict = {'precision_recall_pair_thresholds': [0.86;0.88;0.90;0.92;0.94], 'f_score_param': 0.5}


  # Split the string into key-value pairs
  pairs = input_str.split(',')

  # Initialize an empty dictionary
  result_dict = {}

  # Process each key-value pair
  for pair in pairs:
      # Split the pair by the colon
      key, value = pair.split(':')
      # Remove any leading/trailing whitespace
      key = key.strip()
      value = value.strip()
      # Convert the value to float if possible
      try:
          if key == "precision_recall_pair_thresholds":
            # Split the value by semicolons and convert each to float
            value = [float(v.strip()) for v in value.split(';')]
          else:
            value = float(value)
      except ValueError:
          pass  # Keep the value as a string if it cannot be converted
      # Add the key-value pair to the dictionary
      result_dict[key] = value
  return result_dict


def read_clusters(cluster_file):
    """
    Reads the clusters from a file and returns a dictionary mapping node IDs to a set of cluster IDs.
    """
    node_to_clusters = {}
    with open(cluster_file, 'r') as f:
        for cluster_id, line in enumerate(f):
            nodes = line.strip().split("\t")
            for node in nodes:
                node = node.strip()
                if node:
                    if node not in node_to_clusters:
                        node_to_clusters[node] = set()
                    node_to_clusters[node].add(cluster_id)
    return node_to_clusters

def read_ground_truth_pairs(ground_truth_file):
    """
    Reads the ground truth pairs from a file and returns a list of tuples (node1, node2, weight).
    """
    pairs = []
    with open(ground_truth_file, 'r') as f:
        for line in f:
            parts = line.strip().split('\t')
            if len(parts) >= 3:
                node1 = parts[0].strip()
                node2 = parts[1].strip()
                try:
                    weight = float(parts[2].strip())
                except ValueError:
                    print(f"Invalid weight '{parts[2]}' in line: {line.strip()}")
                    continue
                pairs.append((node1, node2, weight))
            else:
                print(f"Ignoring invalid line: {line.strip()}")
    return pairs

def compute_precision_recall(node_to_clusters, pairs, thresholds, f_score_param):
    """
    Computes precision and recall based on the clusters and ground truth pairs.
    Handles overlapping clusters where a node can belong to multiple clusters.

    node_to_clusters: map from node id to a set of clusters
    pairs: list of (u,v,w) triplets
    """
    TP = 0  # True Positive
    TN = 0  # True Negative
    FP = 0  # False Positive
    FN = 0  # False Negative

    precisions = {}
    recalls = {}    
    f_scores = {}

    for threshold in thresholds:
      for node1, node2, weight in pairs:
          # Determine if the pair is positive or negative
          is_positive = weight > threshold

          # Determine if the nodes are in the same cluster (overlapping clusters)
          if node1 in node_to_clusters and node2 in node_to_clusters:
              clusters1 = node_to_clusters[node1]
              clusters2 = node_to_clusters[node2]
              in_same_cluster = bool(clusters1 & clusters2)  # Check for non-empty intersection
          else:
              logging.warning("skipping nodes %s, %s", node1, node2)
              # Nodes not found in clusters; skip this pair
              continue

          if is_positive:
              if in_same_cluster:
                  TP += 1
              else:
                  FN += 1
          else:
              if not in_same_cluster:
                  TN += 1
              else:
                  FP += 1

      # Calculate precision and recall
      precision = TP / (TP + FP) if (TP + FP) > 0 else 0
      recall = TP / (TP + FN) if (TP + FN) > 0 else 0
      f_score = 0
      if precision !=0 and recall != 0:
        f_score = (1 + f_score_param * f_score_param) * precision * recall / ((f_score_param * f_score_param * precision) + recall)

      precisions[threshold] = precision
      recalls[threshold] = recall
      f_scores[threshold] = f_score

    return precisions, recalls, f_scores


def compute_precision_recall_pair(in_clustering, input_communities, out_statistics, stats_config, stats_dict):
  """
  Compute pair precision and recall, and record result into stats_dict
  """
  stats_config = _config_str_to_dict(stats_config)
  precision_recall_pair_thresholds = stats_config["precision_recall_pair_thresholds"]
  f_score_param = stats_config.get("f_score_param", 1)
  print()
  print("clustering file", in_clustering)
  print("community file", input_communities)
  print("stats file", out_statistics)
  print("parameters, ", precision_recall_pair_thresholds, f_score_param)

  # Read clusters and ground truth pairs
  clusters = read_clusters(in_clustering)
  pairs = read_ground_truth_pairs(input_communities)

  # Compute precision and recall
  precisions, recalls, f_scores = compute_precision_recall(clusters, pairs, precision_recall_pair_thresholds, f_score_param)

  stats_dict["fScore_mean"] = f_scores
  stats_dict["communityPrecision_mean"] = precisions
  stats_dict["communityRecall_mean"] = recalls
  stats_dict["PrecisionRecallPairThresholds"] = precision_recall_pair_thresholds
  stats_dict["fScoreParam"] = f_score_param

  with open(out_statistics, 'w', encoding='utf-8') as f:
    json.dump(stats_dict, f, indent=4)