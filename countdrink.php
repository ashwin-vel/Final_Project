<!DOCTYPE html>
<html>
<head>
 <?php
           // $drink_name = $_GET['drink']; 
 //$drink_name = $_GET['drink']; 
 $drink_name = $_GET['drink'];

            $server = "localhost";
            $username = "root";
            $password = "ashwin";
            $database = "countd";
            $conn = mysqli_connect($server, $username, $password, $database);
            
            // Check for successful connection
            if (!$conn) {
              die("Connection failed: {mysqli_connect_error()}");
            }
            $sql = "UPDATE drink_counts SET count = count + 1 WHERE drink_name = '{$drink_name}'";
            $result= mysqli_query($conn, $sql);
           $view_table = "select * FROM drink_counts;";
           $result1= mysqli_query($conn, $view_table);
           $largest_count="SELECT drink_name FROM drink_counts ORDER BY count DESC LIMIT 1";
            $result2= mysqli_query($conn, $largest_count);
     
        ?>
    </head>
  <body>

<?php
 while ($row = $result2-> fetch_assoc()) {
echo "<h1> $row[drink_name] is the most popular drink </h1>";
      }
?>

       <table class="center">
       <tr>
          <th>Drink Name</th>
	  <th>Count</th>
      </tr>
<?php
       if ($result1->num_rows > 0) {
          while ($row = $result1-> fetch_assoc()) {
            echo "<tr><td>" . $row["drink_name"] . "</td><td>" . $row["count"] . "</td></tr>";
            }
         }

     //while ($row = $result2-> fetch_assoc()) {
//	 echo  . $row["drink_name"]. "is the most popular drink";
//	}
 ?> 
    </table> 
<?php
 while ($row = $result2-> fetch_assoc()) {
echo "<h1> $row[drink_name] is the most popular drink </h1>";
      }
?>                 
    </body>
</html>

