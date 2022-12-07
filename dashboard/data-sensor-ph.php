<?php
include "dbcon.php";

$pH = $database->getReference('pH')->getSnapshot()->getValue();

echo $pH;
?>