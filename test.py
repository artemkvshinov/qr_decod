from pywebio.input import *
from pywebio.output import *
from pywebio.session import *
from pywebio.pin import *
from pywebio import start_server
import requests

adres = input()

#def adres_init(adr):
#    adres = adr
#    print(adres)

#input('Input your age', validate=adres_init)
print(adres)
def post(inf):
    requests.get('http://'+adres+'/action?go='+inf)
# http://192.168.0.17/action?go=backward
    
put_buttons([  
    dict(label=i, value=i, color='secondary')  
    for i in ['left', 'forward','stop', 'backward', 'right',]  
], onclick=post)  