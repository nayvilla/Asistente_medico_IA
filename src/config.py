#Envio de mensajes a traves de la funcion flask
class Config:
    SECRET_KEY = 'B!1w8NAt1T^%kvhUI*S^'

#configuracion de desarrollo
class DevelopmentConfig(Config):
    #Iniciar en modo de depuracion
    DEBUG=True
    MYSQL_HOST = 'localhost'
    MYSQL_USER = 'paciente'
    MYSQL_PASSWORD = '123456789'
    MYSQL_DB = 'sintomas'

#crear un diccionario con esta clase
config={
    'development':DevelopmentConfig
}
