import requests
import xml.etree.ElementTree as ET

# URL of the XML file
url = "https://storage.googleapis.com/pcbs_vldb_2025"

# Fetching the XML content
response = requests.get(url)
xml_content = response.content

# Parsing XML
root = ET.fromstring(xml_content)

# Finding all content keys
content_keys = [content.find("{http://doc.s3.amazonaws.com/2006-03-01}Key").text for content in root.findall(".//{http://doc.s3.amazonaws.com/2006-03-01}Contents")]

# Printing all content keys
print("List of files:")
for key in content_keys:
    print(key)
