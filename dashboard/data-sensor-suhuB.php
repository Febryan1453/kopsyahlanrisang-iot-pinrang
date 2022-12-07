<?php
include "dbcon.php";

$SuhuB = $database->getReference('SuhuB')->getSnapshot()->getValue();

echo $SuhuB."<sup>o</sup>C";
?>