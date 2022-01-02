<?php
    
    $file = fopen("control_data.php", "w");
    $file2 = fopen("control_huella.php", "w");
    $file3 = fopen("control_reiniciar.php", "w");

    switch ($_GET["option"]) {
        case 'esperando':
            fwrite($file, "<?php" . PHP_EOL);
            fwrite($file, "echo 'Esperando...';" . PHP_EOL);
            fwrite($file, "?>" . PHP_EOL);
            break;
        case 'ponerDedo':
            fwrite($file, "<?php" . PHP_EOL);
            fwrite($file, "echo 'Ponga el dedo';" . PHP_EOL);
            fwrite($file, "?>" . PHP_EOL);
            break;
        case 'quitarDedo':
            fwrite($file, "<?php" . PHP_EOL);
            fwrite($file, "echo 'Quite el dedo';" . PHP_EOL);
            fwrite($file, "?>" . PHP_EOL);
            break;
        case 'espera':
            fwrite($file, "<?php" . PHP_EOL);
            fwrite($file, "echo 'Esperando...';" . PHP_EOL);
            fwrite($file, "?>" . PHP_EOL);
            break;
        case 'almacenado':
            fwrite($file, "<?php" . PHP_EOL);
            fwrite($file, "echo 'Huella almacenada';" . PHP_EOL);
            fwrite($file, "?>" . PHP_EOL);
            fwrite($file2, "<?php" . PHP_EOL);
            fwrite($file2, "echo '".$_GET["huella"]."';" . PHP_EOL);
            fwrite($file2, "?>" . PHP_EOL);
            break;
        case 'reiniciar':
            fwrite($file3, $_GET["reiniciar"] . PHP_EOL);
            break;
    }

    fclose($file);
    fclose($file2);
    fclose($file3);

?>