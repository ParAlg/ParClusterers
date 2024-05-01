## Data

The data file names used in the experiment can be obtained from 
```python
python3 data/files.py 
```
Then, each file can be downloaded from "https://storage.googleapis.com/pcbs_vldb_2025/[file_name]". For example, the labels for iris dataset can be downloaded from "https://storage.googleapis.com/pcbs_vldb_2025/UCIknnGraphs/iris.cmty.txt".

Files ends with ".cmty" or ".cmty.txt" are ground truth labels. Files ends with ".ungraph.txt" or ".graph.txt" are in edge list format. Files ends with ".gbbs.txt" are in [GBBS format](https://github.com/ParAlg/gbbs).  

Certain graph files can be large in size. For instance, the largest graph is the Friendster graph, which is around 30GB, so we recommend reviewing the list of available files and download the ones you require.


## Prepare data and config files for experiment.
Create a folder `ParClusterers/pcbs_vldb_2025` and put data into it.
Run `python3 configs_experiments/fix-path.py` in the root folder `ParClusterers/` to change the input graph path in config files to the absolute path of `ParClusterers/pcbs_vldb_2025`. 


## Experiments for small UCI datasets
The outputs will be in `ParClusterers/results/`.

Obtain clustering results
```bash
python3 cluster.py configs_experiments/uci/cluster_uci_pcbs.config
python3 cluster.py configs_experiments/uci/cluster_uci_snap.config
# need to install networkit first
python3 cluster.py configs_experiments/uci/cluster_uci_nk.config
# need to install and start Neo4j server first
python3 cluster.py configs_experiments/uci/cluster_uci_neo4j.config
# need to install and start TigerGraph server first
python3 cluster.py configs_experiments/uci/cluster_uci_tg.config
```

Obtain stats
```bash
python3 stats.py configs_experiments/uci/cluster_uci_pcbs.config configs_experiments/uci/stats_uci.config
python3 stats.py configs_experiments/uci/cluster_uci_snap.config configs_experiments/uci/stats_uci.config
python3 stats.py configs_experiments/uci/cluster_uci_nk.config configs_experiments/uci/stats_uci.config
python3 stats.py configs_experiments/uci/cluster_uci_neo4j.config configs_experiments/uci/stats_uci.config
python3 stats.py configs_experiments/uci/cluster_uci_tg.config configs_experiments/uci/stats_uci.config
```



## Plotting
```bash
# Please install latex, which is used for plotting.
sudo apt-get install texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra
sudo apt-get install dvipng
python3 plotting/plot_pareto.py 
```

## Install NetworKit
Please follow instructions from [NetworKit](https://github.com/networkit/networkit).


## Install Neo4j

The following instructions are written for Neo4j version 5.8.0, which we used in experiments. If version 5.8.0 is not available, please use the latest version and follow instructions from official Neo4j website.

Download Neo4j community version https://neo4j.com/download-center/#community and follow instruction to install. For Google Cloud, use Linux / Mac Executable version. 
- You need Java17 installed for Neo4j version 5.x.
- Extract the contents of the archive, using:
tar -xf <filecode>.
For example,
tar -xf neo4j-community-5.8.0-unix.tar.gz

Download graph data science jar file from [here](https://github.com/neo4j/graph-data-science/releases/). We used version 2.4.3 in our expeirments. **You need a version that's compatible with the Neo4j version you installed**.

Follow the instruction [here](https://neo4j.com/docs/graph-data-science/current/installation/neo4j-server/) to setup the server for graph data science.
- move the neo4j-graph-data-science-[version].jar file into the $NEO4J_HOME/plugins directory
- In neo4j-community-…/conf/neo4j.conf, uncomment `dbms.security.auth_enabled=false`
and add `dbms.security.procedures.unrestricted=gds.*` and `dbms.security.procedures.allowlist=gds.*`.

In neo4j-community-…/, run `./bin/neo4j console` (in background) or `./bin/neo4j start` to start Neo4j. You need to start Neo4j before benchmarking Neo4j methods using PCBS.

You can run `python3 tests/test_neo4j_installation.py` to check if Neo4j is installed successfully and is running. 


## Install TigerGraph
Download TigerGraph DB [here](https://dl.tigergraph.com/). We used version 3.9.2. Note that you need to either pay or request a free dev license from TigerGraph.
Follow instruction for install: https://docs.tigergraph.com/tigergraph-server/current/getting-started/linux.

Once installed, wwitch to tigergraph user (`su tigergraph`), default password is tigergraph.
Run the following
```bash
gadmin start infra
gadmin start gsql
```
Tigergraph can now be accessed using tigergraph user credentials for connection.

Instructions for adding Speaker-Listener Label Propagation method:
- Download gsql file from [here](https://raw.githubusercontent.com/tigergraph/gsql-graph-algorithms/master/algorithms/Community/speaker-listener_label_propagation/tg_slpa.gsql).
- Create query folder in tigergraph and add gsql file. Change `SLPA_QUERY_FILE_PATH` in `cluster_tg.py` to the gsql file location.

You can run `python3 tests/test_tigergraph_installation.py` to check if TigerGraph is installed successfully and is running. 
