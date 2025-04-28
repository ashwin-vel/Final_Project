<?php
// Set the serial port path and baud rate
$serial_port = "/dev/ttyUSB0";  // Update if necessary
$baud_rate = "9600";            // Ensure it matches AVR settings


// Configure the serial port once
//exec("stty -F $serial_port $baud_rate cs8 -cstopb -parenb");
// If a drink is selected, process command
if (isset($_GET['drink'])) {
   $command = strtoupper(trim($_GET['drink']));
   // Map commands to characters
   $drink_map = [
       'COKE' => 'C',
       'SPRITE' => 'S',
       'WATER' => 'W',
       'STOP' => 'G'
   ];
   if (!array_key_exists($command, $drink_map)) {
       echo json_encode(['status' => 'error', 'message' => 'Invalid drink selected']);
       exit;
   }
   $avr_command = $drink_map[$command];
   // Open the serial port
 
   
   $serial = fopen($serial_port, "w+");
   if (!$serial) {
       echo json_encode(['status' => 'error', 'message' => 'Unable to open serial port']);
       exit;
   }
   // Send command to AVR
 
   fwrite($serial, $avr_command);
//   fclose($serial);
}
// Read AVR response (status)

$serial = fopen($serial_port, "r");
//stream_set_blocking($serial, false);

$response = fread($serial, 1);
//fclose($serial);
// Process AVR response
$mes = "Unknown Response";
switch (trim($response)) {
   case 'Q': $mes = "Dispensing Coke"; break;
   case 'R': $mes = "Dispensing Sprite"; break;
   case 'T': $mes = "Dispensing Water"; break;
   case 'K': $mes = "Fill Up Coke"; break;
   case 'M': $mes = "Fill Up Sprite"; break;
   case 'P': $mes = "Fill Up Water"; break;
   case 'J': $mes = "Place Cup"; break;
   case 'D': $mes = "DONE!!!"; break;
   case 'F': $mes = "Overflow/Spillage (Clean UP)"; break;
   case 'G': $mes = "Stopped"; break;
   default:  $mes = "Select Drink :)";
}
// Return JSON response
echo json_encode(['status' => 'success', 'message' => $mes]);
//fflush($serial);
//header("refresh: 1;")
?>
