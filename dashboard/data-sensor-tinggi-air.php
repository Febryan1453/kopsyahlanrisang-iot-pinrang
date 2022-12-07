<?php
include "dbcon.php";

$TinggiAir = $database->getReference('TinggiAir')->getSnapshot()->getValue();

echo "Ketinggian Air= ".$TinggiAir." cm";
?>