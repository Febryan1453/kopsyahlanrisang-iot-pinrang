<?php
  $koneksi    = mysqli_connect("localhost", "root", "", "db_tambak_udang");
  $sql    = mysqli_query($koneksi, "SELECT * FROM tbl_kualitas_air_lanrisang");
  $count  = mysqli_num_rows($sql);
  $mulai = $count - 15;
  //echo $count."<br>";
  //echo $mulai;
  //$jam = mysqli_query($koneksi, "SELECT jam FROM tbl_kualitas_air_lanrisang LIMIT 15 OFFSET $mulai");
  //$do = mysqli_query($koneksi, "SELECT jam, do FROM tbl_kualitas_air_lanrisang LIMIT 15 OFFSET $mulai");
  //$ph = mysqli_query($koneksi, "SELECT jam, ph FROM tbl_kualitas_air_lanrisang LIMIT 15 OFFSET $mulai");
  //while ($p = mysqli_fetch_array($jam)) { echo '"'.$p['jam'].'"'.',' ;}
?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Real Time Monitoring pH dan DO</title>
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
        <canvas id="linechart" width="100" height="100"></canvas>
    </div>

  </body>
</html>

<script  type="text/javascript">
  var ctx = document.getElementById("linechart").getContext("2d");
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
                    label: "DO",
                    fill: false,
                    lineTension: 0.1,
                    backgroundColor: "#29B0D0",
                    borderColor: "#29B0D0",
                    pointHoverBackgroundColor: "#29B0D0",
                    pointHoverBorderColor: "#29B0D0",
                    //data: [<?php //while ($p = mysqli_fetch_array($do)) { echo floatval($p['do']) . ",";}?>]
                    data: [
                          <?php 
                          if ($count < 15){
                              echo "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
                          }
                          else
                          {
                            $do = mysqli_query($koneksi, "SELECT do FROM tbl_kualitas_air_lanrisang LIMIT 15 OFFSET $mulai");
                              while ($p = mysqli_fetch_array($do)) 
                                { 
                                  echo intval($p['do']) . ",";
                                }
                          }
                          ?>
                         ]
                  },
                  {
                    label: "pH",
                    fill: false,
                    lineTension: 0.1,
                    backgroundColor: "#2A516E",
                    borderColor: "#2A516E",
                    pointHoverBackgroundColor: "#2A516E",
                    pointHoverBorderColor: "#2A516E",
                    //data: [<?php //while ($p = mysqli_fetch_array($ph)) { echo floatval($p['ph']) . ",";}?>]
                    data: [
                          <?php 
                          if ($count < 15){
                            echo "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
                          }
                          else
                          {
                            $ph = mysqli_query($koneksi, "SELECT ph FROM tbl_kualitas_air_lanrisang LIMIT 15 OFFSET $mulai");
                              while ($p = mysqli_fetch_array($ph)) 
                                { 
                                  echo floatval($p['ph']) . ",";
                                }
                          }
                          ?>
                          ]
                   }
                   ]
          };

  var myBarChart = new Chart(ctx, {
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