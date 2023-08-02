import random

R_AYUDAR = "Estoy aquí para servirte!. Dime, ¿Cuáles son tus síntomas?"+ '*48'
R_PAPA = 'Mi papá es Naythan, ¿Lo conoces?, el me creo para ayudar a los pacientes con infecciones respiratorias. Es un buen tipo 🤭. Así que, ¿Cuáles son tus síntomas?'+ '*49'

def unknown():
    response = ["¿Puedes escribirlo de otra forma?, no lo he captado"+ '*44',
                "No lo he entendido ..."+ '*45',
                "Suena bien si lo entendiera jaja. Escribelo de otra forma"+ '*46',
                "¿Qué significa eso?, escribelo de otra manera"+ '*47',
                "Lo siento los síntomas que intentas ingresar no están en mi base de datos"+ '*50'][
        random.randrange(5)]
    return response
