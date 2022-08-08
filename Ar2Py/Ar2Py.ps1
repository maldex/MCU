[System.IO.Ports.SerialPort]::getportnames()
$port= new-Object System.IO.Ports.SerialPort COM4,9600,None,8,one
$port.Open()
$port.Readtimeout = 1000
$port.Read(6)