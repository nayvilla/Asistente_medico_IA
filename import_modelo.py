#Importar el modelo--------------------------------------------------------
import sklearn.externals 
import joblib
import pandas as pd
# Mysql
from pymysql import connect

# ---------------------------------------------------------------------------
# seleccion de la base de datos
data_base = connect(host = 'localhost',
                    user = 'root',
                    passwd = 'root',
                    database = 'sintomas')
cur = data_base.cursor()
# ver la tabla pacientes
# query = 'select * from pacientes'
query = 'SELECT * FROM pacientes ORDER BY ID_Paciente DESC LIMIT 1'
         
cur.execute(query)

# cambiar la tabla a dataframe
df = pd.read_sql(query, data_base)
a = df.iloc[0]
#print(df.iloc[0])
# ---------------------------------------------------------------------------
# df = pd.read_csv(r"prueba3.csv", sep=';', engine='python')

nb = joblib.load('234.pkl')

cols_to_drop = ['ID_Paciente', 'Fecha', 'Nombre', 'Cedula', 'Edad', 'BPM', 'SPO2', 'Temperatura']
dataros = df.drop(cols_to_drop, axis=1)
#Rellenar datos faltantes con la palabra "no"
dataros = dataros.fillna('no')
#---------Funciones para transformar str a numeric---------------------------------------
def cambio0str(x):
    if x == 'femenino':
        return 1
    else:
        return 0
#Genero
dataros['Genero N'] = dataros['Genero'].apply(cambio0str)
dataros = dataros.drop('Genero', axis=1)

def cambio1str(x):
    if x == 'si':
        return 1
    else:
        return 0
#secrecion nasal
dataros['Secrecion Nasal N'] = dataros['Secrecion_Nasal'].apply(cambio1str)  
dataros = dataros.drop('Secrecion_Nasal', axis=1)
#Congestión Nasal
dataros['Congestion Nasal N'] = dataros['Congestion_Nasal'].apply(cambio1str) 
dataros = dataros.drop('Congestion_Nasal', axis=1)
#Lagrimeo
dataros['Lagrimeo N'] = dataros['Lagrimeo'].apply(cambio1str) 
dataros = dataros.drop('Lagrimeo', axis=1)
#Estornudos
dataros['Estornudos N'] = dataros['Estornudos'].apply(cambio1str) 
dataros = dataros.drop('Estornudos', axis=1)
#Sensacion ahogo
dataros['Sensacion ahogo N'] = dataros['Sensacion_ahogo'].apply(cambio1str) 
dataros = dataros.drop('Sensacion_ahogo', axis=1)
#Fiebre
dataros['Fiebre N'] = dataros['Fiebre'].apply(cambio1str) 
dataros = dataros.drop('Fiebre', axis=1)
#Malestar general
dataros['Malestar general N'] = dataros['Malestar_general'].apply(cambio1str) 
dataros = dataros.drop('Malestar_general', axis=1)
#Picazon nasal
dataros['Picazon nasal N'] = dataros['Picazon_nasal'].apply(cambio1str) 
dataros = dataros.drop('Picazon_nasal', axis=1)
#Hinchazon ojos
dataros['Hinchazon ojos N'] = dataros['Hinchazon_ojos'].apply(cambio1str) 
dataros = dataros.drop('Hinchazon_ojos', axis=1)
#Ronquidos
dataros['Ronquidos N'] = dataros['Ronquidos'].apply(cambio1str) 
dataros = dataros.drop('Ronquidos', axis=1)
#Dolor muscular
dataros['Dolor muscular N'] = dataros['Dolor_muscular'].apply(cambio1str) 
dataros = dataros.drop('Dolor_muscular', axis=1)
#Perdida voz
dataros['Perdida voz N'] = dataros['Perdida_voz'].apply(cambio1str) 
dataros = dataros.drop('Perdida_voz', axis=1)
#Dolor ojos
dataros['Dolor ojos N'] = dataros['Dolor_ojos'].apply(cambio1str) 
dataros = dataros.drop('Dolor_ojos', axis=1)
#Diarrea
dataros['Diarrea N'] = dataros['Diarrea'].apply(cambio1str) 
dataros = dataros.drop('Diarrea', axis=1)
#Nauseas
dataros['Nauseas N'] = dataros['Nauseas'].apply(cambio1str) 
dataros = dataros.drop('Nauseas', axis=1)
#Dolor barriga
dataros['Dolor barriga N'] = dataros['Dolor_barriga'].apply(cambio1str) 
dataros = dataros.drop('Dolor_barriga', axis=1)
#Dolor pecho
dataros['Dolor pecho N'] = dataros['Dolor_pecho'].apply(cambio1str) 
dataros = dataros.drop('Dolor_pecho', axis=1)
#Perdida apetito
dataros['Perdida apetito N'] = dataros['Perdida_apetito'].apply(cambio1str) 
dataros = dataros.drop('Perdida_apetito', axis=1)
#Escalofrios
dataros['Escalofrios N'] = dataros['Escalofrios'].apply(cambio1str) 
dataros = dataros.drop('Escalofrios', axis=1)

def cambio2str(x):
    if x == 'leve':
        return 1
    elif x == 'moderado':
        return 2
    elif x == 'fuerte':
        return 3
    else:
        return 0
#Dolor garganta
dataros['Dolor garganta N'] = dataros['Dolor_garganta'].apply(cambio2str) 
dataros = dataros.drop('Dolor_garganta', axis=1)
#Dolor articular
dataros['Dolor articular N'] = dataros['dolor_articular'].apply(cambio2str) 
dataros = dataros.drop('dolor_articular', axis=1)
#Dolor cabeza
dataros['Dolor cabeza N'] = dataros['Dolor_cabeza'].apply(cambio2str) 
dataros = dataros.drop('Dolor_cabeza', axis=1)

def cambio3str(x):
    if x == 'seca':
        return 1
    else:
        return 0
#Tos
dataros['Tos N'] = dataros['Tos'].apply(cambio1str)  
dataros = dataros.drop('Tos', axis=1)

data = dataros.drop(columns='Diagnostico')
#target = dataros.Diagnostico
#print(data)
print("------------")
aux1 = nb.predict_proba(data)
#print(aux1)
aux2 = aux1[0][0]
#print(aux2)
aux3 = aux1[0][1]
#print(aux3)
aux4 = aux1[0][2]
#print(aux4)
#print(type(nb.predict_proba(data)))


if aux2 >= 0.8:
    print("------------")
    print("Diagnostico: Covid19")
    sql = "UPDATE pacientes SET Diagnostico = %s ORDER BY ID_Paciente DESC LIMIT 1"
    val = ("Covid19")
    cur.execute(sql, val)
    data_base.commit()
elif aux3 >= 0.8:
    print("------------")
    print("Diagnostico: Rinofaringitis o Resfriado común")
    sql = "UPDATE pacientes SET Diagnostico = %s ORDER BY ID_Paciente DESC LIMIT 1"
    val = ("Resfriado común")
    cur.execute(sql, val)
    data_base.commit()
elif aux4 >= 0.8:
    print("------------")
    print("Diagnostico: Rinitis alérgica")
    sql = "UPDATE pacientes SET Diagnostico = %s ORDER BY ID_Paciente DESC LIMIT 1"
    val = ("Rinitis alérgica")
    cur.execute(sql, val)
    data_base.commit()
else:
    print("------------")
    print("La enfemerdad que usted padece no se encuentra en la categoria")
