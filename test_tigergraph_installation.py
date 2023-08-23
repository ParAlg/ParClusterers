import pyTigerGraph as tg
import json
import sys
import csv
import time
import io
from contextlib import redirect_stdout
import runner_utils
import load_tg

conn = tg.TigerGraphConnection(
    host='http://127.0.0.1',
    username='tigergraph',
    password='tigergraph',
)
print(conn)
print("success")