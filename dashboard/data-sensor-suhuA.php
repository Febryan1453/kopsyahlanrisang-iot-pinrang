<?php
include "dbcon.php";

$SuhuA = $database->getReference('SuhuA')->getSnapshot()->getValue();

echo $SuhuA."<sup>o</sup>C";
?>