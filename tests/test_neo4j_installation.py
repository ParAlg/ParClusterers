from graphdatascience import GraphDataScience
from neo4j import GraphDatabase

neo4j_url = "bolt://localhost:7687"
neo4j_client = GraphDatabase.driver(neo4j_url, auth=None, max_connection_lifetime=7200)
gds = GraphDataScience(neo4j_client, auth=None) #"bolt://localhost:7687"
graph_exists = gds.graph.exists(graph_name="graph")

print("version", gds.version())
print("success")