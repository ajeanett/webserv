#!/usr/bin/php
<?php
echo "Content-Type: text/plain\r\n";
echo "\r\n";

$str = <<<EOD
Example of string
Hello world!
This is PHP!
EOD;
$name = 'Vova';
echo <<<EOT
My name is "$name".\n
EOT;
echo $str;
echo "\n12345\n";
?>
