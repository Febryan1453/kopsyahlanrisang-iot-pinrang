<?php

require __DIR__.'/vendor/autoload.php';

use Kreait\Firebase\Factory;

$factory = (new Factory())
    ->withProjectId('coba-esp32-39fd9')
    ->withDatabaseUri('https://coba-esp32-39fd9-default-rtdb.asia-southeast1.firebasedatabase.app/');

$database = $factory->createDatabase();

?>