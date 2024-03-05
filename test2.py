import requests

headers = {
    'X-Requested-With': 'XMLHttpRequest'
}

data = {
    'username': 'john',
    'password': 'doe'
}

response = requests.post('http://192.168.0.17/action?go="stop"')

print(response.text)