<?php
include "dbcon.php";

$DO = $database->getReference('DO')->getSnapshot()->getValue();

echo $DO. " ppm";
?>