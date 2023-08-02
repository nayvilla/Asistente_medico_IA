# Asistente_medico_IA
Creación de un asistente virtual que diagnostica 3 enfermedades covid19, refriado común, rinitis alérgica. Este proyecto fue desarrollado en lenguaje python, HTML, CSS y JavaScript. Adicionalmente se hizo uso de Bootstrap5 para mejorar el diseño y responsividad de la página web.

Interfaz de Login
![image](https://github.com/nayvilla/Asistente_medico_IA/assets/94719402/1149f667-3a9c-4131-8254-90f35e487bd3)

Interfaz de Registro
![image](https://github.com/nayvilla/Asistente_medico_IA/assets/94719402/cdb7f1be-8209-4a7b-840d-cb306c0209fe)

Interfaz del Dashboard
![image](https://github.com/nayvilla/Asistente_medico_IA/assets/94719402/264565cb-9de7-4165-9d54-7564dd6845d8)

Interfaz del Chatbot
![image](https://github.com/nayvilla/Asistente_medico_IA/assets/94719402/c163753f-8aca-47f1-9284-4a6b1f6ee174)

Interfaz de Historia Clínica 
![image](https://github.com/nayvilla/Asistente_medico_IA/assets/94719402/4c755ba2-0752-4f5c-8f84-e8855963da9a)

Dispositivo electrónico para la medicion de signos vitales
![image](https://github.com/nayvilla/Asistente_medico_IA/assets/94719402/1373de39-176a-46a0-8fa9-3f4c217f1bad)

CONCLUCIONES:

•Las pruebas de funcionamiento ratificaron el cumplimiento del algoritmo de inteligencia artificial, el modelo Máquina de Soporte de Vectores fue la mejor opción ya que manejo adecuadamente la clase “covid19”, “resfriado” y “rinitis” con una precisión de 0.85, 0.88 y 1 respectivamente, donde la clase que mejor se desempeña en el modelo es la clase “rinitis”. Lo que le da como resultado una precisión promedio de 0.91, haciendo al modelo apto para el uso médico dentro de las pautas establecidas por la doctora colaboradora.

• Los resultados obtenidos del dispositivo electrónico en la medición de frecuencia cardiaca (BPM), saturación de oxígeno en la sangre (SpO2) y temperatura corporal (°C) arrojaron una fiabilidad del 95.48%, 97.55% y 98.93% respectivamente. Esto hace que el dispositivo pueda ser empleado por el paciente para verificar sus signos vitales en tiempo real y detectar si existe alguna anomalía en sus medicines, estos datos son enviados al servidor de Google Cloud con un retardo de 2 segundos, logrando así una reducción de la latencia del sistema y la sobrecarga del servidor.

• La aplicación Web llamada “Baymax” utilizada para el diagnosticar a pacientes con covid19, resfriado común y rinitis alérgica, fue alojada en el servidor de 
Google Cloud para lograr que los usuarios puedan acceder a la misma desde cualquier dispositivo con conexión a internet sin requerir ninguna instalación previa. Al utilizar el framework Flask juntamente con Bootstrap 5 se logró una aplicación responsiva, dinámica y robusta ya que cuenta con seguridades tanto para el usuario como para la aplicación Web. Además, las herramientas de HTML, CSS, Javascript, Python y MySQL utilizadas en el desarrollo del proyecto permiten que la aplicación web sea escalable dependiendo de las necesidades que presenten los pacientes.
