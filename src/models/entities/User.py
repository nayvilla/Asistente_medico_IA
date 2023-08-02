from werkzeug.security import check_password_hash, generate_password_hash
#Activar la clase usuario
from flask_login import UserMixin

class User(UserMixin):
    #En su metodo init recibe el id, user, pass y el nombre
    def __init__(self, id, username, password, fullname="") -> None:
        self.id = id
        self.username = username
        self.password = password
        self.fullname = fullname

    @classmethod
    def check_password(self, hashed_password, password):
        return check_password_hash(hashed_password, password)
    #Metodo para chequear el password
print(generate_password_hash("123456789"))

class hash():
    @staticmethod
    def has(passw):
        el= generate_password_hash(str(passw))        
        return el