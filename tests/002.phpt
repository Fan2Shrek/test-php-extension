--TEST--
test1() Basic test
--EXTENSIONS--
test
--FILE--
<?php
$ret = test1();

var_dump($ret);
?>
--EXPECT--
The extension test is loaded and working!
NULL
