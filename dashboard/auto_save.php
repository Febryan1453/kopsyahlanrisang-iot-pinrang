<?php  

date_default_timezone_set('Asia/Makassar');

$koneksi    = mysqli_connect("localhost", "root", "", "db_tambak_udang");

include "dbcon.php";

$jam = date("h:i:sa");
$tgl = date("l, d-M-Y");
$DO = $database->getReference('DO')->getSnapshot()->getValue();
$pH = $database->getReference('pH')->getSnapshot()->getValue();
$SuhuA = $database->getReference('SuhuA')->getSnapshot()->getValue();
$SuhuB = $database->getReference('SuhuB')->getSnapshot()->getValue();
$TinggiAir = $database->getReference('TinggiAir')->getSnapshot()->getValue();

$query = mysqli_query($koneksi,"INSERT INTO `tbl_kualitas_air_lanrisang`(`tgl`, `jam`, `do`, `ph`, `suhua`, `suhub`, `tinggiair`) VALUES ('$tgl','$jam','$DO','$pH','$SuhuA','$SuhuB','$TinggiAir')");

?>