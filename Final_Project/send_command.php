<?php
// Set the serial port path and baud rate
$serial_port = "/dev/ttyUSB0";  // make rhe serial point usb0
$baud_rate = "9600";            //set baud rate

// Configure the serial port once 
exec("stty -F $serial_port $baud_rate cs8 -cstopb -parenb");
// If a drink is selected, process command
if (isset($_GET['drink'])) {
    $command = strtoupper(trim($_GET['drink']));
    // commands to characters
    $drink_map = [
        'COKE' => 'C',
        'SPRITE' => 'S',
        'WATER' => 'W'
    ];
    $avr_command = $drink_map[$command];
    // Open the serial port

    $serial = fopen($serial_port, "w+");
    //this only if  the serial port is not open
    if (!$serial) {
        echo json_encode(['status' => 'error', 'mess' => 'Unable to open serial port']);
        exit;
    }
    // Send command to AVR using the UART

    fwrite($serial, $avr_command);
    fclose($serial);
}
// Read AVR response (status)

$serial = fopen($serial_port, "r");
//reads one charac as the status
$response = fread($serial, 1);
fclose($serial);
switch (trim($response)) {
    case 'Q':
        $mes = "Dispensing Coke";
        break;
    case 'R':
        $mes = "Dispensing Sprite";
        break;
    case 'T':
        $mes = "Dispensing Water";
        break;
    case 'K':
        $mes = "Fill Up Coke";
        break;
    case 'M':
        $mes = "Fill Up Sprite";
        break;
    case 'P':
        $mes = "Fill Up Water";
        break;
    case 'J':
        $mes = "Place Cup";
        break;
    case 'D':
        $mes = "DONE!!!";
        break;
    case 'F':
        $mes = "Overflow/Spillage (Clean UP)";
        break;
    default:
        $mes = "Select Drink :)";
}
// JSON response checks for the status and returns the message
echo json_encode(['status' => 'sucess', 'mess' => $mes]);
?>

