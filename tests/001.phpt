--TEST--
Check if test is loaded
--EXTENSIONS--
test
--FILE--
<?php
echo 'The extension "test" is available';
?>
--EXPECT--
The extension "test" is available
