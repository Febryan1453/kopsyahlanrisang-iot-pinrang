<?php
  $koneksi    = mysqli_connect("localhost", "root", "", "db_tambak_udang");
  $sql    = mysqli_query($koneksi, "SELECT * FROM tbl_kualitas_air_lanrisang");
  $count  = mysqli_num_rows($sql);
  $mulai = $count - 15;
  //$jam = mysqli_query($koneksi, "SELECT jam FROM tbl_kualitas_air_lanrisang LIMIT 15 OFFSET $mulai");
  //$suhua = mysqli_query($koneksi, "SELECT jam, suhua FROM tbl_kualitas_air_lanrisang LIMIT 15 OFFSET $mulai");
  //$suhub = mysqli_query($koneksi, "SELECT jam, suhub FROM tbl_kualitas_air_lanrisang LIMIT 15 OFFSET $mulai");
  //while ($p = mysqli_fetch_array($jam)) { echo '"'.$p['jam'].'"'.',' ;}
?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Real Time Monitor Suhu Air</title>
    <script src="js/Chart.js"></script>
    <style type="text/css">
            .container {
                width: 100%;
                margin: 15px auto;
            }
    </style>
  </head>
  <body>

    <div class="container">
        <canvas id="linechart1" width="100" height="100"></canvas>
    </div>

  </body>
</html>

<script  type="text/javascript">
  var ctx = document.getElementById("linechart1").getContext("2d");
  var data = {
            //labels: [<?php //while ($p = mysqli_fetch_array($jam)) { echo '"'.$p['jam'].'"'.',' ;}?>],
            labels: [ <?php 
                          if ($count < 15){
                              echo "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
                          }
                          else
                          {
                              $jam = mysqli_query($koneksi, "SELECT jam FROM tbl_kualitas_air_lanrisang LIMIT 15 OFFSET $mulai");
                              while ($p = mysqli_fetch_array($jam)) 
                                { 
                                  echo '"'.$p['jam'].'"'.',';
                                }
                          }
                        ?>
                      ],
            datasets: [
                  {
                    label: "Suhu Atas",
                    fill: false,
                    lineTension: 0.1,
                    backgroundColor: "#ff0000",
                    borderColor: "#ff0000",
                    pointHoverBackgroundColor: "#ff0000",
                    pointHoverBorderColor: "#ff0000",
                    data: [
                          <?php 
                          if ($count < 15){
                              echo "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
                          }
                          else
                          {
                            $suhua = mysqli_query($koneksi, "SELECT suhua FROM tbl_kualitas_air_lanrisang LIMIT 15 OFFSET $mulai");
                              while ($p = mysqli_fetch_array($suhua)) 
                                { 
                                  echo floatval($p['suhua']) . ",";
                                }
                          }
                          ?>
                          ]
                  },
                  {
                    label: "Suhu Bawah",
                    fill: false,
                    lineTension: 0.1,
                    backgroundColor: "#F07124",
                    borderColor: "#F07124",
                    pointHoverBackgroundColor: "#F07124",
                    pointHoverBorderColor: "#F07124",
                    //data: [<?php //while ($p = mysqli_fetch_array($suhub)) { echo floatval($p['suhub']) . ",";}?>]
                    data: [
                          <?php 
                          if ($count < 15){
                              echo "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
                          }
                          else
                          {
                              $suhub = mysqli_query($koneksi, "SELECT suhub FROM tbl_kualitas_air_lanrisang LIMIT 15 OFFSET $mulai");
                              while ($p = mysqli_fetch_array($suhub)) 
                                { 
                                  echo floatval($p['suhub']) . ",";
                                }
                          }
                          ?>
                          ]
                  }
                  ]
          };

  var myBarChart1 = new Chart(ctx, {
            type: 'line',
            data: data,
            options: {
            legend: {
              display: true
            },
            barValueSpacing: 10,
            scales: {
              yAxes: [{
                  ticks: {
                      min: 0,
                  }
              }],
              xAxes: [{
                          gridLines: {
                              color: "rgba(0, 0, 0, 0)",
                          }
                      }]
              }
          }
        });
</script>