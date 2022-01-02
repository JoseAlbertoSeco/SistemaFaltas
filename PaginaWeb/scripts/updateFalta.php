<?php
    // insertarAlumno.php
    // Importamos la configuración
    // Leemos los valores que nos llegan por GET SELECT `AC_idAsignatura` FROM `asig_clase` WHERE `AC_idClase`=1;
    $idAlumno = $_GET['idAlumno'];
    //= new DateTime(date('H:i:s'));

    // Horario de las 6 clases lectivas

    $hora_clase1 = 480; // 8:00
    $hora_clase2 = 540; // 9:00 //HORA DE PRUEBA
    $hora_clase3 = 600; // 10:00
    $hora_clase4 = 690; //new DateTime('11:30:00');
    $hora_clase5 = 750; //new DateTime('12:30:00');
    $hora_clase6 = 580;//810; //new DateTime('13:10:00');

    $hora_actual = date('H') * 60 + date('i');
    echo $hora_actual;
    echo $hora_clase6;
    // Comprobación de hora y actualización de falta
    /*
    if($hora_actual >= $hora_clase1 && $hora_actual <= $hora_clase2){
        updateFalta($hora_clase1,1);
    }elseif($hora_actual >= $hora_clase2 && $hora_actual <= $hora_clase3){
        updateFalta($hora_clase2,2);
    }elseif($hora_actual >= $hora_clase3 && $hora_actual <= $hora_clase4){
        updateFalta($hora_clase3,3);
    }elseif($hora_actual >= $hora_clase4 && $hora_actual <= $hora_clase5){
        updateFalta($hora_clase4,4);
    }elseif($hora_actual >= $hora_clase5 && $hora_actual <= $hora_clase6){
        updateFalta($hora_clase5,5);
    }else*/if($hora_actual >= $hora_clase6){
        updateFalta($hora_clase6,6);
    } 

    updateFalta($hora_clase6,2);

function updateFalta($hora_clase, $hora) {

    // Comprobar si el alumno ha llegado puntual o con retaso
    // En caso de no asistir a clase, en la BD ya figura un F

    $hora_actual = date('H') * 60 + date('i');
    
    if ($hora_actual > $hora_clase && $hora_actual < $hora_clase+10) {
        echo "presente";
        setFalta($GLOBALS["idAlumno"],'P');
    } else if ($hora_actual > $hora_clase && $hora_actual < $hora_clase+25) {
        echo "retraso";
        setFalta($GLOBALS["idAlumno"],'R');
    } else{
        echo "falta";
    }

}
function setFalta($idAlumno,$F_asistencia) {

    /*  Obtiene de la BD un idFalta con los siguientes campos:
            - idAlumno
            - Fecha del día (YYYY/mm/dd)
            - Hora de la clase (1-6)
        Luego como solo recibe un único registro, actualiza el
        campo F_asistenia con la letra correspondiente (P ó R)
    */

    require("../config.php");
    $query = "SELECT * FROM `faltas` WHERE `F_idAlumno`=".$idAlumno." AND `F_idFalta`=(SELECT MAX(`F_idFalta`) FROM `faltas` WHERE `F_idAlumno`=".$idAlumno.")";

    if($result = mysqli_query($con, $query)){
        if(mysqli_num_rows($result) > 0){
            while($row = mysqli_fetch_array($result)){
                echo "idFalta". $row['F_idFalta'];

                $sql = "UPDATE `faltas` SET `F_asistencia`='".$F_asistencia."' WHERE `F_idFalta`=".$row['F_idFalta'];
                if(mysqli_query($con, $sql)){
                    echo "Records were updated successfully.";
                } else {
                    echo "ERROR: Could not able to execute $sql. " . mysqli_error($con);
                }
            }
            // Free result set
            mysqli_free_result($result);
        } else{
            echo "No records matching your query were found.";
        }
    } else{
        echo "ERROR: Could not able to execute $sql. " . mysqli_error($con);
    }

}