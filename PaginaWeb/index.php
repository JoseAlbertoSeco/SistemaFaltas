<!DOCTYPE html>
<html lang="en" >
<head>
  <meta charset="UTF-8">
  <title>CodePen - Responsive Login Form</title>
  <link rel='stylesheet' href='https://ajax.googleapis.com/ajax/libs/jqueryui/1.11.2/themes/smoothness/jquery-ui.css'><link rel="stylesheet" href="css/style.css">
  <script src="https://code.jquery.com/jquery-3.2.1.js"></script>

</head>
<body>
<!-- partial:index.partial.html -->
<link href='https://fonts.googleapis.com/css?family=Ubuntu:500' rel='stylesheet' type='text/css'>

<div class="login">
  <div class="login-header">
    <h1>Registro</h1>
  </div>
  <div class="login-form">
    <h3>Nombre Alumno:</h3>
    <input type="text" id="nombre" placeholder="Nombre del alumno"/><br>
    <h3>Primer Apellido:</h3>
    <input type="text" id="ape_1" placeholder="Primer apellido"/>
    <h3>Segundo Apellido:</h3>
    <input type="text" id="ape_2" placeholder="Segundo apellido"/>
    <h3>Curso:</h3>
    <input type="text" id="curso" placeholder="Curso del alumno"/>
    <br><br>
    <input type="button" id="boton" value="Registrar" class="login-button"/>
    <input type="button" id="boton2" value="Relog" class="login-button"/>
    <br><br>
    <h1 class="login-button" id="value">Esperando...</h1>
    <p hidden id="huella">Esperando...</p>

    <script type="text/javascript">
    $(document).ready(function() { //JQUERY

        value = "";
        huella = "";
        comprobar = true;

        function comprobarSensor() {
            value = $('#value').text();

            comprobarHuella();
            $.ajax({
                type: "POST",
                url: "addIndex/control_data.php",
                success: function(data) {
                    $('#value').text(data);
                }
            });
        }
        function comprobarHuella() {
            huella = $('#huella').text();
            $.ajax({
                type: "POST",
                url: "addIndex/control_huella.php",
                success: function(data) {
                    $('#huella').text(data);
                }
            });
        }

        $('#boton').on('click',function registrarAlumno() {

            if (value=="Huella almacenada") {
                nombre = $('#nombre').val();
                ape_1 = $('#ape_1').val();
                ape_2 = $('#ape_2').val();
                curso = $('#curso').val();
                huella = $('#huella').text();
                if (nombre!='' && ape_1!='' && ape_2!='' && curso!='' && huella!='') {
                    $.ajax({
                        type: "GET",
                        url: "scripts/insertarAlumno.php",
                        data: "nombre=" + nombre + "&ape_1=" + ape_1 + "&ape_2=" + ape_2 + "&curso=" + curso + "&huella=" + huella,
                        success: function(dataResult) {
                            if(dataResult.statusCode==200){
                                alert("nombre=" + nombre + "&ape_1=" + ape_1 + "&ape_2=" + ape_2 + "&curso=" + curso + "&huella=" + huella);                      
                            }
                            else if(dataResult.statusCode==201){
                               alert("Se ha producido un error con el registro!");
                            }
                        }
                    }).done( function() {
                        $.ajax({
                            type: "GET",
                            url: "addIndex/control_setup.php",
                            data: "option=reiniciar&reiniciar=false",
                            success: function(dataResult) {
                            }
                        })
                        comprobar = true;  
                    }).fail( function() {
                        alert( 'Error!!' + huella );
                    });
                } else {
                    alert("Algún campo está vacío");
                }   
            } else {
                alert("No se ha almacenado la huella");
            }
        });

        $('#boton2').on('click',function registrarAlumno() {
            nombre = $('#nombre').val('');
            ape_1 = $('#ape_1').val('');
            ape_2 = $('#ape_2').val('');
            curso = $('#curso').val('');
        });

        if (comprobar) {
            setInterval(comprobarSensor, 1000);
        }

    });
    </script>

  </div>
</div>
<div class="error-page">
  <div class="try-again">Error: Try again?</div>
</div>
<!-- partial -->
  <script src='https://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>
<script src='https://ajax.googleapis.com/ajax/libs/jqueryui/1.11.2/jquery-ui.min.js'></script><script  src="scripts/script.js"></script>

</body>
</html>
