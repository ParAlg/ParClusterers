import pyTigerGraph as tg
from contextlib import redirect_stdout

conn = tg.TigerGraphConnection(
    host='http://127.0.0.1',
    username='tigergraph',
    password='tigergraph',
)
print(conn)
print("success")
print(conn.gsql("DROP ALL"))