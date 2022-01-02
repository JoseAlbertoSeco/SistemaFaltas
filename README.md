# Sistema Faltas

Sistema desarrollado para combatir el absentismo escolar de la mano de tecnologías innovadoras como son los lectores dactilares 

# Pre-requisitos

En este apartado se expondrán las librerías necesarias para el correcto funcionamiento del sistema.

## Python:
```python
pip install -r requirements.txt
```

## Arduino:
 
FPM <br />
HardwareSerial <br />
Wifi <br />
WifiMulti <br />
HTTPClient <br />

# Análisis de archivos

* 	Documentación: manual de usuario, documentación del proyecto y power point para su presentación.

* Arduino: configuración de la ESP32 y AS608 para autenticar y registrar huellas. Archivos a cargar en la ESP32: autenticar.ino y registrar.ino.

* Controlador: crea los archivos con las huellas en el servidor web, código fuente en python3: templateControler.py.

* Exe_Controlador: ejecutable que crea los archivos con las huellas en el servidor web: templateControler.exe

* DB: configuración para crear un database en MySQL: SistemaFaltas_emptyDB.sql.

* Pagina Web: condiguración del sistema web. En esta carpeta nos encontramos diferentes archivos: <br />
			- index.php: "main" del servidor en su conjunto. <br />
			- config.php: credenciales para la conexión con la base de datos. <br />
			- Carpeta addIndex: control_data.php, control_huella.php, control_setup.php, controladores de registro. <br />
			- Carpeta css: style.css, archivo para modificar esteticamente la página web. <br />
			- Carpeta scripts: insetarAlumno.php, updateFalta.php y script.js para interaccionar con la base de datos.


  
# Construido con :keyboard:

* Python3 
* Arduino
* Lector de huellas AS6081
* ESP32 → Wi-Fi
* PHP
* MySql
* JavaScript, HTML, CSS, JQuery & AJAX

# Autores :pencil:

* [María Blanco Gónzalez-Mohíno](https://github.com/MariaBlancoGonzalez/)
* [José Alberto Seco Sánchez-Camacho](https://github.com/JoseAlbertoSeco/)
* [Pablo Velasco Crespo](https://github.com/PabloVelascoCrespo/)

# License
Licensed under the term of Apache License 2.0. See file [LICENSE](LICENSE).
