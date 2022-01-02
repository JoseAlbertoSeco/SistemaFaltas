<?php
    // insertarAlumno.php
    // Importamos la configuración
    require("../config.php");
    // Leemos los valores que nos llegan por GET
    $nombre = $_GET['nombre'];
    $ape_1 = $_GET['ape_1'];
    $ape_2 = $_GET['ape_2'];
    $curso = $_GET['curso'];
    $huella = $_GET['huella'];

    // Esta es la instrucción para insertar los valores
    $query = "INSERT INTO `alumno`( `A_nombre`, `A_ape1`, `A_ape2`, `A_idCurso`, `A_HuellaTemplate`) 
              VALUES ('".$nombre."','".$ape_1."','".$ape_2."','".$curso."', '".$huella."')";

    // Ejecutamos la instrucción
    if (mysqli_query($con, $query)) {
        echo json_encode(array("statusCode"=>200));
    } 
    else {
        echo json_encode(array("statusCode"=>201));
    }
    mysqli_close($con);
?>
